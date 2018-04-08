add_executable(Text src/Text.cpp)
target_link_libraries(Text Poco::PDF Poco::Util Poco::XML Poco::Foundation)
