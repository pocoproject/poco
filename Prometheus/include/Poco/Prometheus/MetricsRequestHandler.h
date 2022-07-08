//
// MetricsRequestHandler.h
//
// Library: Prometheus
// Package: HTTP
// Module:  MetricsRequestHandler
//
// Definition of the MetricsRequestHandler class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_MetricsRequestHandler_INCLUDED
#define Prometheus_MetricsRequestHandler_INCLUDED


#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Net/HTTPRequestHandler.h"


namespace Poco {
namespace Prometheus {


class Registry;


class Prometheus_API MetricsRequestHandler: public Poco::Net::HTTPRequestHandler
	/// This class handles incoming HTTP requests for metrics
	/// in the Prometheus text format.
{
public:
	MetricsRequestHandler();
		/// Creates a HTTPRequestHandler using the default Registry.

	MetricsRequestHandler(const Registry& registry);
		/// Creates a HTTPRequestHandler using the given Registry.

	// Poco::Net::HTTPRequestHandler
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
	const Registry& _registry;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_MetricsRequestHandler_INCLUDED
