#
# Makefile
#
# Makefile for Poco CodeGeneration
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX   += -DCodeGeneration_EXPORTS

objects = CodeGenerator CppGenerator GeneratorEngine MethodPropertyFilter Utility

target         = PocoCodeGeneration
target_version = $(LIBVERSION)
target_libs    = PocoCppParser PocoFoundation

include $(POCO_BASE)/build/rules/lib
