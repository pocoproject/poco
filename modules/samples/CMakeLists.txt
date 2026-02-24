add_executable(moduletest src/ModuleTest.cpp)
target_compile_definitions(moduletest PUBLIC ENABLE_MODULES)
target_link_libraries(moduletest PUBLIC Poco::Modules Poco::Foundation)
