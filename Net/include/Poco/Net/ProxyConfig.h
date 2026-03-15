//
// ProxyConfig.h
//
// Library: Net
// Package: HTTP
// Module:  ProxyConfig
//
// Definition of the ProxyConfig class.
//
// Copyright (c) 2026-2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Net_ProxyConfig_INCLUDED
#define Net_ProxyConfig_INCLUDED

#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"

namespace Poco {
namespace Net {


enum class ProxyAuthentication
{
	None,        /// No proxy authentication
	Basic,       /// HTTP Basic proxy authentication (default, if username and password are supplied)
	Digest,      /// HTTP Digest proxy authentication
	NTLM         /// NTLMv2 proxy authentication
};


struct ProxyConfig
	/// HTTP proxy server configuration.
{
	ProxyConfig() = default;

	std::string host;
		/// Proxy server host name or IP address.
	Poco::UInt16 port = HTTPSession::HTTP_PORT;
		/// Proxy server TCP port.
	std::string protocol = "http";
		/// Protocol to use (http or https).
	bool tunnel = true;
		/// Use proxy as tunnel (establish 2-way communication through CONNECT request).
		/// If tunnel option is 'false' request will be sent directly to proxy without CONNECT request.
		///
		/// Warning: Setting tunnel to false for HTTPS sessions means the TLS connection
		/// terminates at the proxy, not at the destination server. The proxy will see
		/// the request in plaintext.
	std::string username;
		/// Proxy server username.
	std::string password;
		/// Proxy server password.
	std::string nonProxyHosts;
		/// A regular expression defining hosts for which the proxy should be bypassed,
		/// e.g. "localhost|127\.0\.0\.1|192\.168\.0\.\d+". Can also be an empty
		/// string to disable proxy bypassing.

	ProxyAuthentication authMethod = ProxyAuthentication::Basic;
		/// The authentication method to use - HTTP Basic or NTLM.
};


} } // namespace Poco::Net

#endif // Net_ProxyConfig_INCLUDED
