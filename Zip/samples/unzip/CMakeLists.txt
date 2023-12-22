add_executable(sample-unzip src/unzip.cpp)

target_link_libraries(sample-unzip PUBLIC Poco::Zip Poco::Util Poco::XML)
