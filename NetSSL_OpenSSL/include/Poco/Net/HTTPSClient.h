//
// HTTPSClient.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/HTTPSClient.h#2 $
//
// Library: NetSSL_OpenSSL
// Package: HTTPSClient
// Module:  HTTPSClient
//
// Definition of the HTTPSClient class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_HTTPSClient_INCLUDED
#define NetSSL_HTTPSClient_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/Utility.h"
#include "Poco/Net/HTTPClient.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/HTTPSSessionInstantiator.h"
#include "Poco/Net/X509Certificate.h"


namespace Poco {
namespace Net {


class SecureStreamSocket;
class HTTPRequest;
class HTTPResponse;


class NetSSL_API HTTPSClient: public HTTPClient
	/// This class implements the client-side of
	/// a HTTPS session.
	///
	/// To send a HTTPS request to a HTTPS server, first
	/// instantiate a HTTPSClient object and
	/// specify the server's host name and port number.
	///
	/// Then create a HTTPRequest object, fill it accordingly,
	/// and pass it as argument to the sendRequst() method.
	///
	/// sendRequest() will return an output stream that can
	/// be used to send the request body, if there is any.
	///
	/// After you are done sending the request body, create
	/// a HTTPResponse object and pass it to receiveResponse().
	///
	/// This will return an input stream that can be used to
	/// read the response body.
	///
	/// See RFC 2616 <http://www.faqs.org/rfcs/rfc2616.html> for more
	/// information about the HTTP protocol.
	///
	/// Note that sending requests that neither contain a content length
	/// field in the header nor are using chunked transfer encoding will
	/// result in a SSL protocol violation, as the framework shuts down
	/// the socket after sending the message body. No orderly SSL shutdown
	/// will be performed in this case.
	///
	/// If session caching has been enabled for the Context object passed
	/// to the HTTPSClient, the HTTPSClient class will
	/// attempt to reuse a previously obtained Session object in
	/// case of a reconnect.
{
public:
	enum
	{
		HTTPS_PORT = 443
	};

	explicit HTTPSClient(const URI& uri,
		InstantiatorPtr pInstantiator = new HTTPSSessionInstantiator,
		bool redirect = true);
		/// Creates a HTTPSClient using the given URI.

	explicit HTTPSClient(const SocketAddress& address,
		const std::string& scheme = "https",
		InstantiatorPtr pInstantiator = new HTTPSSessionInstantiator,
		bool redirect = true);
		/// Creates a HTTPSClient using the given address.

	HTTPSClient(const std::string& host,
		Poco::UInt16 port = HTTPS_PORT,
		const std::string& scheme = "https",
		InstantiatorPtr pInstantiator = new HTTPSSessionInstantiator,
		bool redirect = true);
		/// Creates a HTTPSClient using the given host and port.

	HTTPSClient(const std::string& host,
		Poco::UInt16 port,
		const HTTPClientSession::ProxyConfig& proxyConfig,
		const std::string& scheme = "https",
		InstantiatorPtr pInstantiator = new HTTPSSessionInstantiator,
		bool redirect = true);
		/// Creates a HTTPClient using the given host, port and proxy configuration.

	~HTTPSClient();
		/// Destroys the HTTPSClient and closes
		/// the underlying socket.
	
	bool secure() const;
		/// Return true iff the session uses SSL or TLS,
		/// or false otherwise.
		
	//X509Certificate serverCertificate();
		/// Returns the server's certificate.
		///
		/// The certificate is available after the first request has been sent.
		
	Session::Ptr sslSession();
		/// Returns the SSL Session object for the current 
		/// connection, if session caching has been enabled for
		/// the HTTPSClient's Context. A null pointer is 
		/// returned otherwise.
		///
		/// The Session object can be obtained after the first request has
		/// been sent.
/*		
	// HTTPSession
	void abort();

protected:
	void connect(const SocketAddress& address);
	std::string proxyRequestPrefix() const;
	void proxyAuthenticate(HTTPRequest& request);
	int read(char* buffer, std::streamsize length);
*/
private:
	HTTPSClient(const HTTPSClient&);
	HTTPSClient& operator = (const HTTPSClient&);
	
	Context::Ptr _pContext;
	Session::Ptr _pSession;
};


} } // namespace Poco::Net


#endif // Net_HTTPSClient_INCLUDED
