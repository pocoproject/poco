#
# Makefile
#
# $Id: //poco/Main/Data/testsuite/Makefile#7 $
#
# Makefile for Poco Data testsuite
#

include $(POCO_BASE)/build/rules/global

objects = DataTestSuite Driver \
	DataTest SessionPoolTest \
	Binder Extractor Preparation SessionImpl Connector TestStatementImpl

target         = testrunner
target_version = 1
target_libs    = PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
