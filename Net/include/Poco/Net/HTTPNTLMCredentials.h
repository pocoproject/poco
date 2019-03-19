//
// HTTPNTLMCredentials.h
//
// Library: Net
// Package: HTTP
// Module:	HTTPNTLMCredentials
//
// Definition of the HTTPNTLMCredentials class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPNTLMCredentials_INCLUDED
#define Net_HTTPNTLMCredentials_INCLUDED


#include "Poco/Net/Net.h"
#include <vector>


namespace Poco {
namespace Net {


class HTTPRequest;
class HTTPResponse;


class Net_API HTTPNTLMCredentials
	/// This is a utility class for working with
	/// HTTP NTLMv2 Authentication in HTTPRequest
	/// objects.
{
public:
	HTTPNTLMCredentials();
		/// Creates an empty HTTPNTLMCredentials object.

	HTTPNTLMCredentials(const std::string& username, const std::string& password);
		/// Creates a HTTPNTLMCredentials object with the given username and password.

	~HTTPNTLMCredentials();
		/// Destroys the HTTPNTLMCredentials.

	void reset();
		/// Resets the HTTPNTLMCredentials object to a clean state.

	void setUsername(const std::string& username);
		/// Sets the username.

	const std::string& getUsername() const;
		/// Returns the username.

	void setPassword(const std::string& password);
		/// Sets the password.

	const std::string& getPassword() const;
		/// Returns the password.

	void authenticate(HTTPRequest& request, const HTTPResponse& response);
		/// Parses WWW-Authenticate header of the HTTPResponse, initializes
		/// internal state, and adds authentication information to the given HTTPRequest.

	void authenticate(HTTPRequest& request, const std::string& ntlmChallengeBase64);
		/// Initializes internal state according to information from the
		/// ntlmChallengeBase64, and adds authentication
		/// information to the given HTTPRequest.
		///
		/// Throws InvalidArgumentException if responseAuthParams is
		/// invalid.

	void updateAuthInfo(HTTPRequest& request);
		/// Updates internal state and adds authentication information to
		/// the given HTTPRequest.

	void proxyAuthenticate(HTTPRequest& request, const HTTPResponse& response);
		/// Parses Proxy-Authenticate header of the HTTPResponse, initializes
		/// internal state, and adds proxy authentication information to the given HTTPRequest.

	void proxyAuthenticate(HTTPRequest& request, const std::string& ntlmChallengeBase64);
		/// Initializes internal state according to information from the
		/// HTTPAuthenticationParams of the response, and adds proxy authentication
		/// information to the given HTTPRequest.
		///
		/// Throws InvalidArgumentException if HTTPAuthenticationParams is
		/// invalid or some required parameter is missing.
		/// Throws NotImplementedException in case of unsupported digest
		/// algorithm or quality of protection method.

	void updateProxyAuthInfo(HTTPRequest& request);
		/// Updates internal state and adds proxy authentication information to
		/// the given HTTPRequest.

	static const std::string SCHEME;

private:
	HTTPNTLMCredentials(const HTTPNTLMCredentials&);
	HTTPNTLMCredentials& operator = (const HTTPNTLMCredentials&);

	std::string createNTLMMessage(const std::string& ntlmChallengeBase64);
	static void splitUsername(const std::string& usernameAndDomain, std::string& username, std::string& domain);
	static std::string toBase64(const std::vector<unsigned char>& buffer);
	static std::vector<unsigned char> fromBase64(const std::string& base64);

	std::string _username;
	std::string _password;
};


//
// inlines
//
inline const std::string& HTTPNTLMCredentials::getUsername() const
{
	return _username;
}


inline const std::string& HTTPNTLMCredentials::getPassword() const
{
	return _password;
}


} } // namespace Poco::Net


#endif // Net_HTTPNTLMCredentials_INCLUDED
