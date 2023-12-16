//
// PrivateKeyFactory.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  PrivateKeyFactory
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/PrivateKeyFactory.h"
#include "Poco/Net/SSLManager.h"


namespace Poco {
namespace Net {


PrivateKeyFactory::PrivateKeyFactory()
= default;


PrivateKeyFactory::~PrivateKeyFactory()
= default;


PrivateKeyFactoryRegistrar::PrivateKeyFactoryRegistrar(const std::string& name, PrivateKeyFactory* pFactory)
{
	SSLManager::instance().privateKeyFactoryMgr().setFactory(name, pFactory);
}


PrivateKeyFactoryRegistrar::~PrivateKeyFactoryRegistrar()
= default;


} } // namespace Poco::Net
