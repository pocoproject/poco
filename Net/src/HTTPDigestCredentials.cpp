//
// HTTPDigestCredentials.cpp
//
// Library: Net
// Package: HTTP
// Module:	HTTPDigestCredentials
//
// Copyright (c) 2011, Anton V. Yabchinskiy (arn at bestmx dot ru).
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Exception.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/SHA2Engine.h"
#include "Poco/Net/HTTPDigestCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"


namespace
{
	std::string digest(Poco::DigestEngine& engine,
					   const std::string& a,
					   const std::string& b,
					   const std::string& c = std::string(),
					   const std::string& d = std::string(),
					   const std::string& e = std::string(),
					   const std::string& f = std::string())
	{
		engine.reset();
		engine.update(a);
		engine.update(':');
		engine.update(b);
		if (!c.empty())
		{
			engine.update(':');
			engine.update(c);
			if (!d.empty())
			{
				engine.update(':');
				engine.update(d);
				engine.update(':');
				engine.update(e);
				engine.update(':');
				engine.update(f);
			}
		}
		return Poco::DigestEngine::digestToHex(engine.digest());
	}

	std::string formatNonceCounter(int counter)
	{
		return Poco::NumberFormatter::formatHex(counter, 8);
	}
}


namespace Poco {
namespace Net {


const std::string HTTPDigestCredentials::SCHEME = "Digest";
const std::vector<std::string> HTTPDigestCredentials::SUPPORTED_ALGORITHMS = {
	"MD5",
	"MD5-sess",
	"SHA",
	"SHA-sess",
	"SHA-256",
	"SHA-256-sess",
	"SHA-512-256",
	"SHA-512-256-sess",
	"SHA-512",
	"SHA-512-sess"
};
const std::string HTTPDigestCredentials::MD_5_ALGORITHM = "MD5";
const std::string HTTPDigestCredentials::MD_5_SESS_ALGORITHM = "MD5-sess";
const std::string HTTPDigestCredentials::SHA_ALGORITHM = "SHA";
const std::string HTTPDigestCredentials::SHA_SESS_ALGORITHM = "SHA-sess";
const std::string HTTPDigestCredentials::SHA_256_ALGORITHM = "SHA-256";
const std::string HTTPDigestCredentials::SHA_256_SESS_ALGORITHM = "SHA-256-sess";
const std::string HTTPDigestCredentials::SHA_512_256_ALGORITHM = "SHA-512-256";
const std::string HTTPDigestCredentials::SHA_512_256_SESS_ALGORITHM = "SHA-512-256-sess";
const std::string HTTPDigestCredentials::SHA_512_ALGORITHM = "SHA-512";
const std::string HTTPDigestCredentials::SHA_512_SESS_ALGORITHM = "SHA-512-sess";
const std::string HTTPDigestCredentials::DEFAULT_QOP("");
const std::string HTTPDigestCredentials::NONCE_PARAM("nonce");
const std::string HTTPDigestCredentials::REALM_PARAM("realm");
const std::string HTTPDigestCredentials::QOP_PARAM("qop");
const std::string HTTPDigestCredentials::ALGORITHM_PARAM("algorithm");
const std::string HTTPDigestCredentials::USERNAME_PARAM("username");
const std::string HTTPDigestCredentials::OPAQUE_PARAM("opaque");
const std::string HTTPDigestCredentials::URI_PARAM("uri");
const std::string HTTPDigestCredentials::RESPONSE_PARAM("response");
const std::string HTTPDigestCredentials::AUTH_PARAM("auth");
const std::string HTTPDigestCredentials::CNONCE_PARAM("cnonce");
const std::string HTTPDigestCredentials::NC_PARAM("nc");
int HTTPDigestCredentials::_nonceCounter(0);
Poco::FastMutex HTTPDigestCredentials::_nonceMutex;


class HTTPDigestCredentials::DigestEngineProvider 
{
public:
	DigestEngineProvider(std::string algorithm): _algorithm(algorithm) 
	{
		_isSessionAlgorithm = _algorithm.find("sess") != std::string::npos;
	}

	DigestEngine& engine() 
	{
		if (icompare(_algorithm, SHA_ALGORITHM) == 0 || icompare(_algorithm, SHA_SESS_ALGORITHM) == 0)
		{
			return _sha1Engine;
		}
		if (icompare(_algorithm, SHA_256_ALGORITHM) == 0 || icompare(_algorithm, SHA_256_SESS_ALGORITHM) == 0)
		{
			return _sha256Engine;
		} 
		else if (icompare(_algorithm, SHA_512_256_ALGORITHM) == 0 || icompare(_algorithm, SHA_512_256_SESS_ALGORITHM) == 0)
		{
			return _sha512_256Engine;
		} 
		else if (icompare(_algorithm, SHA_512_ALGORITHM) == 0 || icompare(_algorithm, SHA_512_SESS_ALGORITHM) == 0)
		{
			return _sha512;
		}
		else 
		{
			return _md5Engine;
		}
	}

