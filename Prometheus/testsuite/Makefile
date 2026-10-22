#
# Makefile
#
# Makefile for Poco Prometheus testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	CallbackMetricTest \
	CounterTest \
	GaugeTest \
	HistogramTest \
	IntCounterTest \
	IntGaugeTest \
	ProcessCollectorTest \
	PrometheusTestSuite \
	RegistryTest

target         = testrunner
target_version = 1
target_libs    = PocoPrometheus PocoNet PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
