//
// ConnectionFactory.h
//
// $Id: //poco/1.4/Shell/include/Poco/Shell/ConnectionFactory.h#1 $
//
// Library: Shell
// Package: Shell
// Module:  Connection
//
// Definition of the ConnectionFactory class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef Shell_ConnectionFactory_INCLUDED
#define Shell_ConnectionFactory_INCLUDED


#include "Poco/Shell/Shell.h"
#include "Poco/Net/TCPServerConnectionFactory.h"


namespace Poco {
namespace Shell {


class CommandProcessor;


class Shell_API ConnectionFactory: public Poco::Net::TCPServerConnectionFactory
	/// The factory for Connection objects.
{
public:
	ConnectionFactory(CommandProcessor& processor, const std::string& authServiceName);
		/// Creates the ConnectionFactory.
		
	~ConnectionFactory();
		/// Destroys the ConnectionFactory.
		
	// TCPServerConnectionFactory
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);

private:
	ConnectionFactory();
	
	CommandProcessor& _commandProcessor;
	std::string _authServiceName;
};


} } // namespace Poco::Shell


#endif // Shell_ConnectionFactory_INCLUDED
