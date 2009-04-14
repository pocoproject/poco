#
# Makefile
#
# $Id: //poco/1.3/Util/Makefile#3 $
#
# Makefile for Poco Util
#

include $(POCO_BASE)/build/rules/global

objects = AbstractConfiguration Application ConfigurationMapper \
	ConfigurationView HelpFormatter IniFileConfiguration LayeredConfiguration \
	LoggingConfigurator LoggingSubsystem MapConfiguration \
	Option OptionException OptionProcessor OptionSet \
	PropertyFileConfiguration Subsystem SystemConfiguration \
	XMLConfiguration FilesystemConfiguration ServerApplication \
	Validator IntValidator RegExpValidator OptionCallback \
	Timer TimerTask

ifeq ($(POCO_CONFIG),MinGW)
	objects += WinService 
endif

target         = PocoUtil
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoXML

include $(POCO_BASE)/build/rules/lib
