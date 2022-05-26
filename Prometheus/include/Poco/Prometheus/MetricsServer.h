//
// MetricsServer.h
//
// Library: Prometheus
// Package: HTTP
// Module:  MetricsServer
//
// Definition of the MetricsServer class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_MetricsServer_INCLUDED
#define Prometheus_MetricsServer_INCLUDED


#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"


namespace Poco {
namespace Prometheus {


class Registry;


class Prometheus_API MetricsServer
	/// A basic HTTP server for handling Prometheus metrics scraping
	/// requests, based on Poco::Net::HTTPServer.
{
public:
	static const Poco::UInt16 DEFAULT_PORT; /// 9100
	static const std::string DEFAULT_PATH;  /// "/metrics"

	MetricsServer(Poco::UInt16 port = DEFAULT_PORT, const std::string& path = DEFAULT_PATH);
		/// Creates a HTTPServer for serving metrics using the default Registry, listening
		/// on the given port number and the given path.

	MetricsServer(const Registry& registry, Poco::UInt16 port = DEFAULT_PORT, const std::string& path = DEFAULT_PATH);
		/// Creates a HTTPServer for serving metrics using the given Registry, listening
		/// on the given port number and the given path.

	MetricsServer(const Registry& registry, Poco::Net::ServerSocket& socket, Poco::Net::HTTPServerParams::Ptr pServerParams = defaultParams(), const std::string& path = DEFAULT_PATH);
		/// Creates a HTTPServer for serving metrics using the given Registry, listening
		/// on the server socket (which can be a properly configured Poco::Net::SecureServerSocket
		/// to enable HTTPS), and the given path.

	~MetricsServer() = default;
		/// Destroys the HTTPServer.

	void start();
		/// Starts the underlying Poco::Net::HTTPServer.

	void stop();
		/// Stops the underlying Poco::Net::HTTPServer.

protected:
	static Poco::Net::HTTPServerParams::Ptr defaultParams();

private:
	Poco::Net::HTTPServer _httpServer;

	MetricsServer(const MetricsServer&) = delete;
	MetricsServer(MetricsServer&&) = delete;
	MetricsServer& operator = (const MetricsServer&) = delete;
	MetricsServer& operator = (MetricsServer&&) = delete;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_MetricsServer_INCLUDED
