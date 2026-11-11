poco_add_executable(Zip-decompress-fuzzer ZipDecompress.cpp)
target_link_libraries(Zip-decompress-fuzzer PUBLIC Poco::Zip)
set_target_properties(Zip-decompress-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
