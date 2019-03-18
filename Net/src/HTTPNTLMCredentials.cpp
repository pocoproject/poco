//
// HTTPNTLMCredentials.cpp
//
// Library: Net
// Package: HTTP
// Module:	HTTPNTLMCredentials
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPNTLMCredentials.h"
#include "Poco/Net/NTLMCredentials.h"
#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/DateTime.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/MemoryStream.h"
#include <sstream>


namespace Poco {
namespace Net {


const std::string HTTPNTLMCredentials::SCHEME = "NTLM";


HTTPNTLMCredentials::HTTPNTLMCredentials()
{
}


HTTPNTLMCredentials::HTTPNTLMCredentials(const std::string& username, const std::string& password):
	_username(username),
	_password(password)
{
}


HTTPNTLMCredentials::~HTTPNTLMCredentials()
{
}


void HTTPNTLMCredentials::reset()
{
}


void HTTPNTLMCredentials::setUsername(const std::string& username)
{
	_username = username;
}


void HTTPNTLMCredentials::setPassword(const std::string& password)
{
	_password = password;
}


void HTTPNTLMCredentials::authenticate(HTTPRequest& request, const HTTPResponse& response)
{
	HTTPAuthenticationParams params(response);
	authenticate(request, params.get(HTTPAuthenticationParams::NTLM, ""));
}


void HTTPNTLMCredentials::authenticate(HTTPRequest& request, const std::string& ntlmChallengeBase64)
{
	std::string ntlmMessage = createNTLMMessage(ntlmChallengeBase64);
	request.setCredentials(SCHEME, ntlmMessage);
}


void HTTPNTLMCredentials::updateAuthInfo(HTTPRequest& request)
{
	request.removeCredentials();
}


void HTTPNTLMCredentials::proxyAuthenticate(HTTPRequest& request, const HTTPResponse& response)
{
	HTTPAuthenticationParams params(response, HTTPAuthenticationParams::PROXY_AUTHENTICATE);
	proxyAuthenticate(request, params.get(HTTPAuthenticationParams::NTLM, ""));
}


void HTTPNTLMCredentials::proxyAuthenticate(HTTPRequest& request, const std::string& ntlmChallengeBase64)
{
	std::string ntlmMessage = createNTLMMessage(ntlmChallengeBase64);
	request.setProxyCredentials(SCHEME, ntlmMessage);
}


void HTTPNTLMCredentials::updateProxyAuthInfo(HTTPRequest& request)
{
	request.removeProxyCredentials();
}


std::string HTTPNTLMCredentials::createNTLMMessage(const std::string& responseAuthParams)
{
	if (responseAuthParams.empty())
	{
		NTLMCredentials::NegotiateMessage negotiateMsg;
		std::string username;
		splitUsername(_username, username, negotiateMsg.domain);
		std::vector<unsigned char> negotiateBuf = NTLMCredentials::formatNegotiateMessage(negotiateMsg);

		std::ostringstream ostr;
		Poco::Base64Encoder base64(ostr);
		base64.rdbuf()->setLineLength(0);
		base64.write(reinterpret_cast<const char*>(&negotiateBuf[0]), negotiateBuf.size());
		base64.close();
		return ostr.str();
	}
	else
	{
		Poco::MemoryInputStream istr(responseAuthParams.data(), responseAuthParams.size());
		Poco::Base64Decoder debase64(istr);
		std::vector<unsigned char> buffer(responseAuthParams.size());
		debase64.read(reinterpret_cast<char*>(&buffer[0]), buffer.size());
		std::size_t size = debase64.gcount();

		Poco::Net::NTLMCredentials::ChallengeMessage challengeMsg;
		if (NTLMCredentials::parseChallengeMessage(&buffer[0], size, challengeMsg))
		{
			std::string username;
			std::string domain;
			splitUsername(_username, username, domain);

			NTLMCredentials::AuthenticateMessage authenticateMsg;
			authenticateMsg.flags = challengeMsg.flags;
			authenticateMsg.target = challengeMsg.target;
			authenticateMsg.username = username;

			std::vector<unsigned char> nonce = NTLMCredentials::createNonce();
			Poco::UInt64 timestamp = NTLMCredentials::createTimestamp();
			std::vector<unsigned char> ntlm2Hash = NTLMCredentials::createNTLMv2Hash(username, challengeMsg.target, _password);
			authenticateMsg.lmResponse = NTLMCredentials::createLMv2Response(ntlm2Hash, challengeMsg.challenge, nonce);
			authenticateMsg.ntlmResponse = Poco::Net::NTLMCredentials::createNTLMv2Response(ntlm2Hash, challengeMsg.challenge, nonce, challengeMsg.targetInfo, timestamp);

			std::vector<unsigned char> authenticateBuf = Poco::Net::NTLMCredentials::formatAuthenticateMessage(authenticateMsg);

			std::ostringstream ostr;
			Poco::Base64Encoder base64(ostr);
			base64.rdbuf()->setLineLength(0);
			base64.write(reinterpret_cast<const char*>(&authenticateBuf[0]), authenticateBuf.size());
			base64.close();
			return ostr.str();
		}
		else throw Poco::InvalidArgumentException("Invalid NTLM challenge");
	}
}


void HTTPNTLMCredentials::splitUsername(const std::string& usernameAndDomain, std::string& username, std::string& domain)
{
	std::string::size_type pos = usernameAndDomain.find('\\');
	if (pos != std::string::npos)
	{
		domain.assign(usernameAndDomain, 0, pos);
		username.assign(usernameAndDomain, pos + 1);
		return;
	}
	else
	{
		pos = usernameAndDomain.find('@');
		if (pos != std::string::npos)
		{
			username.assign(usernameAndDomain, 0, pos);
			domain.assign(usernameAndDomain, pos + 1);
			return;
		}
	}
	username = usernameAndDomain;
}



} } // namespace Poco::Net
