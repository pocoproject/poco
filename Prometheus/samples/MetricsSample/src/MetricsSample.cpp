#include "Poco/Prometheus/Counter.h"
#include "Poco/Prometheus/Gauge.h"
#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/Prometheus/ProcessCollector.h"
#include "Poco/Prometheus/ThreadPoolCollector.h"
#include "Poco/Prometheus/MetricsServer.h"
#include "Poco/Util/ServerApplication.h"
#include <iostream>
#include <cmath>


using Poco::Util::ServerApplication;
using namespace std::string_literals;
using namespace Poco::Prometheus;


class MetricsSampleApp: public ServerApplication
{
protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}

	int main(const ArgVec& args)
	{
		try
		{
			Counter counter1("test_counter"s);
			counter1.help("A test counter"s);

			counter1.inc();

			Counter counter2("http_requests"s, {
				/*.help = */"HTTP requests, grouped by method"s,
				/*.labelNames =*/ {"method"s}
			});

			counter2.labels({"POST"s}).inc();
			counter2.labels({"GET"s}).inc();

			Counter counter3("http_requests_uri"s, {
				/*.help =*/ "HTTP requests, grouped by method and uri"s,
				/*.labelNames =*/ {"method"s, "path"s}
			});

			counter3.labels({"POST"s, "/metrics"}).inc(3);
			counter3.labels({"GET"s, "/test"s}).inc();

			CallbackIntCounter counter4("callback_int"s, []()
				{
					return 42;
				}
			);

			Gauge gauge1("sample_count"s);
			gauge1.help("A sample gauge"s);
			gauge1.set(-INFINITY);

			Histogram histo("sample_histogram"s, {
				/*.help =*/ "A histogram"s,
				/*.labelNames =*/ {},
				/*.buckets =*/ {1.0, 2.0, 3.0}
			});

			histo.observe(0.5);
			histo.observe(1.1);
			histo.observe(5.0);
			histo.observe(2.1);

			ProcessCollector pc;
			ThreadPoolCollector tpc;

			const Poco::UInt16 metricsPort = static_cast<Poco::UInt16>(config().getInt("metrics.port"s, 9100));
			MetricsServer server(metricsPort);
			server.start();

			waitForTerminationRequest();

			server.stop();
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << exc.displayText() << std::endl;
			return Application::EXIT_SOFTWARE;
		}

		return Application::EXIT_OK;
	}
};


POCO_SERVER_MAIN(MetricsSampleApp)
