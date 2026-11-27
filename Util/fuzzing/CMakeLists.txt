poco_add_executable(Util-properties-fuzzer PropertiesFuzz.cpp)
target_link_libraries(Util-properties-fuzzer PUBLIC Poco::Util)
set_target_properties(Util-properties-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Util-ini-fuzzer IniFuzz.cpp)
target_link_libraries(Util-ini-fuzzer PUBLIC Poco::Util)
set_target_properties(Util-ini-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

if(ENABLE_XML)
	poco_add_executable(Util-xml-config-fuzzer XMLConfigFuzz.cpp)
	target_link_libraries(Util-xml-config-fuzzer PUBLIC Poco::Util)
	set_target_properties(Util-xml-config-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
endif()
