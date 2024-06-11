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
#include "Poco/String.h"


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
	clear();
}


void HTTPNTLMCredentials::reset()
{
}


void HTTPNTLMCredentials::clear()
{
	Poco::secureClear(_username);
	Poco::secureClear(_password);
	_host.clear();
}


void HTTPNTLMCredentials::setUsername(const std::string& username)
{
	_username = username;
}


void HTTPNTLMCredentials::setPassword(const std::string& password)
{
	_password = password;
}


void HTTPNTLMCredentials::setHost(const std::string& host)
{
	_host = host;
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
		std::vector<unsigned char> negotiateBuf;
		if (useSSPINTLM())
		{
			_pNTLMContext = SSPINTLMCredentials::createNTLMContext(_host, SSPINTLMCredentials::SERVICE_HTTP);
			negotiateBuf = SSPINTLMCredentials::negotiate(*_pNTLMContext);
		}
		else
		{
			NTLMCredentials::NegotiateMessage negotiateMsg;
			std::string username;
			NTLMCredentials::splitUsername(_username, username, negotiateMsg.domain);
			negotiateBuf = NTLMCredentials::formatNegotiateMessage(negotiateMsg);
		}
		return NTLMCredentials::toBase64(negotiateBuf);
	}
	else
	{
		std::vector<unsigned char> buffer = NTLMCredentials::fromBase64(responseAuthParams);
		if (buffer.empty()) throw HTTPException("Invalid NTLM challenge");
		std::vector<unsigned char> authenticateBuf;
		if (useSSPINTLM() && _pNTLMContext)
		{
			authenticateBuf = SSPINTLMCredentials::authenticate(*_pNTLMContext, buffer);
		}
		else
		{
			NTLMCredentials::ChallengeMessage challengeMsg;
			if (NTLMCredentials::parseChallengeMessage(&buffer[0], buffer.size(), challengeMsg))
			{
				if ((challengeMsg.flags & NTLMCredentials::NTLM_FLAG_NEGOTIATE_NTLM2_KEY) == 0)
				{
					throw HTTPException("Proxy does not support NTLMv2 authentication");
				}

				std::string username;
				std::string domain;
				NTLMCredentials::splitUsername(_username, username, domain);

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

				authenticateBuf = NTLMCredentials::formatAuthenticateMessage(authenticateMsg);
			}
			else throw HTTPException("Invalid NTLM challenge");
		}
		return NTLMCredentials::toBase64(authenticateBuf);
	}
}


} } // namespace Poco::Net
