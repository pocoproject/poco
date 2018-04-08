add_executable(Image src/Image.cpp)
target_link_libraries(Image Poco::PDF Poco::Util Poco::XML Poco::Foundation)
