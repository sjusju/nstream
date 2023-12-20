#pragma once
#ifndef BASIC_SOCKETBUF_HPP_
#define BASIC_SOCKETBUF_HPP_
/*
 * basic_socketbuf.hpp
 *
 *  Created on: 2022. 11. 16.
 *      Author: sjusju
 */
#include <iostream>
#include <algorithm>

#include <cstdint>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

namespace nstream
{
inline namespace v20231220
{

#define MTU 1500
#define TCPIP_HEADER 40

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_socketbuf: public std::basic_streambuf<CharT, Traits>
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;
public:
	char_type* beginptr()
	{
		return this->pbase();
	}
	basic_socketbuf():
		fd{-1},
		mode{},
		bufsize{MTU - TCPIP_HEADER}
	{}
	basic_socketbuf(const basic_socketbuf&) = delete;
	basic_socketbuf(basic_socketbuf&&) = default;
	virtual ~basic_socketbuf()
	{
		close();
	}
	bool is_open() const
	{
		return fd != -1;
	}
	basic_socketbuf* fromSocket(int&& socket_fd, std::ios_base::openmode m)
	{
		if (is_open()) return nullptr;
		fd = socket_fd;
		openmode(m);
		return this;
	}
	basic_socketbuf* open(in_port_t port, std::ios_base::openmode m)
	{
		if (is_open()) return nullptr;
		int server = socket(AF_INET, SOCK_STREAM, 0);
		if (server == -1) return nullptr;
		sockaddr_in addr = {};
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		if (bind(server, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) return nullptr;
		if (listen(server, 1) == -1) return nullptr;
		fd = accept(server, nullptr, nullptr);
		::close(server);
		if (fd == -1) return nullptr;
		openmode(m);
		return this;
	}
	basic_socketbuf* open(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m)
	{
		openmode(m);
		return open_client_network_order(htonl(ipv4), htons(port));
	}
	basic_socketbuf* open(const char* host_name, in_port_t port, std::ios_base::openmode m)
	{
		hostent* server = gethostbyname(host_name);
		if (!server) return nullptr;
		openmode(m);
		return open_client_network_order(*reinterpret_cast<in_addr_t*>(server->h_addr_list[0]), htons(port));
	}
	basic_socketbuf* close()
	{
		if (is_open()) ::close(fd);
		fd = -1;
		return this;
	}
	bool is_open_for_read() const
	{
		return mode & std::ios_base::in;
	}
	bool is_open_for_write() const
	{
		return mode & std::ios_base::out;
	}
	void openmode(std::ios_base::openmode m)
	{
		if (m & std::ios_base::in)
		{
			if (!is_open_for_read())
			{
				char_type* buffer = new char_type[bufsize + 1];
				this->setg(buffer, buffer + bufsize, buffer + bufsize);
			}
		}
		else
		{
			if (is_open_for_read())
			{
				delete[] this->eback();
				this->setg(nullptr, nullptr, nullptr);
			}
		}
		if ((m & std::ios_base::out) || (m & std::ios_base::app))
		{
			if (!is_open_for_write())
			{
				char_type* buffer = new char_type[bufsize + 1];
				this->setp(buffer, buffer + bufsize);
			}
		}
		else
		{
			if (is_open_for_write())
			{
				this->overflow();
				delete[] this->pbase();
				this->setp(nullptr, nullptr);
			}
		}
		mode = m;
	}
protected:
	virtual int_type overflow(int_type c = traits_type::eof()) override
	{
		if (!is_open_for_write()) return traits_type::eof();
		char_type* pbase_ = this->pbase();
		char_type* pptr_ = this->pptr();
		char_type* epptr_ = this->epptr();
		if (!traits_type::eq_int_type(c, traits_type::eof()))
		{
			*pptr_ = traits_type::to_char_type(c);
			this->pbump(1);
			pptr_ += 1;
		}
		if (send(fd, pbase_, pptr_ - pbase_, 0) == -1)
		{
			return traits_type::eof();
		}
		this->setp(pbase_, epptr_);
		return traits_type::not_eof(c);
	}
	virtual int_type underflow() override
	{
		if (!is_open_for_read()) return traits_type::eof();
		char_type* eback_ = this->eback();
		char_type* gptr_ = this->gptr();
		char_type* egptr_ = this->egptr();
		const ptrdiff_t remains = egptr_ - gptr_;
		char_type* gptr_new = eback_ + remains;
		const size_t len = bufsize - remains;
		if (remains)
		{
			std::copy(gptr_, egptr_, eback_);
		}
		const ssize_t length_got = recv(fd, gptr_new, len, 0);
		if (length_got <= 0)
		{
			this->setg(eback_, eback_, eback_);
			return traits_type::eof();
		}
		this->setg(eback_, eback_, gptr_new + length_got);
		return traits_type::to_int_type(*gptr_new);
	}
	virtual int sync() override
	{
		if (this->pbase() < this->pptr())
		{
			if (traits_type::eq_int_type(overflow(), traits_type::eof()))
			{
				return -1;
			}
		}
		return 0;
	}
private:
	basic_socketbuf* open_client_network_order(in_addr_t ipv4, in_port_t port)
	{
		if (is_open()) return nullptr;
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1) return nullptr;
		sockaddr_in addr = {};
		addr.sin_addr.s_addr = ipv4;
		addr.sin_family = AF_INET;
		addr.sin_port = port;
		if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) return nullptr;
		return this;
	}
private:
	int fd;
	std::ios_base::openmode mode;
	size_t bufsize;
};

using socketbuf = basic_socketbuf<char>;

using wsocketbuf = basic_socketbuf<wchar_t>;

} // inline namespace v*
} // namespace sjusju

#endif // #ifndef BASIC_SOCKETBUF_HPP_