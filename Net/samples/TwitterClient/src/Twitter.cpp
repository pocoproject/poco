//
// Twitter.cpp
//
// $Id: //poco/1.4/Net/samples/TwitterClient/src/Twitter.cpp#2 $
//
// A C++ implementation of a Twitter client based on the POCO Net library.
//
// Copyright (c) 2009-2013, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Twitter.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/URI.h"
#include "Poco/SHA1Engine.h"
#include "Poco/HMACEngine.h"
#include "Poco/Base64Encoder.h"
#include "Poco/RandomStream.h"
#include "Poco/Timestamp.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include <sstream>
#include <map>


const std::string Twitter::TWITTER_URI("http://api.twitter.com/1.1/statuses/");


Twitter::Twitter():
	_uri(TWITTER_URI)
{
}

	
Twitter::Twitter(const std::string& twitterURI):
	_uri(twitterURI)
{
}

	
Twitter::~Twitter()
{
}


void Twitter::login(const std::string& consumerKey, const std::string& consumerSecret, const std::string& token, const std::string& tokenSecret)
{
	_consumerKey    = consumerKey;
	_consumerSecret = consumerSecret;
	_token          = token;
	_tokenSecret    = tokenSecret;
}

	
Poco::Int64 Twitter::update(const std::string& status)
{
	Poco::Net::HTMLForm form;
	form.set("status", status);
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> pResult = invoke(Poco::Net::HTTPRequest::HTTP_POST, "update", form);
	return pResult->getInt64("id", 0);
}


Poco::AutoPtr<Poco::Util::AbstractConfiguration> Twitter::invoke(const std::string& httpMethod, const std::string& twitterMethod, Poco::Net::HTMLForm& form)
{
	// Create the request URI.
	// We use the JSON version of the Twitter API.
	Poco::URI uri(_uri + twitterMethod + ".json");
	
	Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
	Poco::Net::HTTPRequest req(httpMethod, uri.getPath(), Poco::Net::HTTPMessage::HTTP_1_1);
	
	// Sign request
	sign(req, form, uri.toString());
	
	// Send the request.
	form.prepareSubmit(req);
	std::ostream& ostr = session.sendRequest(req);
	form.write(ostr);
	
	// Receive the response.
	Poco::Net::HTTPResponse res;
	std::istream& rs = session.receiveResponse(res);
	
	Poco::AutoPtr<Poco::Util::JSONConfiguration> pResult = new Poco::Util::JSONConfiguration(rs);

	// If everything went fine, return the JSON document.
	// Otherwise throw an exception.
	if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
	{
		return pResult;
	}
	else
	{
		throw Poco::ApplicationException("Twitter Error", pResult->getString("errors[0].message", ""));
	}
}


void Twitter::sign(Poco::Net::HTTPRequest& request, const Poco::Net::HTMLForm& params, const std::string& uri) const
{
	std::string nonce(createNonce());
	std::string timestamp(Poco::NumberFormatter::format(Poco::Timestamp().epochTime()));
	std::string signature(createSignature(request, params, uri, nonce, timestamp));
	std::string authorization(
		Poco::format(
			"OAuth"
			" oauth_consumer_key=\"%s\","
			" oauth_nonce=\"%s\","
			" oauth_signature=\"%s\","
			" oauth_signature_method=\"HMAC-SHA1\","
			" oauth_timestamp=\"%s\","
			" oauth_token=\"%s\","
			" oauth_version=\"1.0\"",
			percentEncode(_consumerKey),
			percentEncode(nonce),
			percentEncode(signature),
			timestamp,
			percentEncode(_token)
		)
	);
	request.set("Authorization", authorization);
}


std::string Twitter::createNonce() const
{
	std::ostringstream base64Nonce;
	Poco::Base64Encoder base64Encoder(base64Nonce);
	Poco::RandomInputStream randomStream;
	for (int i = 0; i < 32; i++)
	{
		base64Encoder.put(randomStream.get());
	}
	base64Encoder.close();
	std::string nonce = base64Nonce.str();
	return Poco::translate(nonce, "+/=", "");
}


std::string Twitter::createSignature(Poco::Net::HTTPRequest& request, const Poco::Net::HTMLForm& params, const std::string& uri, const std::string& nonce, const std::string& timestamp) const
{
	std::map<std::string, std::string> paramsMap;
	paramsMap["oauth_consumer_key"]     = percentEncode(_consumerKey);
	paramsMap["oauth_nonce"]            = percentEncode(nonce);
	paramsMap["oauth_signature_method"] = "HMAC-SHA1";
	paramsMap["oauth_timestamp"]        = timestamp;
	paramsMap["oauth_token"]            = percentEncode(_token);
	paramsMap["oauth_version"]          = "1.0";
	for (Poco::Net::HTMLForm::ConstIterator it = params.begin(); it != params.end(); ++it)
	{
		paramsMap[percentEncode(it->first)] = percentEncode(it->second);
	}
	
	std::string paramsString;
	for (std::map<std::string, std::string>::const_iterator it = paramsMap.begin(); it != paramsMap.end(); ++it)
	{
		if (it != paramsMap.begin()) paramsString += '&';
		paramsString += it->first;
		paramsString += "=";
		paramsString += it->second;
	}
	
	std::string signatureBase = request.getMethod();
	signatureBase += '&';
	signatureBase += percentEncode(uri);
	signatureBase += '&';
	signatureBase += percentEncode(paramsString);
	
	std::string signingKey;
	signingKey += percentEncode(_consumerSecret);
	signingKey += '&';
	signingKey += percentEncode(_tokenSecret);
	
	Poco::HMACEngine<Poco::SHA1Engine> hmacEngine(signingKey);
	hmacEngine.update(signatureBase);
	Poco::DigestEngine::Digest digest = hmacEngine.digest();
	std::ostringstream digestBase64;
	Poco::Base64Encoder base64Encoder(digestBase64);
	base64Encoder.write(reinterpret_cast<char*>(&digest[0]), digest.size());
	base64Encoder.close();
	return digestBase64.str();
}


std::string Twitter::percentEncode(const std::string& str)
{
	std::string encoded;
	Poco::URI::encode(str, "!?#/'\",;:$&()[]*+=@", encoded);
	return encoded;
}
