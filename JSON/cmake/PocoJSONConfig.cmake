include(CMakeFindDependencyMacro)
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR})
find_dependency(PocoFoundation)
include("${CMAKE_CURRENT_LIST_DIR}/PocoJSONTargets.cmake")