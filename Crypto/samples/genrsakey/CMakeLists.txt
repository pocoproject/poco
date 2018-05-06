add_executable(genrsakey src/genrsakey.cpp)
target_link_libraries(genrsakey PUBLIC Poco::Crypto Poco::Util Poco::XML)
