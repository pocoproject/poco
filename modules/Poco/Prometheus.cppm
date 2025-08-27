module;

#ifdef ENABLE_PROMETHEUS
#include "Poco/Prometheus/AtomicFloat.h"
#include "Poco/Prometheus/CallbackMetric.h"
#include "Poco/Prometheus/Collector.h"
#include "Poco/Prometheus/Counter.h"
#include "Poco/Prometheus/Exporter.h"
#include "Poco/Prometheus/Gauge.h"
#include "Poco/Prometheus/Histogram.h"
#include "Poco/Prometheus/IntCounter.h"
#include "Poco/Prometheus/IntGauge.h"
#include "Poco/Prometheus/LabeledMetric.h"
#include "Poco/Prometheus/LabeledMetricImpl.h"
#include "Poco/Prometheus/Metric.h"
#include "Poco/Prometheus/MetricsRequestHandler.h"
#include "Poco/Prometheus/MetricsServer.h"
#include "Poco/Prometheus/ProcessCollector.h"
#include "Poco/Prometheus/Prometheus.h"
#include "Poco/Prometheus/Registry.h"
#include "Poco/Prometheus/TextExporter.h"
#include "Poco/Prometheus/ThreadPoolCollector.h"
#endif

export module Poco.Prometheus;

export namespace Poco::Prometheus {
	#ifdef ENABLE_PROMETHEUS
	using Poco::Prometheus::AtomicFloat;
	using Poco::Prometheus::CallbackMetric;
	using Poco::Prometheus::Collector;
	using Poco::Prometheus::Counter;
	using Poco::Prometheus::CounterSample;
	using Poco::Prometheus::Exporter;
	using Poco::Prometheus::Gauge;
	using Poco::Prometheus::GaugeSample;
	using Poco::Prometheus::Histogram;
	using Poco::Prometheus::HistogramData;
	using Poco::Prometheus::HistogramSample;
	using Poco::Prometheus::IntCounter;
	using Poco::Prometheus::IntGauge;
	using Poco::Prometheus::LabeledMetric;
	using Poco::Prometheus::LabeledMetricImpl;
	using Poco::Prometheus::Metric;
	using Poco::Prometheus::MetricsRequestHandler;
	using Poco::Prometheus::MetricsServer;
	using Poco::Prometheus::ProcessCollector;
	using Poco::Prometheus::Registry;
	using Poco::Prometheus::TextExporter;
	using Poco::Prometheus::ThreadPoolCollector;

	using Poco::Prometheus::CallbackCounter;
	using Poco::Prometheus::CallbackGauge;
	using Poco::Prometheus::CallbackIntCounter;
	using Poco::Prometheus::CallbackIntGauge;
	#endif
}
