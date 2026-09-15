//
// ClientAuthenticator.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ClientAuthenticator
//
// Definition of the ClientAuthenticator class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_ClientAuthenticator_INCLUDED
#define RemotingNG_TCP_ClientAuthenticator_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API ClientAuthenticator: public Poco::RefCountedObject
	/// This is the base class for implementations of the client
	/// part of authentication.
{
public:
	using Ptr = Poco::AutoPtr<ClientAuthenticator>;

	virtual std::string startAuthentication(Credentials& clientCredentials) = 0;
		/// Begin the authentication process.
		///
		/// Returns the authentication scheme and updates the
		/// given credentials according to the specific mechanism.
		///
		/// For a simple username/password authentication mechanism, this
		/// completes the authentication process.

	virtual bool continueAuthentication(const Credentials& serverCredentials, Credentials& clientCredentials) = 0;
		/// Continues the authentication flow with the Credentials
		/// received from the server.
		///
		/// This is used for multi-step authentication flows such as SCRAM-SHA-1.
		///
		/// Returns true if the authentication flow should be continued,
		/// otherwise false.
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_ClientAuthenticator_INCLUDED
