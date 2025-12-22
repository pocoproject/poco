#
# Makefile
#
# Makefile for Poco Prometheus
#

include $(POCO_BASE)/build/rules/global

objects = \
	Collector \
	Counter \
	IntCounter \
	Gauge \
	IntGauge \
	LabeledMetric \
	Histogram \
	Registry \
	TextExporter \
	MetricsRequestHandler \
	MetricsServer \
	ProcessCollector \
	ThreadPoolCollector

target         = PocoPrometheus
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet

include $(POCO_BASE)/build/rules/lib
