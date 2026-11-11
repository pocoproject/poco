poco_add_executable(Crypto-x509-fuzzer X509Fuzz.cpp)
target_link_libraries(Crypto-x509-fuzzer PUBLIC Poco::Crypto)
set_target_properties(Crypto-x509-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Crypto-key-fuzzer KeyFuzz.cpp)
target_link_libraries(Crypto-key-fuzzer PUBLIC Poco::Crypto)
set_target_properties(Crypto-key-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
