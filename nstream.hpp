#pragma once
#ifndef NSTREAM_HPP_
#define NSTREAM_HPP_
/*
 * nstream.hpp
 *
 *  Created on: 2022. 11. 18.
 *      Author: sjusju
 */
#include <iostream>

#include <netinet/in.h>

#include "basic_socketbuf.hpp"

namespace nstream
{
inline namespace v20231220
{

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_instream: public std::basic_istream<CharT, Traits>
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

	using istream_type = std::basic_istream<CharT, Traits>;
	using streambuf_type = basic_socketbuf<CharT, Traits>;
private:
	static constexpr std::ios_base::openmode default_openmode = std::ios_base::in;
public:
	basic_instream():
		istream_type{&buffer},
		buffer{}
	{}
	basic_instream(const basic_instream&) = delete;
	basic_instream(basic_instream&& R):
		istream_type{&buffer},
		buffer{std::move(R.buffer)}
	{}
	basic_instream(in_port_t port, std::ios_base::openmode m = default_openmode):
		istream_type{&buffer},
		buffer{}
	{
		open(port, m);
	}
	basic_instream(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode):
		istream_type{&buffer},
		buffer{}
	{
		open(ipv4, port, m);
	}
	basic_instream(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode):
		istream_type{&buffer},
		buffer{}
	{
		open(host_name, port, m);
	}
	bool is_open() const
	{
		return buffer.is_open();
	}
	void open(in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(port, m | default_openmode);
	}
	void open(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(ipv4, port, m | default_openmode);
	}
	void open(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(host_name, port, m | default_openmode);
	}
	void close()
	{
		buffer.close();
	}
	streambuf_type* rdbuf() const
	{
		return const_cast<streambuf_type*>(&buffer);
	}
private:
	streambuf_type buffer;
};

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_onstream: public std::basic_ostream<CharT, Traits>
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

	using ostream_type = std::basic_ostream<CharT, Traits>;
	using streambuf_type = basic_socketbuf<CharT, Traits>;
private:
	static constexpr std::ios_base::openmode default_openmode = std::ios_base::out;
public:
	basic_onstream():
		ostream_type{&buffer},
		buffer{}
	{}
	basic_onstream(const basic_onstream&) = delete;
	basic_onstream(basic_onstream&&):
		ostream_type{&buffer},
		buffer{}
	{}
	basic_onstream(in_port_t port, std::ios_base::openmode m = default_openmode):
		ostream_type{&buffer},
		buffer{}
	{
		open(port, m);
	}
	basic_onstream(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode):
		ostream_type{&buffer},
		buffer{}
	{
		open(ipv4, port, m);
	}
	basic_onstream(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode):
		ostream_type{&buffer},
		buffer{}
	{
		open(host_name, port, m);
	}
	bool is_open() const
	{
		return buffer.is_open();
	}
	void open(in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(port, m | default_openmode);
	}
	void open(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(ipv4, port, m | default_openmode);
	}
	void open(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(host_name, port, m | default_openmode);
	}
	void close()
	{
		buffer.close();
	}
	streambuf_type* rdbuf() const
	{
		return const_cast<streambuf_type*>(&buffer);
	}
private:
	streambuf_type buffer;
};

template<class CharT, class Traits = std::char_traits<CharT>>
class basic_netstream: public std::basic_iostream<CharT, Traits>
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using int_type = typename Traits::int_type;
	using pos_type = typename Traits::pos_type;
	using off_type = typename Traits::off_type;

	using iostream_type = std::basic_iostream<CharT, Traits>;
	using streambuf_type = basic_socketbuf<CharT, Traits>;
private:
	static constexpr std::ios_base::openmode default_openmode = std::ios_base::in | std::ios_base::out;
public:
	basic_netstream():
		iostream_type{&buffer},
		buffer{}
	{}
	basic_netstream(const basic_netstream&) = delete;
	basic_netstream(basic_netstream&&):
		iostream_type{&buffer},
		buffer{}
	{}
	basic_netstream(in_port_t port, std::ios_base::openmode m = default_openmode):
		iostream_type{&buffer},
		buffer{}
	{
		open(port);
	}
	basic_netstream(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode):
		iostream_type{&buffer},
		buffer{}
	{
		open(ipv4, port);
	}
	basic_netstream(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode):
		iostream_type{&buffer},
		buffer{}
	{
		open(host_name, port);
	}
	bool is_open() const
	{
		return buffer.is_open();
	}
	void open(in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(port, m | default_openmode);
	}
	void open(in_addr_t ipv4, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(ipv4, port, m | default_openmode);
	}
	void open(const char* host_name, in_port_t port, std::ios_base::openmode m = default_openmode)
	{
		buffer.open(host_name, port, m | default_openmode);
	}
	void close()
	{
		buffer.close();
	}
	streambuf_type* rdbuf() const
	{
		return const_cast<streambuf_type*>(&buffer);
	}
private:
	streambuf_type buffer;
};

using instream = basic_instream<char>;
using onstream = basic_onstream<char>;
using netstream = basic_netstream<char>;

using winstream = basic_instream<wchar_t>;
using wonstream = basic_onstream<wchar_t>;
using wnetstream = basic_netstream<wchar_t>;

} // inline namespace v*
} // namespace sjusju

#endif // #ifndef NSTREAM_HPP_