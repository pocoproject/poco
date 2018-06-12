add_executable(un7zip src/un7zip.cpp)
target_link_libraries(un7zip PUBLIC Poco::SevenZip Poco::Util Poco::XML)
target_compile_options(un7zip PUBLIC $<$<BOOL:${MINGW}>-municode>)
