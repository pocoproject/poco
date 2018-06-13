//
// tcpserver.cpp
//
// This sample demonstrates a TCP server.
//
// Copyright (c) 2005-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <iostream>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFilter;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::StreamSocket;
using Poco::UInt16;
using Poco::NumberParser;
using Poco::Logger;
using Poco::Event;
using Poco::NamedEvent;
using Poco::Process;
using Poco::ProcessImpl;
using Poco::Exception;


namespace
{
	class ClientConnection: public TCPServerConnection
	{
	public:
		ClientConnection(const StreamSocket& s): TCPServerConnection(s)
		{
		}

		void run()
		{
			StreamSocket& ss = socket();
			try
			{
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0)
				{
					std::cout << "Received " << n << " bytes:" << std::endl;
					std::string msg;
					Logger::formatDump(msg, buffer, n);
					std::cout << msg << std::endl;
					n = ss.receiveBytes(buffer, sizeof(buffer));
				}
			}
			catch (Exception& exc)
			{
				std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
			}
		}
	};

	typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;
#if defined(POCO_OS_FAMILY_WINDOWS)
	NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
	Event terminator;
#endif
}


int main(int argc, char** argv)
{
	try
	{
		Poco::UInt16 port = NumberParser::parse((argc > 1) ? argv[1] : "2001");

		TCPServer srv(new TCPFactory(), port);
		srv.start();

		std::cout << "TCP server listening on port " << port << '.'
			 << std::endl << "Press Ctrl-C to quit." << std::endl;

		terminator.wait();
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}

	return 0;
}
