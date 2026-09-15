//
// Listener.h
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  Listener
//
// Definition of the Listener class.
//
// Copyright (c) 2006-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_HTTP_Listener_INCLUDED
#define RemotingNG_HTTP_Listener_INCLUDED


#include "Poco/RemotingNG/HTTP/HTTP.h"
#include "Poco/RemotingNG/Listener.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


class RemotingNGHTTP_API Listener: public Poco::RemotingNG::Listener
	/// The RemotingNG Listener implementation for the RemotingNG HTTP transport.
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
		/// for accepting RemotingNG HTTP requests using a HTTPRequestHandler.

	Listener(const std::string& endPoint, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance for handling incoming HTTP requests, using
		/// a Poco::Net::ServerSocket bound to endPoint.

	Listener(const std::string& endPoint, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance using the given server socket for handling
		/// incoming HTTP requests. The given server socket can
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

	// Poco::RemotingNG::Listener
	void start();
	void stop();
	const std::string& protocol() const;
	std::string createURI(const Poco::RemotingNG::Identifiable::TypeId& typeId, const Poco::RemotingNG::Identifiable::ObjectId& objectId);
	bool handlesURI(const std::string& uri);
	void registerObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject, Poco::RemotingNG::Skeleton::Ptr pSkeleton);
	void unregisterObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject);

private:
	bool _secure;
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHTTPServer;
	bool _enableChunkedTransferEncoding;
	bool _enableCompression;
	std::string _corsAllowedOrigin;
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


} } } // namespace Poco::RemotingNG::HTTP


#endif // RemotingNG_HTTP_Listener_INCLUDED
