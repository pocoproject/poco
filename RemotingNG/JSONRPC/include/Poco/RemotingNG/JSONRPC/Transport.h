//
// Transport.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Transport
//
// Definition of the Transport class.
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_Transport_INCLUDED
#define RemotingNG_JSONRPC_Transport_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/Deserializer.h"
#include "Poco/RemotingNG/JSONRPC/CookieStore.h"
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
	class DeflatingOutputStream;
}


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API Transport: public Poco::RemotingNG::Transport
	/// The Transport implementation for RemotingNG JSONRPC.
	///
	/// This transport implements the JSON-RPC 2.0 protocol
	/// (see <http://www.jsonrpc.org/specification> for the specification).
	///
	/// Parameters are always passed by-name through an object, passing
	/// by position (array) is not supported. Furthermore, batched requests
	/// are not supported.
	///
	/// The transport supports HTTP Basic and HTTP Digest authentication.
	/// Please note that for HTTP Digest authentication to work, chunked transfer
	/// encoding must be disabled. Also, HTTP Digest authentication is not supported
	/// for one-way requests.
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
		AUTH_ANY     = 3  /// HTTP Basic or Digest authentication (whichever the server requests).
	};

	Transport();
		/// Creates a Transport for JSON-RPC 2.0.

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
		
	bool isChunkedTransferEncodingEnabled() const;
		/// Returns true iff chunked transfer encoding is enabled,
		/// which is the default unless enableChunkedTransferEncoding(false) has
		/// been called.

	void enableChunkedTransferEncoding(bool enable = true);
		/// Enables or disables chunked transfer encoding for HTTP requests.
		/// This is normally enabled. However, some poorly implemented HTTP stacks cannot 
		/// handle chunked HTTP messages. Also, chunked transfer encoding must be
		/// disabled if AUTH_DIGEST or AUTH_ANY authentication is used.
		///
		/// Regardless of this setting, chunked transfer encoding is always supported
		/// for responses sent by the server.
		
	bool isCompressionEnabled() const;
		/// Returns true iff HTTP compression (GZIP) is enabled for requests.
		/// Default is disabled.
		
	void enableCompression(bool enable);
		/// Enables or disables HTTP compression via GZIP Content-Encoding for
		/// requests.
		/// This is normally disabled, due to a lack of support by most servers.
		///
		/// Note that chunked transfer encoding must also be enabled in
		/// order to allow compression.
		///
		/// Regardless of this setting, server responses can always be compressed 
		/// using GZIP Content-Encoding.
		
	void setAuthentication(AuthMode authMode);
		/// Sets the authentication mode.
		///
		/// Please note that for AUTH_DIGEST or AUTH_ANY, chunked
		/// transfer encoding must be disabled.
		
	AuthMode getAuthentication() const;
		/// Returns the authentication mode.

	void setCredentials(const Credentials& creds);
		/// Sets the credentials for client authentication.
		///
		/// The Credentials object should contain a username
		/// (Credentials::ATTR_USERNAME, "n") and a password
		/// (Credentials::ATTR_PASSWORD, "p") attribute.
		
	Credentials getCredentials() const;
		/// Returns the credentials, containing username and password
		/// attributes.

	//@ deprecated
	void setUsername(const std::string& username);
		/// Sets the username for HTTP authentication.
		///
		/// This method is deprecated and may be removed in a future version.
		/// Code should be changed to use setCredentials() instead.
		
	//@ deprecated
	const std::string& getUsername() const;
		/// Returns the username for HTTP authentication.
		///
		/// This method is deprecated and may be removed in a future version.
		/// Code should be changed to use getCredentials() instead.
		
	//@ deprecated
	void setPassword(const std::string& password);
		/// Sets the password for HTTP authentication.
		///
		/// This method is deprecated and may be removed in a future version.
		/// Code should be changed to use setCredentials() instead.
		
	//@ deprecated
	const std::string& getPassword() const;
		/// Returns the password for HTTP authentication.
		///
		/// This method is deprecated and may be removed in a future version.
		/// Code should be changed to use getCredentials() instead.

	const std::string& getUserAgent() const;
		/// Returns the value of the User-Agent header sent with JSON-RPC HTTP
		/// requests, or an empty string if no User-Agent value has been set.
	
	void setUserAgent(const std::string& userAgent);
		/// Sets the value of the User-Agent header sent with JSON-RPC HTTP
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
	void prepareRequest(Poco::Net::HTTPRequest& request, const std::string& messageName);
	
	static const std::string USER_AGENT;

private:
	Poco::Net::HTTPCredentials _creds;
	Poco::Net::ProxyConfig _proxyConfig;
	Poco::SharedPtr<Poco::Net::HTTPClientSession> _pSession;
	Serializer _serializer;
	Deserializer _deserializer;
	std::string _endPoint;
	Poco::URI _endPointURI;
	bool _keepAlive;
	bool _chunkedTransferEncoding;
	bool _compression;
	AuthMode _authMode;
	std::string _userAgent;
	std::map<std::string, std::string> _customHeaders;
	Poco::SharedPtr<std::stringstream> _pStream;
	Poco::SharedPtr<Poco::DeflatingOutputStream> _pDeflater;
	CookieStore::Ptr _pCookieStore;

	static Poco::Net::HTTPSessionFactory _httpSessionFactory;
	
	friend class JSONRPCRequestHandler;
};


//
// inlines
//
inline Poco::Net::HTTPSessionFactory& Transport::httpSessionFactory()
{
	return _httpSessionFactory;
}


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_Transport_INCLUDED
