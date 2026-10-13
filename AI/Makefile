#
# Makefile
#
# Makefile for Poco AI (LLM providers and agent runtime)
#

include $(POCO_BASE)/build/rules/global

objects = AgentLoop AnthropicProvider MCPHost OpenAIProvider ToolDispatchProvider ToolRegistry

target         = PocoAI
target_version = $(LIBVERSION)
target_libs    = PocoAIMCPHTTP PocoAIMCP PocoNet PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/lib
