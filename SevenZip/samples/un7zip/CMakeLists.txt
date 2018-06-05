set(SAMPLE_NAME "un7zip")

add_executable(un7zip src/un7zip.cpp)
target_link_libraries(un7zip PUBLIC Poco::SevenZip Poco::Util Poco::XML)

POCO_ENABLE_EXE_WMAIN(un7zip)
