add_executable(Template src/Template.cpp)
target_link_libraries(Template Poco::PDF Poco::Util Poco::XML Poco::Foundation)
