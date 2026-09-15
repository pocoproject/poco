//
// SCRAMClientAuthenticator.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SCRAMClientAuthenticator
//
// Definition of the SCRAMClientAuthenticator class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_SCRAMClientAuthenticator_INCLUDED
#define RemotingNG_TCP_SCRAMClientAuthenticator_INCLUDED


#include "Poco/RemotingNG/TCP/ClientAuthenticator.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API SCRAMClientAuthenticator: public ClientAuthenticator
	/// This ClientAuthenticator implementation provides the SCRAM-SHA-1
	/// (Salted Challenge Response Authentication Mechanism with SHA-1)
	/// authentication mechanism.
	///
	/// SCRAM specifies the authentication message exchange between the client
	/// and the server.
	/// It uses the PBKDF2 algorithm from the Public-Key Cryptography Standards (PKCS)
	/// and has the following features:
	///
	///   - The password is never transmitted in plain text, only a hash of the
	///     password is transmitted.
	///   - The server does not need to store the passwords in plain text (or
	///     encrypted); salted hashes of the passwords are sufficient.
	///   - The protocol allows for authenticating the client against the server
	///     and also authentication the server against the client.
{
public:
	using Ptr = Poco::AutoPtr<SCRAMClientAuthenticator>;

	static const std::string SCRAM_SHA1;
		/// The name of the mechanism is "SCRAM-SHA-1".

	SCRAMClientAuthenticator();
		/// Creates the SCRAMClientAuthenticator.

	~SCRAMClientAuthenticator();
		/// Destroys the SCRAMClientAuthenticator.

	// ClientAuthenticator
	std::string startAuthentication(Credentials& clientCredentials);
	bool continueAuthentication(const Credentials& serverCredentials, Credentials& clientCredentials);

protected:
	std::string hashCredentials(const Credentials& creds);

	static const std::string DEFAULT_CRED_MD5_SALT;

	enum State
	{
		STATE_INIT,
		STATE_START,
		STATE_CLIENT_AUTH,
	};

private:
	State _state;
	Credentials _creds;
	std::string _nonce;
	std::string _saltedPassword;
	std::string _authMessage;
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_SCRAMClientAuthenticator_INCLUDED
