poco_add_executable(moduletest src/ModuleTest.cpp)
target_compile_definitions(moduletest PUBLIC ENABLE_MODULES)
target_link_libraries(moduletest PUBLIC Poco::Modules Poco::Foundation)
# import Poco; needs module scanning, which is off by default under the 3.26 floor (CMP0155).
set_target_properties(moduletest PROPERTIES CXX_SCAN_FOR_MODULES ON)
