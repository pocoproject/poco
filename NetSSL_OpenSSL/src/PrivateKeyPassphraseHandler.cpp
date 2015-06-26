//
// PrivateKeyPassphraseHandler.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/src/PrivateKeyPassphraseHandler.cpp#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  PrivateKeyPassphraseHandler
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Delegate.h"


using Poco::Delegate;


namespace Poco {
namespace Net {


PrivateKeyPassphraseHandler::PrivateKeyPassphraseHandler(bool onServerSide): _serverSide(onServerSide)
{
	SSLManager::instance().PrivateKeyPassphraseRequired += Delegate<PrivateKeyPassphraseHandler, std::string>(this, &PrivateKeyPassphraseHandler::onPrivateKeyRequested);
}


PrivateKeyPassphraseHandler::~PrivateKeyPassphraseHandler()
{
	SSLManager::instance().PrivateKeyPassphraseRequired -= Delegate<PrivateKeyPassphraseHandler, std::string>(this, &PrivateKeyPassphraseHandler::onPrivateKeyRequested);
}


} } // namespace Poco::Net
