add_executable(sample-zip src/zip.cpp)
target_link_libraries(sample-zip PUBLIC Poco::Zip Poco::Util Poco::XML)
