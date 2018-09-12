add_executable(TextConverter src/TextConverter.cpp)
target_link_libraries(TextConverter PUBLIC Poco::Encodings)
