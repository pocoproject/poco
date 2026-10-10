# Tessil ordered-map - header-only ordered hash map/set
# Version: 1.2.0
# Source: https://github.com/Tessil/ordered-map

add_library(_BUNDLED_TESSIL INTERFACE)

target_include_directories(_BUNDLED_TESSIL
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(
	FILES
		include/Poco/ordered_hash.h
		include/Poco/ordered_map.h
		include/Poco/ordered_set.h
	DESTINATION include/Poco
	COMPONENT Devel
)

add_library(Tessil::OrderedMap ALIAS _BUNDLED_TESSIL)
