include(CMakeFindDependencyMacro)
find_dependency(OpenSSL)
find_dependency(PocoFoundation)
include("${CMAKE_CURRENT_LIST_DIR}/PocoCryptoTargets.cmake")
