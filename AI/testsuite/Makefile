#
# Makefile
#
# Makefile for Poco AI testsuite
#

include $(POCO_BASE)/build/rules/global

# The MCPHost tests serve their MCP servers with the test host of the
# AI/MCP/HTTP test suite.
INCLUDE += -I$(POCO_BASE)/AI/MCP/HTTP/testsuite/src

objects = \
	Driver \
	AITestSuite \
	ToolRegistryTest \
	CancellationTest \
	AgentLoopTest \
	AnthropicProviderTest \
	OpenAIProviderTest \
	MCPHostTest \
	ChatServiceTest

target         = testrunner
target_version = 1
target_libs    = PocoAI PocoAIMCPHTTP PocoAIMCP PocoNet PocoJSON PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
