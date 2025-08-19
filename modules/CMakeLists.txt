cmake_minimum_required(VERSION 3.28)

add_library(PocoModules)
add_library(Poco::PocoModules ALIAS PocoModules)

set(POCO_MODULES
	Poco.cppm
)

if(ENABLE_FOUNDATION)
	list(APPEND POCO_MODULES
		Foundation.cppm
		Dynamic.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_FOUNDATION)
	if(ENABLE_ENCODINGS)
		target_compile_definitions(PocoModules PUBLIC ENABLE_ENCODINGS)
	endif()
endif()

if(ENABLE_ACTIVERECORD)
	list(APPEND POCO_MODULES
		ActiveRecord.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_ACTIVERECORD)
endif()

if(ENABLE_CRYPTO)
	list(APPEND POCO_MODULES
		Crypto.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_CRYPTO)
endif()

if(ENABLE_DATA)
	list(APPEND POCO_MODULES
		Data.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_DATA)

	if(ENABLE_DATA_MYSQL)
		list(APPEND POCO_MODULES
			Data/MySQL.cppm
		)
		target_compile_definitions(PocoModules PUBLIC ENABLE_DATA_MYSQL)
	endif()

	if(ENABLE_DATA_ODBC)
		list(APPEND POCO_MODULES
			Data/ODBC.cppm
		)
		target_compile_definitions(PocoModules PUBLIC ENABLE_DATA_ODBC)
	endif()

	if(ENABLE_DATA_POSTGRESQL)
		list(APPEND POCO_MODULES
			Data/PostgreSQL.cppm
		)
		target_compile_definitions(PocoModules PUBLIC ENABLE_DATA_POSTGRESQL)
	endif()

	if(ENABLE_DATA_SQLITE)
		list(APPEND POCO_MODULES
			Data/SQLite.cppm
		)
		target_compile_definitions(PocoModules PUBLIC ENABLE_DATA_SQLITE)
	endif()
endif()

if(ENABLE_JSON)
	list(APPEND POCO_MODULES
		JSON.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_JSON)
endif()

if(ENABLE_JWT)
	list(APPEND POCO_MODULES
		JWT.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_JWT)
endif()

if(ENABLE_MONGODB)
	list(APPEND POCO_MODULES
		MongoDB.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_MONGODB)
endif()

if(ENABLE_NET)
	list(APPEND POCO_MODULES
		Net.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_NET)
endif()

if(ENABLE_PROMETHEUS)
	list(APPEND POCO_MODULES
		Prometheus.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_PROMETHEUS)
endif()

if(ENABLE_REDIS)
	list(APPEND POCO_MODULES
		Redis.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_REDIS)
endif()

if(ENABLE_UTIL)
	list(APPEND POCO_MODULES
		Util.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_UTIL)
endif()

if(ENABLE_XML)
	list(APPEND POCO_MODULES
		XML.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_XML)
endif()

if(ENABLE_ZIP)
	list(APPEND POCO_MODULES
		Zip.cppm
	)
	target_compile_definitions(PocoModules PUBLIC ENABLE_ZIP)
endif()

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
