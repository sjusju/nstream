/*
 * NetPipe.cpp
 *
 *  Created on: 2022. 11. 19.
 *      Author: sjusju
 */
#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>

#include "nstream.hpp"

using socketbuf = nstream::basic_socketbuf<char>;
using asdf = std::stringstream;

void forward(std::istream& is, std::ostream& os)
{
	std::string str;
	for (;is && os;)
	{
		std::getline(is, str);
		os << str << std::endl;
	}
}

void printHelpAndExit(const char* arg0)
{
	std::cout << "Usage: " << arg0 << " (send | recv) --port <port number> [--name <server name>]\n";
	exit(0);
}

int main(int argc, char** argv)
{
	std::string name;
	uint16_t port;
	std::string mode;
	{
		bool expect_name = false;
		bool expect_port = false;
		for (int i = 1; i < argc; i ++)
		{
			std::string arg = argv[i];
			if (arg == "help" || arg == "--help" || arg == "?")
			{
				printHelpAndExit(argv[0]);
			}
			else if (expect_name)
			{
				expect_name = false;
				name = arg;
			}
			else if (expect_port)
			{
				expect_port = false;
				try
				{
					unsigned long tmp = std::stoul(arg);
					if (tmp > 65535)
					{
						std::cout << "Port number should be in range [0, 65535]." << std::endl;
						printHelpAndExit(argv[0]);
					}
					port = (uint16_t) tmp;
				}
				catch (std::invalid_argument const& e)
				{
					std::cout << "Expected valid port number, not \"" << arg << "\"." << std::endl;
					printHelpAndExit(argv[0]);
				}
			}
			else if (arg == std::string("send"))
			{
				mode = arg;
			}
			else if (arg == std::string("recv"))
			{
				mode = arg;
			}
			else if (arg == std::string("--name"))
			{
				expect_name = true;
			}
			else if (arg == std::string("--ip"))
			{
				expect_name = true;
			}
			else if (arg == std::string("--port"))
			{
				expect_port = true;
			}
			else
			{
				std::cout << "Unrecognized argument: " << arg << std::endl;
				printHelpAndExit(argv[0]);
			}
		}
		if (expect_name)
		{
			std::cout << "Expected server name/ip address." << std::endl;
			printHelpAndExit(argv[0]);
		}
		if (expect_port)
		{
			std::cout << "Expected port number." << std::endl;
			printHelpAndExit(argv[0]);
		}
		if (mode != "send" && mode != "recv")
		{
			std::cout << "Mode flag(send/recv) should be provided." << std::endl;
			printHelpAndExit(argv[0]);
		}
	}
	if (mode == "send")
	{
		nstream::onstream ons;
		if (name.length())
		{
			ons.open(name.data(), port);
		}
		else
		{
			ons.open(port);
		}
		forward(std::cin, ons);
	}
	else
	{
		nstream::instream ins;
		if (name.length())
		{
			ins.open(name.data(), port);
		}
		else
		{
			ins.open(port);
		}
		forward(ins, std::cout);
	}
	return 0;
}