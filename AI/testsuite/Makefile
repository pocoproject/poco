#
# Makefile
#
# Makefile for Poco AI testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	AITestSuite \
	ToolRegistryTest \
	AgentLoopTest \
	AnthropicProviderTest \
	OpenAIProviderTest \
	ToolDispatchProviderTest \
	MCPHostTest

target         = testrunner
target_version = 1
target_libs    = PocoAI PocoAIMCPHTTP PocoAIMCP PocoNet PocoJSON PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
