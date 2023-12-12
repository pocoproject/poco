# Sources
file(GLOB SRCS_G "src/*.cpp")

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")

add_executable(ProGen ${SRCS_G} ${HDRS_G})

target_link_libraries(ProGen PUBLIC Poco::Foundation Poco::Util Poco::XML)
