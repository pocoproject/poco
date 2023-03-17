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
#include "Poco/Net/SSPINTLMCredentials.h"
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

	HTTPNTLMCredentials(const std::string& username, const std::string& password, const std::string& host);
		/// Creates a HTTPNTLMCredentials object with the given username, password and target host.

	~HTTPNTLMCredentials();
		/// Destroys the HTTPNTLMCredentials.

	void reset();
		/// Resets the HTTPNTLMCredentials object to a clean state.
		/// Does not clear username and password.

	void clear();
		/// Clears username, password and host.

	void setUsername(const std::string& username);
		/// Sets the username.

	const std::string& getUsername() const;
		/// Returns the username.

	void setPassword(const std::string& password);
		/// Sets the password.

	const std::string& getPassword() const;
		/// Returns the password.

	bool empty() const;
		/// Returns true if both username and password are empty, otherwise false.

	void setHost(const std::string& host);
		/// Sets the target host.
		///
		/// Used for SSPI-based NTLM authentication only.

	const std::string& getHost() const;
		/// Returns the target host.

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
	bool useSSPINTLM() const;

	std::string _username;
	std::string _password;
	std::string _host;
	Poco::SharedPtr<NTLMContext> _pNTLMContext;
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


inline const std::string& HTTPNTLMCredentials::getHost() const
{
	return _host;
}


inline bool HTTPNTLMCredentials::useSSPINTLM() const
{
	return _username.empty() && _password.empty() && SSPINTLMCredentials::available();
}


inline bool HTTPNTLMCredentials::empty() const
{
	return _username.empty() && _password.empty();
}


} } // namespace Poco::Net


#endif // Net_HTTPNTLMCredentials_INCLUDED
