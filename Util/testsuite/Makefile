#
# Makefile
#
# $Id: //poco/svn/Util/testsuite/Makefile#1 $
#
# Makefile for Poco Util testsuite
#

include $(POCO_BASE)/build/rules/global

objects = AbstractConfigurationTest ConfigurationTestSuite \
	ConfigurationMapperTest ConfigurationViewTest Driver  \
	HelpFormatterTest IniFileConfigurationTest LayeredConfigurationTest \
	LoggingConfiguratorTest MapConfigurationTest \
	OptionProcessorTest OptionSetTest OptionTest \
	OptionsTestSuite PropertyFileConfigurationTest \
	SystemConfigurationTest UtilTestSuite XMLConfigurationTest \
	FilesystemConfigurationTest ValidatorTest

target         = testrunner
target_version = 1
target_libs    = PocoUtil PocoXML PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
