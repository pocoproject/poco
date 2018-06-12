# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

add_executable(EncodingsCompiler ${SRCS} )
set_target_properties(EncodingsCompiler
    PROPERTIES
    OUTPUT_NAME tec
    )

target_link_libraries(EncodingsCompiler PUBLIC Poco::Net Poco::Util)
target_compile_options(EncodingsCompiler PUBLIC $<$<BOOL:${MINGW}>-municode>)

install(
    TARGETS EncodingsCompiler EXPORT "EncodingsCompilerTargets"
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
