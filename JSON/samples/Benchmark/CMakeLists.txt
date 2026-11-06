poco_add_executable(sample-json-benchmark src/Benchmark.cpp)
target_link_libraries(sample-json-benchmark PUBLIC Poco::JSON)
