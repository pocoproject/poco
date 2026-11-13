poco_add_executable(Crypto-key-fuzzer CryptoKeyCert.cpp)
target_link_libraries(Crypto-key-fuzzer PUBLIC Poco::Crypto)
set_target_properties(Crypto-key-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
