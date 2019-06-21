if(@POCO_UNBUNDLED@)
  include(CMakeFindDependencyMacro)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
  find_dependency(ZLIB REQUIRED)
  find_dependency(PCRE REQUIRED)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PocoFoundationTargets.cmake")
