//
// Transport.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Transport
//
// Definition of the Transport class.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_Transport_INCLUDED
#define RemotingNG_REST_Transport_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/REST/CookieStore.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <sstream>
#include <map>


namespace Poco
{
	class InflatingInputStream;
	class DeflatingOutputStream;
}


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API Transport: public Poco::RemotingNG::Transport
	/// The Transport implementation for RemotingNG REST.
	///
	/// This transport implements REST-style APIs based on the OpenAPI
	/// specification (formerly the Swagger RESTful API)
	/// (see <https://github.com/OAI/OpenAPI-Specification/blob/master/versions/2.0.md>
	/// for the specification).
	///
	/// The transport supports HTTP Basic and HTTP Digest authentication.
	/// HTTP Digest authentication is not supported for one-way requests.
{
public:
	static const std::string PROTOCOL;
	static const std::string CONTENT_TYPE;

	enum AuthMode
		/// Authentication mode.
	{
		AUTH_NONE    = 0, /// No authentication.
		AUTH_BASIC   = 1, /// HTTP Basic Authentication only (credentials are always sent).
		AUTH_DIGEST  = 2, /// HTTP Digest Authentication only.
		AUTH_ANY     = 3, /// HTTP Basic or Digest authentication (whichever the server requests).
		AUTH_BEARER  = 4, /// OAuth 2.0 Bearer Token (Credentials::ATTR_TOKEN, Credentials::ATTR_MECHANISM).
		AUTH_CUSTOM  = 5  /// Use custom authentication header (Credentials::ATTR_TOKEN, Credentials::ATTR_MECHANISM).
	};

	Transport();
		/// Creates a Transport for REST.

	~Transport();
		/// Destroys the Transport.

	Poco::Timespan getTimeout() const;
		/// Returns the HTTP timeout.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the HTTP timeout.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	bool isKeepAliveEnabled() const;
		/// Returns true iff HTTP/1.1 persistent connections are enabled.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	void enableKeepAlive(bool enable);
		/// Enables or disables HTTP/1.1 persistent connections.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	Poco::Timespan getKeepAliveTimeout() const;
		/// Returns the timeout for HTTP/1.1 persistent connections.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	void setKeepAliveTimeout(const Poco::Timespan& timeout);
		/// Sets the timeout for HTTP/1.1 persistent connections.
		///
		/// The Transport must be connected, otherwise a Poco::IllegalStateException
		/// will be thrown.

	bool isCompressionEnabled() const;
		/// Returns true iff HTTP compression (GZIP) is enabled for requests.
		/// Default is disabled.

	void enableCompression(bool enable);
		/// Enables or disables HTTP compression via GZIP Content-Encoding for
		/// requests.
		/// This is normally disabled, due to a lack of support by most servers.
		///
		/// Regardless of this setting, server responses can always be compressed
		/// using GZIP Content-Encoding.

	void setAuthentication(AuthMode authMode);
		/// Sets the authentication mode.

	AuthMode getAuthentication() const;
		/// Returns the authentication mode.

	//@ deprecated
	void setUsername(const std::string& username);
		/// Sets the username for HTTP authentication.
		///
		/// Deprecated. Code should be changed to
		/// use setCredentials().

	//@ deprecated
	const std::string& getUsername() const;
		/// Returns the username for HTTP authentication.
		///
		/// Deprecated. Code should be changed to
		/// use getCredentials().

	//@ deprecated
	void setPassword(const std::string& password);
		/// Sets the password for HTTP authentication.
		///
		/// Deprecated. Code should be changed to
		/// use setCredentials().

	//@ deprecated
	const std::string& getPassword() const;
		/// Returns the password for HTTP authentication.
		///
		/// Deprecated. Code should be changed to
		/// use getCredentials().

	void setCredentials(const Poco::RemotingNG::Credentials& creds);
		/// Sets the credentials which must match the specified
		/// authentication mode (see setAuthentication()).
		///
		/// For AUTH_BASIC, AUTH_DIGEST and AUTH_ANY, the
		/// Credentials object must contain a
		/// Credentials::ATTR_USERNAME and a Credentials::ATTR_PASSWORD
		/// attribute.
		///
		/// For AUTH_BEARER, the Credentials object must
		/// contain a Credentials::ATTR_TOKEN attribute containing the
		/// bearer token. Optionally, a Credentials::ATTR_MECHANISM
		/// attribute can be specified to override the default
		/// authentication scheme ("Bearer").
		///
		/// For AUTH_CUSTOM, the Credentials object must
		/// contain a Credentials::ATTR_TOKEN attribute containing the
		/// value passed in the custom authentication header, as
		/// well as a Credentials::ATTR_MECHANISM attribute specifying
		/// the name of the request header.

	const Poco::RemotingNG::Credentials& getCredentials() const;
		/// Returns a const reference to the internal
		/// Credentials object.

	const std::string& getUserAgent() const;
		/// Returns the value of the User-Agent header sent with REST HTTP
		/// requests, or an empty string if no User-Agent value has been set.

	void setUserAgent(const std::string& userAgent);
		/// Sets the value of the User-Agent header sent with REST HTTP
		/// requests. If an empty string is given (default), no User-Agent
		/// header is included in the request.

	void setCustomHeader(const std::string& name, const std::string& value);
		/// Adds a custom HTTP header to the HTTP request.

	void removeCustomHeader(const std::string& name);
		/// Removes a custom HTTP header previously added with setCustomHeader().

	bool hasCustomHeader(const std::string& name) const;
		/// Returns true if a custom HTTP header with the given name has been set.

	const std::string& getCustomHeader(const std::string& name) const;
		/// Returns the value of the custom HTTP header with the given name.
		///
		/// Throws a Poco::NotFoundException if the custom header with the 
		/// given name does not exist.

	void setProxyConfig(const Poco::Net::ProxyConfig& proxyConfig);
		/// Sets the proxy configuration.

	const Poco::Net::ProxyConfig& getProxyConfig() const;
		/// Returns the proxy configuration.

	CookieStore::Ptr getCookieStore() const;
		/// Returns the CookieStore.

	void setCookieStore(CookieStore::Ptr pCookieStore);
		/// Sets the CookieStore.

	static Poco::Net::HTTPSessionFactory& httpSessionFactory();
		/// Returns the Poco::Net::HTTPSessionFactory instance used by
		/// the transport.

	// Poco::RemotingNG::Transport
	const std::string& endPoint() const;
	void connect(const std::string& endPoint);
	void disconnect();
	bool connected() const;
	Poco::RemotingNG::Serializer& beginMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	void sendMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	Poco::RemotingNG::Serializer& beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	Poco::RemotingNG::Deserializer& sendRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType);
	void endRequest();

