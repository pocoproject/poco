#
# Makefile
#
# $Id: //poco/1.3/CppParser/Makefile#1 $
#
# Makefile for Poco CppParser
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX   += -DCppParser_EXPORTS

objects = CppToken Decl Enum EnumValue Function NameSpace Parameter \
	Parser Struct Symbol Tokenizer TypeDef Utility Variable \
	Attributes AttributesParser

target         = PocoCppParser
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
