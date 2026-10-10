#
# Makefile
#
# Makefile for Poco Prometheus
#

include $(POCO_BASE)/build/rules/global

objects = \
	Collector \
	Counter \
	Exporter \
	Gauge \
	Histogram \
	IntCounter \
	IntGauge \
	LabeledMetric \
	Metric \
	MetricsRequestHandler \
	MetricsServer \
	ProcessCollector \
	Registry \
	TextExporter \
	ThreadPoolCollector

target         = PocoPrometheus
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet

include $(POCO_BASE)/build/rules/lib
