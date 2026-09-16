//
// SCRAMAuthenticator.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SCRAMAuthenticator
//
// Definition of the SCRAMAuthenticator class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_SCRAMAuthenticator_INCLUDED
#define RemotingNG_TCP_SCRAMAuthenticator_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/Authenticator.h"
#include "Poco/DigestEngine.h"
#include "Poco/ExpireCache.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API SCRAMAuthenticator: public Authenticator
	/// This abstract Authenticator implementation provides the SCRAM-SHA-1
	/// (Salted Challenge Response Authentication Mechanism with SHA-1)
	/// authentication mechanism.
	///
	/// SCRAM specifies the authentication message exchange between the client
	/// and the server.
	/// It uses the PBKDF2 algorithm from the Public-Key Cryptography Standards (PKCS)
	/// and has the following features:
	///
	///   - The password is never transmitted in plain text, only a hash of the
	///     password is transmitted from client to server.
	///   - The server does not need to store the passwords in plain text (or
	///     encrypted); salted hashes of the passwords are sufficient.
	///   - The protocol allows for authenticating the client against the server
	///     and also authenticating the server against the client.
	///
	/// Since the Remoting framework does not provide user account and password
	/// storage, this class must be subclassed and the following methods
	/// must be implemented:
	///
	///   - saltForUser()
	///   - hashForUser()
{
public:
	using Ptr = Poco::AutoPtr<SCRAMAuthenticator>;

	SCRAMAuthenticator();
		/// Creates the SCRAMAuthenticator.

	~SCRAMAuthenticator();
		/// Destroys the SCRAMAuthenticator.

	// Authenticator
	AuthenticateResult authenticate(const Credentials& credentials, Poco::UInt32 conversationID);

	// Utility methods
	static std::string createNonce();
	static std::string digestToBinaryString(Poco::DigestEngine& engine);
	static std::string digestToHexString(Poco::DigestEngine& engine);
	static std::string computeClientProof(const std::string& username, const std::string& clientNonce, const std::string& salt, int iterations, const std::string& saltedPassword, const std::string& serverNonce, std::string& clientAuthMessage);
	static std::string computeServerSignature(const std::string& saltedPassword, const std::string& clientAuthMessage);

protected:
	virtual std::string saltForUser(const std::string& username, int& iterations) = 0;
		/// Returns the salt for hashing the given user's password, as well as the
		/// number of iterations for the PBKDF2 algorithm.
		///
		/// Returns an empty string if the user does not exist.

	virtual std::string hashForUser(const std::string& username) = 0;
		/// Returns the PBKDF2-hashed password for the given user.
		///
		/// Note that the returned string must contain the raw bytes of
		/// the hash, not the hex-encoded or base64-encoded hash.

	Poco::UInt32 newConversationID();
		/// Generates a new Conversation ID.

	enum State
	{
		STATE_INIT,
		STATE_START,
		STATE_CLIENT_AUTH
	};

	struct Conversation
	{
		using Ptr = Poco::SharedPtr<Conversation>;

		Conversation():
			state(STATE_INIT),
			iterations(0)
		{
		}

		State state;
		std::string username;
		std::string clientNonce;
		std::string serverNonce;
		std::string salt;
		int iterations;
	};

	using ConversationCache = Poco::ExpireCache<Poco::UInt32, Conversation>;

private:
	ConversationCache _conversations;
	Poco::UInt32 _nextConversationID;
	Poco::FastMutex _mutex;
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_SCRAMAuthenticator_INCLUDED
