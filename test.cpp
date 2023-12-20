/*
 * test.cpp
 *
 *  Created on: 2022. 11. 17.
 *      Author: sjusju
 */
#include <iostream>
#include <string>

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

int main(int argc, char** argv)
{
	bool send = argc == 2 && std::string(argv[1]) == std::string("send");
	nstream::instream ins;
	nstream::onstream ons;
	if (send)
	{
		ons.open("localhost", 33333);
	}
	else
	{
		ins.open(33333);
	}
	std::cout << "Opened!" << std::endl;
	forward(send ? std::cin : ins, send ? ons : std::cout);
	return 0;
}