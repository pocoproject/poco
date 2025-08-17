cmake_minimum_required(VERSION 3.28)

add_library(PocoModules)
add_library(Poco::PocoModules ALIAS PocoModules)

set(POCO_MODULES
    Foundation.cppm
	ActiveRecord.cppm
	Crypto.cppm
	Data.cppm
	Dynamic.cppm
	JSON.cppm
	JWT.cppm
	MongoDB.cppm
	Net.cppm
	Prometheus.cppm
	Redis.cppm
	Util.cppm
	XML.cppm
	Zip.cppm
	Poco.cppm
)

if(NOT COMMAND configure_cpp_module_target)
    function(configure_cpp_module_target target)
        target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${POCO_MODULES})
    endfunction()
endif()

configure_cpp_module_target(PocoModules)

set_target_properties(PocoModules
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} 
    SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoModules
    DEFINE_SYMBOL POCOMODULES_EXPORTS
)

target_link_libraries(PocoModules PUBLIC 
    Poco::Foundation
    Poco::ActiveRecord
	Poco::Crypto
	Poco::Data
	Poco::JSON
	Poco::JWT
	Poco::MongoDB
	Poco::Net
	Poco::Prometheus
	Poco::Redis
	Poco::Util
	Poco::Zip
)

target_include_directories(PocoModules
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(PocoModules PUBLIC cxx_std_20)

if(POCO_MODULES_EXPORT_IMPL)
	target_compile_definitions(PocoModules PUBLIC POCO_MODULES_EXPORT_IMPL)
endif()
