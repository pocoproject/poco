//
// ProxyConfig.h
//
// Library: Net
// Package: HTTP
// Module:  ProxyConfig
//
// Definition of the ProxyConfig class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Net_ProxyConfig_INCLUDED
#define Net_ProxyConfig_INCLUDED

#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include <string>

namespace Poco {
namespace Net {
        enum ProxyAuthentication {
            PROXY_AUTH_NONE,        /// No proxy authentication
            PROXY_AUTH_HTTP_BASIC,  /// HTTP Basic proxy authentication (default, if username and password are supplied)
            PROXY_AUTH_HTTP_DIGEST, /// HTTP Digest proxy authentication
            PROXY_AUTH_NTLM         /// NTLMv2 proxy authentication
        };

        struct ProxyConfig
            /// HTTP proxy server configuration.
        {
            ProxyConfig() :
                    port(HTTPSession::HTTP_PORT),
                    protocol("http"),
                    tunnel(true),
                    authMethod(PROXY_AUTH_HTTP_BASIC) {
            }

            std::string host;
            /// Proxy server host name or IP address.
            Poco::UInt16 port;
            /// Proxy server TCP port.
            std::string protocol;
            /// Protocol to use (http or https).
            bool tunnel;
            /// Use proxy as tunnel (establish 2-way communication through CONNECT request).
            /// If tunnel option is 'false' request will be send directly to proxy without CONNECT request.
            std::string username;
            /// Proxy server username.
            std::string password;
            /// Proxy server password.
            std::string nonProxyHosts;
            /// A regular expression defining hosts for which the proxy should be bypassed,
            /// e.g. "localhost|127\.0\.0\.1|192\.168\.0\.\d+". Can also be an empty
            /// string to disable proxy bypassing.

            ProxyAuthentication authMethod;
            /// The authentication method to use - HTTP Basic or NTLM.
        };

} } // namespace Poco::Net

#endif // Net_ProxyConfig_INCLUDED
