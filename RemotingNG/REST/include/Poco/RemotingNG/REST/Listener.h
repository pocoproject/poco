//
// Listener.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Listener
//
// Definition of the Listener class.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_Listener_INCLUDED
#define RemotingNG_REST_Listener_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/Listener.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API Listener: public Poco::RemotingNG::Listener
	/// The RemotingNG Listener implementation for the REST transport.
	///
	/// For more information, please see the Transport class.
{
public:
	using Ptr = Poco::AutoPtr<Listener>;

	enum Protocol
	{
		PROTO_HTTP,
		PROTO_HTTPS
	};

	Listener(const std::string& endPoint, Protocol proto);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// The value specified for proto determines the URI
		/// scheme used by createURI().
		///
		/// Note that the Listener instance created by this constructor
		/// does not actually accept requests itself. It merely registers an
		/// endpoint in the ORB. A Poco::Net::HTTPServer must be created
		/// for accepting REST requests using a RESTRequestHandler.

	Listener(const std::string& endPoint, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance for handling incoming REST requests, using
		/// a Poco::Net::ServerSocket bound to endPoint.

	Listener(const std::string& endPoint, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance using the given server socket for handling
		/// incoming REST requests. The given server socket can
		/// be a Poco::Net::ServerSocket or a Poco::Net::SecureServerSocket
		/// which must be bound to a socket address corresponding to
		/// the given endPoint.

	~Listener();
		/// Destroys the Listener.

	void enableChunkedTransferEncoding(bool enable = true);
		/// Enables or disables chunked transfer encoding for responses.
		/// Default is enabled.

	bool isChunkedTransferEncodingEnabled() const;
		/// Returns true iff chunked transfer encoding is enabled for responses.

	void enableCompression(bool enable = true);
		/// Enables or disables GZIP compression for responses.
		/// Default is enabled.
		///
		/// Note that chunked transfer encoding must also be enabled
		/// in order to allow compression.

	bool isCompressionEnabled() const;
		/// Returns true iff GZIP compression is enabled for responses.

	void enableCORS(bool enable = true);
		/// Enables or disables CORS (Cross Origin Resource Sharing).
		/// Default is disabled.

	bool isCORSEnabled() const;
		/// Returns true iff CORS is enabled.

	void setCORSAllowedOrigin(const std::string& origin);
		/// Sets the CORS allowed origin.
		///
		/// Can be empty (default), an asterisk ("*"), or a specific origin
		/// in the form of a complete http(s) URL ("http://domain.com").
		///
		/// In case of an empty origin specified here, all origins will
		/// be accepted and the origin specified by the client will simply
		/// be echoed back. If an asterisk is specified, again, all origins
		/// will be accepted, but the asterisk will be sent back to the client
		/// in the "Access-Control-Allow-Origin" header.
		/// If any other origin is given, the origin presented by the client
		/// must exactly match the specified origin for the request to be accepted.

	const std::string& getCORSAllowedOrigin() const;
		/// Retuns the CORS allowed origin.
		///
		/// See setCORSAllowedOrigin() for more information.

	void setCORSAllowedHeaders(const std::string& headers);
		/// Sets the CORS allowed headers.
		///
		/// Must be a list of comma-separated header names.
		/// The default is "Accept, Content-Type".
		///
		/// Note that "Content-Type" should always be included in the list.
		/// Any custom headers used for passing parameters in REST methods must
		/// be included in this list.

	const std::string& getCORSAllowedHeaders() const;
		/// Returns the CORS allowed headers.
		///
		/// See setCORSAllowedHeaders() for more information.

	void logServiceExceptions(bool enable = true);
		/// Enable or disable logging of exceptions thrown by service methods.
		///
		/// If enabled, exceptions thrown by service methods are logged with
		/// the "RemotingNG.REST.RESTRequestHandler" logger.

	bool areServiceExceptionsLogged() const;
		/// Returns true iff exceptions thrown by service methods are logged,
		/// otherwise false.

	// Poco::RemotingNG::Listener
	void start();
	void stop();
	const std::string& protocol() const;
	std::string createURI(const Poco::RemotingNG::Identifiable::TypeId& typeId, const Poco::RemotingNG::Identifiable::ObjectId& objectId);
	bool handlesURI(const std::string& uri);
	void registerObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject, Poco::RemotingNG::Skeleton::Ptr pSkeleton);
	void unregisterObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject);

	static const std::string CORS_ALLOWED_HEADERS;

private:
	bool _secure;
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHTTPServer;
	bool _logServiceExceptions;
	bool _enableChunkedTransferEncoding;
	bool _enableCompression;
	bool _enableCORS;
	std::string _corsAllowedOrigin;
	std::string _corsAllowedHeaders;
};


//
// inlines
//
inline bool Listener::isChunkedTransferEncodingEnabled() const
{
	return _enableChunkedTransferEncoding;
}


inline bool Listener::isCompressionEnabled() const
{
	return _enableCompression;
}


inline bool Listener::isCORSEnabled() const
{
	return _enableCORS;
}


inline const std::string& Listener::getCORSAllowedOrigin() const
{
	return _corsAllowedOrigin;
}


inline const std::string& Listener::getCORSAllowedHeaders() const
{
	return _corsAllowedHeaders;
}


inline bool Listener::areServiceExceptionsLogged() const
{
	return _logServiceExceptions;
}


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_Listener_INCLUDED
