#
# Makefile
#
# Makefile for Poco AI MCP testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	MCPTestSuite \
	EnvelopeTest \
	HandshakeTest \
	ToolsTest \
	StreamIngressTest \
	StdIOTest \
	FeaturesTest

target         = testrunner
target_version = 1
target_libs    = PocoAIMCP PocoJSON PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
