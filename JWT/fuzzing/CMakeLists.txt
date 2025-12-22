add_executable(JWT-decode-fuzzer JWTDecode.cpp)
target_link_libraries(JWT-decode-fuzzer PUBLIC Poco::JWT)
set_target_properties(JWT-decode-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
