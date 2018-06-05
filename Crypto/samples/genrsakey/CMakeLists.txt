add_executable(genrsakey src/genrsakey.cpp)
target_link_libraries(genrsakey PUBLIC Poco::Crypto Poco::Util Poco::XML)

POCO_ENABLE_EXE_WMAIN(genrsakey)
