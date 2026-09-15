//
// SCRAMClientAuthenticator.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  SCRAMClientAuthenticator
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/SCRAMClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/SCRAMAuthenticator.h"
#include "Poco/SHA1Engine.h"
#include "Poco/MD5Engine.h"
#include "Poco/PBKDF2Engine.h"
#include "Poco/HMACEngine.h"
#include "Poco/NumberParser.h"
#include "Poco/Format.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


const std::string SCRAMClientAuthenticator::SCRAM_SHA1("SCRAM-SHA-1");
const std::string SCRAMClientAuthenticator::DEFAULT_CRED_MD5_SALT("poco");


SCRAMClientAuthenticator::SCRAMClientAuthenticator():
	_state(STATE_INIT)
{
}

	
SCRAMClientAuthenticator::~SCRAMClientAuthenticator()
{
}


std::string SCRAMClientAuthenticator::startAuthentication(Credentials& creds)
{
	poco_assert (_state == STATE_INIT);

	_creds = creds;
	_nonce = SCRAMAuthenticator::createNonce();

	Credentials initialCreds;
	initialCreds.setAttribute(Credentials::ATTR_USERNAME, creds.getAttribute(Credentials::ATTR_USERNAME));
	initialCreds.setAttribute(Credentials::ATTR_NONCE, _nonce);
	
	creds = initialCreds;
	_state = STATE_START;

	return SCRAM_SHA1;
}


bool SCRAMClientAuthenticator::continueAuthentication(const Credentials& serverCreds, Credentials& creds)
{
	if (_state == STATE_START)
	{
		const std::string serverNonce = serverCreds.getAttribute(Credentials::ATTR_NONCE);
		const std::string salt = serverCreds.getAttribute(Credentials::ATTR_SALT);
		const int iterations = Poco::NumberParser::parse(serverCreds.getAttribute(Credentials::ATTR_ITERATIONS));
		const Poco::UInt32 dkLen = 20;
		
		const std::string username = _creds.getAttribute(Credentials::ATTR_USERNAME);
		const std::string hashedPassword = hashCredentials(_creds);
		const std::string clientFirstMsg = Poco::format("n=%s,r=%s", username, _nonce);

		Poco::PBKDF2Engine<Poco::HMACEngine<Poco::SHA1Engine> > pbkdf2(salt, iterations, dkLen);
		pbkdf2.update(hashedPassword);
		_saltedPassword = SCRAMAuthenticator::digestToBinaryString(pbkdf2);

		const std::string clientProof = SCRAMAuthenticator::computeClientProof(username, _nonce, salt, iterations, _saltedPassword, serverNonce, _authMessage);
		
		creds.setAttribute(Credentials::ATTR_NONCE, serverNonce);
		creds.setAttribute(Credentials::ATTR_PASSWORD, clientProof);
		
		_state = STATE_CLIENT_AUTH;
		return true;
	}
	else if (_state == STATE_CLIENT_AUTH)
	{
		const std::string serverSignatureReceived = serverCreds.getAttribute(Credentials::ATTR_SIGNATURE);
		const std::string serverSignatureComputed = SCRAMAuthenticator::computeServerSignature(_saltedPassword, _authMessage);
		
		_creds.clearAttributes();
		_nonce.clear();
		_saltedPassword.clear();
		_authMessage.clear();

		_state = STATE_INIT;
		
		return serverSignatureComputed == serverSignatureReceived;
	}	
	return false;
}


std::string SCRAMClientAuthenticator::hashCredentials(const Credentials& creds)
{
	Poco::MD5Engine md5;
	md5.update(creds.getAttribute(Credentials::ATTR_USERNAME));
	md5.update(':');
	md5.update(creds.getAttribute(Credentials::ATTR_SALT, DEFAULT_CRED_MD5_SALT));
	md5.update(':');
	md5.update(creds.getAttribute(Credentials::ATTR_PASSWORD));
	return SCRAMAuthenticator::digestToHexString(md5);
}


} } } // namespace Poco::RemotingNG::TCP
