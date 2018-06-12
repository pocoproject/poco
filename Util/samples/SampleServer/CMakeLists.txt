add_executable(SampleServer src/SampleServer.cpp)
target_link_libraries(SampleServer PUBLIC Poco::Util Poco::JSON Poco::XML)
target_compile_options(SampleServer PUBLIC 
    $<$<BOOL:${MINGW}>-municode>
    $<$<BOOL:${WINCE}>/ENTRY:wmainCRTStartup>
)
