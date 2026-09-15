//
// CredentialsStore.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  CredentialsStore
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/CredentialsStore.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


CredentialsStore::CredentialsStore()
{
	_random.seed();
}

	
CredentialsStore::~CredentialsStore()
{
}

	
Poco::UInt64 CredentialsStore::addCredentials(const Credentials& creds)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Poco::UInt64 authToken = generateAuthToken();
	_credentials[authToken] = creds;
	return authToken;
}

	
const Credentials& CredentialsStore::getCredentials(Poco::UInt64 authToken) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	CredentialsMap::const_iterator it = _credentials.find(authToken);
	if (it != _credentials.end())
	{
		return it->second;
	}
	else throw Poco::NotFoundException();
}


Poco::UInt64 CredentialsStore::generateAuthToken()
{
	Poco::UInt64 token = 0;
	do
	{
		Poco::UInt32 r1 = _random.next();
		Poco::UInt32 r2 = _random.next();
		Poco::UInt32 r3 = _random.next();
		token = static_cast<Poco::UInt64>(r1)*static_cast<Poco::UInt64>(r2);
		token ^= static_cast<Poco::UInt64>(r3) << 32;
	}
	while (token == 0 || _credentials.find(token) != _credentials.end());
	return token;
}


} } } // namespace Poco::RemotingNG::TCP


