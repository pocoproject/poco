//
// PlainClientAuthenticator.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  PlainClientAuthenticator
//
// Definition of the PlainClientAuthenticator class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_PlainClientAuthenticator_INCLUDED
#define RemotingNG_TCP_PlainClientAuthenticator_INCLUDED


#include "Poco/RemotingNG/TCP/ClientAuthenticator.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API PlainClientAuthenticator: public ClientAuthenticator
	/// A very simple ClientAuthenticator for username/password
	/// based plain-text authentication.
	///
	/// Since credentials are transferred in clear text, this
	/// should only be used over an encrypted transport.
{
public:
	using Ptr = Poco::AutoPtr<PlainClientAuthenticator>;

	static const std::string PLAIN;
		/// The name of the mechanism is "PLAIN".

	PlainClientAuthenticator();
		/// Creates the PlainClientAuthenticator.

	~PlainClientAuthenticator();
		/// Destroys the PlainClientAuthenticator.

	// ClientAuthenticator
	std::string startAuthentication(Credentials& clientCredentials);
	bool continueAuthentication(const Credentials& serverCredentials, Credentials& clientCredentials);
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_PlainClientAuthenticator_INCLUDED