	bool isSessionAlgorithm() 
	{
		return _isSessionAlgorithm;
	}

private:
	std::string _algorithm;
	SHA1Engine _sha1Engine;
	MD5Engine _md5Engine;
	SHA2Engine _sha256Engine { SHA2Engine::ALGORITHM::SHA_256 };
	SHA2Engine _sha512_256Engine { SHA2Engine::ALGORITHM::SHA_512_256 };
	SHA2Engine _sha512 { SHA2Engine::ALGORITHM::SHA_512 };
	bool _isSessionAlgorithm;
};


HTTPDigestCredentials::HTTPDigestCredentials()
{
}


HTTPDigestCredentials::HTTPDigestCredentials(const std::string& username, const std::string& password):
	_username(username),
	_password(password)
{
}


HTTPDigestCredentials::~HTTPDigestCredentials()
{
	clear();
}


void HTTPDigestCredentials::reset()
{
	_requestAuthParams.clear();
	_nc.clear();
}


void HTTPDigestCredentials::setUsername(const std::string& username)
{
	_username = username;
}


void HTTPDigestCredentials::setPassword(const std::string& password)
{
	_password = password;
}


void HTTPDigestCredentials::clear()
{
	Poco::secureClear(_username);
	Poco::secureClear(_password);
}


void HTTPDigestCredentials::authenticate(HTTPRequest& request, const HTTPResponse& response)
{
	authenticate(request, HTTPAuthenticationParams(response));
}


void HTTPDigestCredentials::authenticate(HTTPRequest& request, const HTTPAuthenticationParams& responseAuthParams)
{
	createAuthParams(request, responseAuthParams);
	request.setCredentials(SCHEME, _requestAuthParams.toString());
}


void HTTPDigestCredentials::updateAuthInfo(HTTPRequest& request)
{
	updateAuthParams(request);
	request.setCredentials(SCHEME, _requestAuthParams.toString());
}


void HTTPDigestCredentials::proxyAuthenticate(HTTPRequest& request, const HTTPResponse& response)
{
	proxyAuthenticate(request, HTTPAuthenticationParams(response, HTTPAuthenticationParams::PROXY_AUTHENTICATE));
}


void HTTPDigestCredentials::proxyAuthenticate(HTTPRequest& request, const HTTPAuthenticationParams& responseAuthParams)
{
	createAuthParams(request, responseAuthParams);
	request.setProxyCredentials(SCHEME, _requestAuthParams.toString());
}


void HTTPDigestCredentials::updateProxyAuthInfo(HTTPRequest& request)
{
	updateAuthParams(request);
	request.setProxyCredentials(SCHEME, _requestAuthParams.toString());
}


std::string HTTPDigestCredentials::createNonce()
{
	Poco::FastMutex::ScopedLock lock(_nonceMutex);

	MD5Engine md5;
	Timestamp::TimeVal now = Timestamp().epochMicroseconds();

	md5.update(&_nonceCounter, sizeof(_nonceCounter));
	md5.update(&now, sizeof(now));

	++_nonceCounter;

	return DigestEngine::digestToHex(md5.digest());
}


void HTTPDigestCredentials::createAuthParams(const HTTPRequest& request, const HTTPAuthenticationParams& responseAuthParams)
{
	// Not implemented: "domain" auth parameter and integrity protection.

	if (!responseAuthParams.has(NONCE_PARAM) || !responseAuthParams.has(REALM_PARAM))
		throw InvalidArgumentException("Invalid HTTP authentication parameters");

	const std::string& nonce = responseAuthParams.get(NONCE_PARAM);
	const std::string& qop = responseAuthParams.get(QOP_PARAM, DEFAULT_QOP);
	const std::string& realm = responseAuthParams.getRealm();

	_requestAuthParams.clear();
	_requestAuthParams.set(USERNAME_PARAM, _username);
	_requestAuthParams.set(NONCE_PARAM, nonce);
	_requestAuthParams.setRealm(realm);
	if (responseAuthParams.has(OPAQUE_PARAM))
	{
		_requestAuthParams.set(OPAQUE_PARAM, responseAuthParams.get(OPAQUE_PARAM));
	}
	if (responseAuthParams.has(ALGORITHM_PARAM))
	{
		_requestAuthParams.set(ALGORITHM_PARAM, responseAuthParams.get(ALGORITHM_PARAM));
	}

	if (qop.empty())
	{
		updateAuthParams(request);
	}
	else
	{
		Poco::StringTokenizer tok(qop, ",", Poco::StringTokenizer::TOK_TRIM);
		bool qopSupported = false;
		for (Poco::StringTokenizer::Iterator it = tok.begin(); it != tok.end(); ++it)
		{
			if (icompare(*it, AUTH_PARAM) == 0)
			{
				qopSupported = true;
				_requestAuthParams.set(CNONCE_PARAM, createNonce());
				_requestAuthParams.set(QOP_PARAM, *it);
				updateAuthParams(request);
				break;
			}
		}
		if (!qopSupported)
			throw NotImplementedException("Unsupported QoP requested", qop);
	}
}

void HTTPDigestCredentials::updateAuthParams(const HTTPRequest& request)
{
	const std::string qop = _requestAuthParams.get(QOP_PARAM, DEFAULT_QOP);
	const std::string realm = _requestAuthParams.getRealm();
	const std::string nonce = _requestAuthParams.get(NONCE_PARAM);

	_requestAuthParams.set(URI_PARAM, request.getURI());

	if (qop.empty())
	{
		/// Assume that https://tools.ietf.org/html/rfc7616 is not supported
		/// and still using https://tools.ietf.org/html/rfc2069#section-2.4

		MD5Engine engine;

		const std::string ha1 = digest(engine, _username, realm, _password);
		const std::string ha2 = digest(engine, request.getMethod(), request.getURI());

		_requestAuthParams.set(RESPONSE_PARAM, digest(engine, ha1, nonce, ha2));
	}
	else if (icompare(qop, AUTH_PARAM) == 0)
	{
		const std::string algorithm = _requestAuthParams.get(ALGORITHM_PARAM, MD_5_ALGORITHM);

		if (!isAlgorithmSupported(algorithm)) {
			throw NotImplementedException("Unsupported digest algorithm", algorithm);
		}

		const std::string cnonce = _requestAuthParams.get(CNONCE_PARAM);
		const std::string nc = formatNonceCounter(updateNonceCounter(nonce));

		DigestEngineProvider engineProvider(algorithm);
		DigestEngine &engine = engineProvider.engine();

		std::string ha1 = digest(engine, _username, realm, _password);

		if (engineProvider.isSessionAlgorithm()) {
			ha1 = digest(engine, ha1, nonce, cnonce);
		}

		const std::string ha2 = digest(engine, request.getMethod(), request.getURI());

		_requestAuthParams.set(NC_PARAM, nc);
		_requestAuthParams.set(CNONCE_PARAM, cnonce);
		_requestAuthParams.set(RESPONSE_PARAM, digest(engine, ha1, nonce, nc, cnonce, qop, ha2));
	}
}


bool HTTPDigestCredentials::verifyAuthInfo(const HTTPRequest& request) const
{
	HTTPAuthenticationParams params(request);
	return verifyAuthParams(request, params);
}


bool HTTPDigestCredentials::verifyAuthParams(const HTTPRequest& request, const HTTPAuthenticationParams& params) const
{
	const std::string& nonce = params.get(NONCE_PARAM);
	const std::string& realm = params.getRealm();
	const std::string& qop   = params.get(QOP_PARAM, DEFAULT_QOP);
	std::string response;
	if (qop.empty())
	{
		MD5Engine engine;

		const std::string ha1 = digest(engine, _username, realm, _password);
		const std::string ha2 = digest(engine, request.getMethod(), request.getURI());
		response = digest(engine, ha1, nonce, ha2);
	}
	else if (icompare(qop, AUTH_PARAM) == 0)
	{
		const std::string& algorithm = params.get(ALGORITHM_PARAM, MD_5_ALGORITHM);

		if (!isAlgorithmSupported(algorithm)) {
			throw NotImplementedException("Unsupported digest algorithm", algorithm);
		}

		DigestEngineProvider engineProvider(algorithm);
		DigestEngine& engine = engineProvider.engine();

		const std::string& cnonce = params.get(CNONCE_PARAM);
		const std::string& nc = params.get(NC_PARAM);

		std::string ha1 = digest(engine, _username, realm, _password);

		if (engineProvider.isSessionAlgorithm()) {
			ha1 = digest(engine, ha1, nonce, cnonce);
		}

		const std::string ha2 = digest(engine, request.getMethod(), request.getURI());
		response = digest(engine, ha1, nonce, nc, cnonce, qop, ha2);
	}
	return response == params.get(RESPONSE_PARAM);
}


int HTTPDigestCredentials::updateNonceCounter(const std::string& nonce)
{
	NonceCounterMap::iterator iter = _nc.find(nonce);

	if (iter == _nc.end())
	{
		iter = _nc.insert(NonceCounterMap::value_type(nonce, 0)).first;
	}
	iter->second++;

	return iter->second;
}


bool HTTPDigestCredentials::isAlgorithmSupported(const std::string& algorithm) const
{
	bool isAlgorithmSupported = std::find_if(std::begin(SUPPORTED_ALGORITHMS),
			std::end(SUPPORTED_ALGORITHMS),
			[&algorithm](const std::string& supportedAlgorithm)
	{
		return icompare(algorithm, supportedAlgorithm) == 0;
	}) != std::end(SUPPORTED_ALGORITHMS);

	return isAlgorithmSupported;
}


} } // namespace Poco::Net
