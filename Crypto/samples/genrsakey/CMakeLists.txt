add_executable(genrsakey src/genrsakey.cpp)
target_link_libraries(genrsakey Poco::Crypto Poco::Util Poco::XML)
