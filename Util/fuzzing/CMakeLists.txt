poco_add_executable(Util-config-fuzzer UtilConfig.cpp)
target_link_libraries(Util-config-fuzzer PUBLIC Poco::Util)
set_target_properties(Util-config-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
