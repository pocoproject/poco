#
# Makefile
#
# Makefile for Poco Util testsuite
#

include $(POCO_BASE)/build/rules/global

objects = AbstractConfigurationTest ConfigurationTestSuite \
	ConfigurationMapperTest ConfigurationViewTest Driver  \
	HelpFormatterTest IniFileConfigurationTest LayeredConfigurationTest \
	LocalConfigurationViewTest LoggingConfiguratorTest MapConfigurationTest \
	OptionProcessorTest OptionSetTest OptionTest \
	OptionsTestSuite PropertyFileConfigurationTest \
	SystemConfigurationTest UtilTestSuite XMLConfigurationTest \
	FilesystemConfigurationTest ValidatorTest \
	TimerTestSuite TimerTest \
	JSONConfigurationTest

# FastLogger - enabled by default
# Set POCO_NO_FASTLOGGER=1 to disable
ifdef POCO_NO_FASTLOGGER
COMMONFLAGS += -DPOCO_NO_FASTLOGGER
endif

target         = testrunner
target_version = 1
target_libs    = PocoUtil PocoXML PocoJSON PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz -lpcre -lexpat
endif
