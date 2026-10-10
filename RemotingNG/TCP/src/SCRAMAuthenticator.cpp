//
// SCRAMAuthenticator.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SCRAMAuthenticator
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/SCRAMAuthenticator.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/PBKDF2Engine.h"
#include "Poco/HMACEngine.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/MemoryStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/RandomStream.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace TCP {


SCRAMAuthenticator::SCRAMAuthenticator():
	_nextConversationID(0)
{
}


SCRAMAuthenticator::~SCRAMAuthenticator()
{
}


AuthenticateResult SCRAMAuthenticator::authenticate(const Credentials& creds, Poco::UInt32 conversationID)
{
	if (conversationID == 0)
	{
		conversationID = newConversationID();

		Conversation::Ptr pConv = new Conversation;
		pConv->username = creds.getAttribute(Credentials::ATTR_USERNAME);
		pConv->clientNonce = creds.getAttribute(Credentials::ATTR_NONCE);
		pConv->serverNonce = createNonce();
		pConv->salt = saltForUser(pConv->username, pConv->iterations);

		if (!pConv->salt.empty())
		{
			Credentials serverCreds;
			serverCreds.setAttribute(Credentials::ATTR_NONCE, pConv->serverNonce);
			serverCreds.setAttribute(Credentials::ATTR_SALT, pConv->salt);
			serverCreds.setAttribute(Credentials::ATTR_ITERATIONS, Poco::NumberFormatter::format(pConv->iterations));

			_conversations.add(conversationID, pConv);
			pConv->state = STATE_START;

			return AuthenticateResult(AuthenticateResult::AUTH_CONTINUE, serverCreds, conversationID);
		}
	}
	else
	{
		Conversation::Ptr pConv = _conversations.get(conversationID);
		if (pConv)
		{
			if (pConv->state == STATE_START)
			{
				const std::string receivedServerNonce = creds.getAttribute(Credentials::ATTR_NONCE);
				const std::string receivedClientProof = creds.getAttribute(Credentials::ATTR_PASSWORD);

				const std::string saltedPassword = hashForUser(pConv->username);
				std::string clientAuthMessage;
				const std::string computedClientProof = computeClientProof(pConv->username, pConv->clientNonce, pConv->salt, pConv->iterations, saltedPassword, pConv->serverNonce, clientAuthMessage);

				if (computedClientProof == receivedClientProof && pConv->serverNonce == receivedServerNonce)
				{
					std::string serverSignature = computeServerSignature(saltedPassword, clientAuthMessage);

					Credentials serverCreds;
					serverCreds.setAttribute(Credentials::ATTR_SIGNATURE, serverSignature);

					pConv->state = STATE_CLIENT_AUTH;
					return AuthenticateResult(AuthenticateResult::AUTH_CONTINUE, serverCreds, conversationID);
				}
				else
				{
					_conversations.remove(conversationID);
				}
			}
			else if (pConv->state == STATE_CLIENT_AUTH)
			{
				_conversations.remove(conversationID);
				Credentials serverCreds;
				serverCreds.setAttribute(Credentials::ATTR_USERNAME, pConv->username);
				return AuthenticateResult(AuthenticateResult::AUTH_DONE, serverCreds, conversationID);
			}
		}
	}
	return AuthenticateResult();
}


Poco::UInt32 SCRAMAuthenticator::newConversationID()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Poco::UInt32 result = ++_nextConversationID;
	return result;
}


std::string SCRAMAuthenticator::digestToBinaryString(Poco::DigestEngine& engine)
{
	Poco::DigestEngine::Digest d = engine.digest();
	return std::string(reinterpret_cast<const char*>(&d[0]), d.size());
}


std::string SCRAMAuthenticator::digestToHexString(Poco::DigestEngine& engine)
{
	Poco::DigestEngine::Digest d = engine.digest();
	return Poco::DigestEngine::digestToHex(d);
}


std::string SCRAMAuthenticator::createNonce()
{
	Poco::MD5Engine md5;
	Poco::RandomInputStream randomStream;
	for (int i = 0; i < 8; i++)
	{
		md5.update(randomStream.get());
	}
	return digestToHexString(md5);
}


std::string SCRAMAuthenticator::computeClientProof(const std::string& username, const std::string& clientNonce, const std::string& salt, int iterations, const std::string& saltedPassword, const std::string& serverNonce, std::string& clientAuthMessage)
{
	const std::string clientFirstMessage = Poco::format("n=%s,r=%s"s, username, clientNonce);
	const std::string clientFinalNoProof = Poco::format("c=biws,r=%s"s, serverNonce);
	const std::string serverFirstMessage = Poco::format("r=%s,s=%s,i=%d"s, serverNonce, salt, iterations);
	clientAuthMessage = Poco::format("%s,%s,%s"s, clientFirstMessage, serverFirstMessage, clientFinalNoProof);

	Poco::HMACEngine<Poco::SHA1Engine> hmacKey(saltedPassword);
	hmacKey.update("Client Key", 10);
	const std::string clientKey = SCRAMAuthenticator::digestToBinaryString(hmacKey);

	Poco::SHA1Engine sha1;
	sha1.update(clientKey);
	const std::string storedKey = SCRAMAuthenticator::digestToBinaryString(sha1);

	Poco::HMACEngine<Poco::SHA1Engine> hmacSig(storedKey);
	hmacSig.update(clientAuthMessage);
	const std::string clientSignature = SCRAMAuthenticator::digestToBinaryString(hmacSig);

	std::string clientProof(clientKey);
	for (std::size_t i = 0; i < clientProof.size(); i++)
	{
		clientProof[i] ^= clientSignature[i];
	}

	return clientProof;
}


std::string SCRAMAuthenticator::computeServerSignature(const std::string& saltedPassword, const std::string& clientAuthMessage)
{
	Poco::HMACEngine<Poco::SHA1Engine> hmacSKey(saltedPassword);
	hmacSKey.update("Server Key", 10);
	std::string serverKey = SCRAMAuthenticator::digestToBinaryString(hmacSKey);

	Poco::HMACEngine<Poco::SHA1Engine> hmacSSig(serverKey);
	hmacSSig.update(clientAuthMessage);
	std::string serverSignature = SCRAMAuthenticator::digestToBinaryString(hmacSSig);

	return serverSignature;
}


} } } // namespace Poco::RemotingNG::TCP
