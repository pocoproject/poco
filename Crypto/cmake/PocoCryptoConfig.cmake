include(CMakeFindDependencyMacro)
find_dependency(PocoFoundation)
find_dependency(OpenSSL REQUIRED COMPONENTS Crypto)
include("${CMAKE_CURRENT_LIST_DIR}/PocoCryptoTargets.cmake")
