//
// Listener.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Listener
//
// Definition of the Listener class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_SOAP_Listener_INCLUDED
#define RemotingNG_SOAP_Listener_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/Deserializer.h"
#include "Poco/RemotingNG/Listener.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API Listener: public Poco::RemotingNG::Listener
	/// The RemotingNG Listener implementation for the SOAP transport.
{
public:
	using Ptr = Poco::AutoPtr<Listener>;

	enum Protocol
	{
		PROTO_HTTP,
		PROTO_HTTPS
	};

	using RequestHeaderInterceptor = Deserializer::HeaderInterceptor;

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
		/// for accepting SOAP requests using a SOAPRequestHandler.

	Listener(const std::string& endPoint, const std::string& wsdlPath, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance for handling incoming SOAP requests, using
		/// a Poco::Net::ServerSocket bound to endPoint.
		///
		/// wsdlPath specifies the path to the directory containing
		/// WSDL documents. It can be empty if the WSDL documents are
		/// located in the current working directory.

	Listener(const std::string& endPoint, const std::string& wsdlPath, const Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams);
		/// Creates a Listener for the given endpoint,
		/// which must be in <host>:<port> format.
		///
		/// This constructor will create a Poco::Net::HTTPServer
		/// instance using the given server socket for handling
		/// incoming SOAP requests. The given server socket can
		/// be a Poco::Net::ServerSocket or a Poco::Net::SecureServerSocket
		/// which must be bound to a socket address corresponding to
		/// the given endPoint.
		///
		/// wsdlPath specifies the path to the directory containing
		/// WSDL documents. It can be empty if the WSDL documents are
		/// located in the current working directory.

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

	void enableMTOM(bool enable = true);
		/// Enables or disables MTOM (Message Transport Optimization Mechanism)
		/// for server responses. If enabled, every response will be sent in a
		/// MIME multipart message and binary data (std::vector<char>) will
		/// be sent raw in a separate MIME part instead of Base64-encoded within
		/// the SOAP body.
		///
		/// See http://www.w3.org/TR/soap12-mtom/ for more information about MTOM.

	bool isMTOMEnabled() const;
		/// Returns true iff MTOM is enabled.

	void enableWSSE(bool enable = true);
		/// Enables or disables support for WS-Security Username Token Profile.
		/// If enabled, an Authenticator must be registered that can handle 
		/// WSSE credentials, which consist of:
		///   - username
		///   - password
		///   - nonce
		///   - created
	
	bool isWSSEEnabled() const;
		/// Returns true if WSSE is enabled.

	void addRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor);
		/// Adds a header interceptor to the request deserializer.

	void removeRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor);
		/// Removes a header interceptor from the request deserializer.

	const std::vector<Deserializer::HeaderInterceptor::Ptr>& requestHeaderInterceptors() const;

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
	bool _enableMTOM;
	bool _enableWSSE;
	std::vector<RequestHeaderInterceptor::Ptr> _requestHeaderInterceptors;
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


inline bool Listener::isMTOMEnabled() const
{
	return _enableMTOM;
}


inline bool Listener::isWSSEEnabled() const
{
	return _enableWSSE;
}


inline const std::vector<Deserializer::HeaderInterceptor::Ptr>& Listener::requestHeaderInterceptors() const
{
	return _requestHeaderInterceptors;
}


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_Listener_INCLUDED