protected:
	const Poco::Net::HTTPClientSession& requireSession() const;
	Poco::Net::HTTPClientSession& requireSession();
	void prepareRequest(const std::string& messageName);

	static const std::string USER_AGENT;
	static const std::string BEARER;
	static const std::string EMPTY;

private:
	Poco::RemotingNG::Credentials _creds;
	Poco::Net::HTTPCredentials _httpCreds;
	Poco::Net::ProxyConfig _proxyConfig;
	Poco::SharedPtr<Poco::Net::HTTPClientSession> _pSession;
	Poco::SharedPtr<Poco::Net::HTTPRequest> _pRequest;
	Poco::SharedPtr<Poco::Net::HTTPResponse> _pResponse;
	Serializer _serializer;
	Deserializer _deserializer;
	std::string _endPoint;
	Poco::URI _endPointURI;
	bool _keepAlive;
	bool _compression;
	AuthMode _authMode;
	std::string _userAgent;
	std::map<std::string, std::string> _customHeaders;
	Poco::SharedPtr<std::stringstream> _pStream;
	Poco::SharedPtr<Poco::InflatingInputStream> _pInflater;
	Poco::SharedPtr<Poco::DeflatingOutputStream> _pDeflater;
	CookieStore::Ptr _pCookieStore;

	static Poco::Net::HTTPSessionFactory _httpSessionFactory;

	friend class RESTRequestHandler;
};


//
// inlines
//
inline Poco::Net::HTTPSessionFactory& Transport::httpSessionFactory()
{
	return _httpSessionFactory;
}


inline const Poco::RemotingNG::Credentials& Transport::getCredentials() const
{
	return _creds;
}


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_Transport_INCLUDED
