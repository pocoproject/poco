//
// tcpclient.cpp
//
// This sample demonstrates the StreamSocket and SocketStream classes.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include <iostream>


using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Path;
using Poco::Exception;


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Path p(argv[0]);
		std::cout << "usage: " << p.getBaseName() << " IP:PORT DATA" << std::endl;
		std::cout << "       sends DATA to IP:PORT" << std::endl;
		return 1;
	}
	std::string addr(argv[1]);
	std::string str(argv[2]);

	try
	{
		SocketAddress sa(addr);
		StreamSocket sock(sa);

		sock.sendBytes(str.data(), static_cast<int>(str.length()));
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}

	return 0;
}
