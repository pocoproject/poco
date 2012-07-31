set( TEST_SRCS
src/AbstractConfigurationTest.cpp
src/JSONConfigurationTest.cpp
src/ConfigurationMapperTest.cpp
src/ConfigurationTestSuite.cpp
src/ConfigurationViewTest.cpp
src/Driver.cpp
src/FilesystemConfigurationTest.cpp
src/HelpFormatterTest.cpp
src/IniFileConfigurationTest.cpp
src/LayeredConfigurationTest.cpp
src/LoggingConfiguratorTest.cpp
src/MapConfigurationTest.cpp
src/OptionProcessorTest.cpp
src/OptionSetTest.cpp
src/OptionTest.cpp
src/OptionsTestSuite.cpp
src/PropertyFileConfigurationTest.cpp
src/SystemConfigurationTest.cpp
src/UtilTestSuite.cpp
src/ValidatorTest.cpp
src/XMLConfigurationTest.cpp
src/TimerTestSuite.cpp
src/TimerTest.cpp
)

set( WIN_TEST_SRCS
src/WinConfigurationTest.cpp
src/WinDriver.cpp
src/WinRegistryTest.cpp
src/WindowsTestSuite.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoUtil PocoJSON PocoXML PocoFoundation CppUnit)
