poco_add_executable(Zip-archive-fuzzer ArchiveFuzz.cpp)
target_link_libraries(Zip-archive-fuzzer PUBLIC Poco::Zip)
set_target_properties(Zip-archive-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Zip-decompress-fuzzer DecompressFuzz.cpp)
target_link_libraries(Zip-decompress-fuzzer PUBLIC Poco::Zip)
set_target_properties(Zip-decompress-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
