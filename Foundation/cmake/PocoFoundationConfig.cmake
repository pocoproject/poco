if(@POCO_UNBUNDLED@)
  include(CMakeFindDependencyMacro)
  find_dependency(ZLIB REQUIRED)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PocoFoundationTargets.cmake")
