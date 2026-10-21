cmake_minimum_required(VERSION 3.28)

# poco_modules_add(<definition> <library> [<module interface file>...])
# adds the module interface files to the module, the compile definition that enables them
# in Poco.cppm and in the partitions, and the library the module links
macro(poco_modules_add definition library)
	list(APPEND POCO_MODULES ${ARGN})
	list(APPEND POCO_MODULES_DEFINITIONS ${definition})
	list(APPEND POCO_MODULES_DEPENDENCIES ${library})
endmacro()

set(POCO_MODULES "")
set(POCO_MODULES_DEFINITIONS "")
set(POCO_MODULES_DEPENDENCIES "")

poco_modules_add(ENABLE_FOUNDATION Foundation Poco.cppm Poco/Foundation.cppm)

if(ENABLE_ENCODINGS)
	poco_modules_add(ENABLE_ENCODINGS Encodings)
endif()

if(ENABLE_ACTIVERECORD)
	poco_modules_add(ENABLE_ACTIVERECORD ActiveRecord Poco/ActiveRecord.cppm)
endif()

if(ENABLE_CODEGENERATION)
	poco_modules_add(ENABLE_CODEGENERATION CodeGeneration Poco/CodeGeneration.cppm)
endif()

if(ENABLE_CRYPTO)
	poco_modules_add(ENABLE_CRYPTO Crypto Poco/Crypto.cppm)
endif()

if(ENABLE_CPPPARSER)
	poco_modules_add(ENABLE_CPPPARSER CppParser Poco/CppParser.cppm)
endif()

if(ENABLE_DATA)
	poco_modules_add(ENABLE_DATA Data Poco/Data.cppm)

	if(ENABLE_DATA_MYSQL)
		poco_modules_add(ENABLE_DATA_MYSQL DataMySQL Poco/Data/MySQL.cppm)
	endif()

	if(ENABLE_DATA_ODBC)
		poco_modules_add(ENABLE_DATA_ODBC DataODBC Poco/Data/ODBC.cppm)
	endif()

	if(ENABLE_DATA_POSTGRESQL)
		poco_modules_add(ENABLE_DATA_POSTGRESQL DataPostgreSQL Poco/Data/PostgreSQL.cppm)
	endif()

	if(ENABLE_DATA_SQLITE)
		poco_modules_add(ENABLE_DATA_SQLITE DataSQLite Poco/Data/SQLite.cppm)
	endif()
endif()

if(ENABLE_DNSSD)
	poco_modules_add(ENABLE_DNSSD DNSSD Poco/DNSSD.cppm)

	if(TARGET Poco::DNSSDAvahi)
		poco_modules_add(ENABLE_DNSSD_AVAHI DNSSDAvahi Poco/DNSSD/Avahi.cppm)
	endif()

	if(TARGET Poco::DNSSDBonjour)
		poco_modules_add(ENABLE_DNSSD_BONJOUR DNSSDBonjour Poco/DNSSD/Bonjour.cppm)
	endif()
endif()

if(ENABLE_GEO)
	poco_modules_add(ENABLE_GEO Geo Poco/Geo.cppm)
endif()

if(ENABLE_JSON)
	poco_modules_add(ENABLE_JSON JSON Poco/JSON.cppm)
endif()

if(ENABLE_JWT)
	poco_modules_add(ENABLE_JWT JWT Poco/JWT.cppm)
endif()

if(ENABLE_MONGODB)
	poco_modules_add(ENABLE_MONGODB MongoDB Poco/MongoDB.cppm)
endif()

if(ENABLE_NET)
	poco_modules_add(ENABLE_NET Net Poco/Net.cppm)

	if(TARGET Poco::NetSSL)
		poco_modules_add(ENABLE_NETSSL_OPENSSL NetSSL)
	elseif(TARGET Poco::NetSSLWin)
		poco_modules_add(ENABLE_NETSSL_WIN NetSSLWin)
	endif()
endif()

