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
#include "Poco/Net/NetException.h"
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
		return toBase64(negotiateBuf);
	}
	else
	{
		std::vector<unsigned char> buffer = fromBase64(responseAuthParams);
		NTLMCredentials::ChallengeMessage challengeMsg;
		if (NTLMCredentials::parseChallengeMessage(&buffer[0], buffer.size(), challengeMsg))
		{
			if ((challengeMsg.flags & NTLMCredentials::NTLM_FLAG_NEGOTIATE_NTLM2_KEY) == 0)
			{
				throw HTTPException("Proxy does not support NTLMv2 authentication");
			}

			std::string username;
			std::string domain;
			splitUsername(_username, username, domain);

			NTLMCredentials::AuthenticateMessage authenticateMsg;
			authenticateMsg.flags = challengeMsg.flags;
			authenticateMsg.target = challengeMsg.target;
			authenticateMsg.username = username;

			std::vector<unsigned char> lmNonce = NTLMCredentials::createNonce();
			std::vector<unsigned char> ntlmNonce = NTLMCredentials::createNonce();
			Poco::UInt64 timestamp = NTLMCredentials::createTimestamp();
			std::vector<unsigned char> ntlm2Hash = NTLMCredentials::createNTLMv2Hash(username, challengeMsg.target, _password);

			authenticateMsg.lmResponse = NTLMCredentials::createLMv2Response(ntlm2Hash, challengeMsg.challenge, lmNonce);
			authenticateMsg.ntlmResponse = NTLMCredentials::createNTLMv2Response(ntlm2Hash, challengeMsg.challenge, ntlmNonce, challengeMsg.targetInfo, timestamp);

			std::vector<unsigned char> authenticateBuf = NTLMCredentials::formatAuthenticateMessage(authenticateMsg);
			return toBase64(authenticateBuf);
		}
		else throw HTTPException("Invalid NTLM challenge");
	}
}


void HTTPNTLMCredentials::splitUsername(const std::string& usernameAndDomain, std::string& username, std::string& domain)
{
	std::string::size_type pos = usernameAndDomain.find('\\');
	if (pos != std::string::npos)
	{
		domain.assign(usernameAndDomain, 0, pos);
		username.assign(usernameAndDomain, pos + 1, std::string::npos);
		return;
	}
	else
	{
		pos = usernameAndDomain.find('@');
		if (pos != std::string::npos)
		{
			username.assign(usernameAndDomain, 0, pos);
			domain.assign(usernameAndDomain, pos + 1, std::string::npos);
			return;
		}
	}
	username = usernameAndDomain;
}


std::string HTTPNTLMCredentials::toBase64(const std::vector<unsigned char>& buffer)
{
	std::ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	base64.rdbuf()->setLineLength(0);
	base64.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
	base64.close();
	return ostr.str();
}


std::vector<unsigned char> HTTPNTLMCredentials::fromBase64(const std::string& base64)
{
	Poco::MemoryInputStream istr(base64.data(), base64.size());
	Poco::Base64Decoder debase64(istr);
	std::vector<unsigned char> buffer(base64.size());
	debase64.read(reinterpret_cast<char*>(&buffer[0]), buffer.size());
	buffer.resize(static_cast<std::size_t>(debase64.gcount()));
	return buffer;
}


} } // namespace Poco::Net
