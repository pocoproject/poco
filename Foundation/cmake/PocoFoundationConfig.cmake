if(@POCO_UNBUNDLED@)
	include(CMakeFindDependencyMacro)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
	find_dependency(ZLIB REQUIRED)
	find_dependency(PCRE2 REQUIRED)
	find_dependency(Utf8Proc REQUIRED)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PocoFoundationTargets.cmake")