if(ENABLE_PDF)
	poco_modules_add(ENABLE_PDF PDF Poco/PDF.cppm)
endif()

if(ENABLE_PROMETHEUS)
	poco_modules_add(ENABLE_PROMETHEUS Prometheus Poco/Prometheus.cppm)
endif()

if(ENABLE_REDIS)
	poco_modules_add(ENABLE_REDIS Redis Poco/Redis.cppm)
endif()

if(ENABLE_REMOTINGNG)
	poco_modules_add(ENABLE_REMOTINGNG RemotingNG Poco/RemotingNG.cppm)

	if(ENABLE_REMOTINGNG_HTTP)
		poco_modules_add(ENABLE_REMOTINGNG_HTTP RemotingNGHTTP Poco/RemotingNG/HTTP.cppm)
	endif()

	if(ENABLE_REMOTINGNG_JSONRPC)
		poco_modules_add(ENABLE_REMOTINGNG_JSONRPC RemotingNGJSONRPC Poco/RemotingNG/JSONRPC.cppm)
	endif()

	if(ENABLE_REMOTINGNG_REST)
		poco_modules_add(ENABLE_REMOTINGNG_REST RemotingNGREST Poco/RemotingNG/REST.cppm)
	endif()

	if(ENABLE_REMOTINGNG_SOAP)
		poco_modules_add(ENABLE_REMOTINGNG_SOAP RemotingNGSOAP Poco/RemotingNG/SOAP.cppm)
	endif()

	if(ENABLE_REMOTINGNG_TCP)
		poco_modules_add(ENABLE_REMOTINGNG_TCP RemotingNGTCP Poco/RemotingNG/TCP.cppm)
	endif()
endif()

if(ENABLE_SSH)
	poco_modules_add(ENABLE_SSH SSH Poco/SSH.cppm)
endif()

if(ENABLE_SERIAL)
	poco_modules_add(ENABLE_SERIAL Serial Poco/Serial.cppm)
endif()

if(ENABLE_SEVENZIP)
	poco_modules_add(ENABLE_SEVENZIP SevenZip Poco/SevenZip.cppm)
endif()

if(ENABLE_UTIL)
	poco_modules_add(ENABLE_UTIL Util Poco/Util.cppm)
endif()

if(ENABLE_XML)
	poco_modules_add(ENABLE_XML XML Poco/XML.cppm)
endif()

if(ENABLE_XSD_PARSER)
	poco_modules_add(ENABLE_XSD_PARSER XSDParser Poco/XSD/Parser.cppm)
endif()

if(ENABLE_XSD_TYPES)
	poco_modules_add(ENABLE_XSD_TYPES XSDTypes Poco/XSD/Types.cppm)
endif()

if(ENABLE_XSD_VALIDATOR)
	poco_modules_add(ENABLE_XSD_VALIDATOR XSDValidator Poco/XSD/Validator.cppm)
endif()

if(ENABLE_ZIP)
	poco_modules_add(ENABLE_ZIP Zip Poco/Zip.cppm)
endif()

add_library(Modules)
add_library(Poco::Modules ALIAS Modules)

if(NOT COMMAND configure_cpp_module_target)
    function(configure_cpp_module_target target)
        target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${POCO_MODULES})
    endfunction()
endif()

configure_cpp_module_target(Modules)

target_compile_definitions(Modules PRIVATE ${POCO_MODULES_DEFINITIONS})

list(TRANSFORM POCO_MODULES_DEPENDENCIES PREPEND "Poco::" OUTPUT_VARIABLE _poco_modules_libraries)
target_link_libraries(Modules PUBLIC ${_poco_modules_libraries})
unset(_poco_modules_libraries)

set_target_properties(Modules
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION}
    SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoModules
    DEFINE_SYMBOL Modules_EXPORTS
)

target_compile_features(Modules PUBLIC cxx_std_20)

POCO_INSTALL(Modules)
POCO_GENERATE_PACKAGE(Modules)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()
