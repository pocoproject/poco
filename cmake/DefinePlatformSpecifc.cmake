# http://www.cmake.org/Wiki/CMake_Useful_Variables :
# CMAKE_BUILD_TYPE
#    Choose the type of build. CMake has default flags for these:
#
#    * None (CMAKE_C_FLAGS or CMAKE_CXX_FLAGS used)
#    * Debug (CMAKE_C_FLAGS_DEBUG or CMAKE_CXX_FLAGS_DEBUG)
#    * Release (CMAKE_C_FLAGS_RELEASE or CMAKE_CXX_FLAGS_RELEASE)
#    * RelWithDebInfo (CMAKE_C_FLAGS_RELWITHDEBINFO or CMAKE_CXX_FLAGS_RELWITHDEBINFO
#    * MinSizeRel (CMAKE_C_FLAGS_MINSIZEREL or CMAKE_CXX_FLAGS_MINSIZEREL)
 
# Setting CXX Flag /MD or /MT and POSTFIX values i.e MDd / MD / MTd / MT / d
if(WIN32)
	if(POCO_MT)
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

		if(POCO_STATIC)
			set(CMAKE_DEBUG_POSTFIX "mtd" CACHE STRING "Set Debug library postfix" FORCE)	
			set(CMAKE_RELEASE_POSTFIX "mt" CACHE STRING "Set Release library postfix" FORCE)
			set(CMAKE_MINSIZEREL_POSTFIX "mt" CACHE STRING "Set MinSizeRel library postfix" FORCE)
			set(CMAKE_RELWITHDEBINFO_POSTFIX "mtd" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
		else(POCO_STATIC)
			set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set Debug library postfix" FORCE)	
			set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix" FORCE)
			set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Set MinSizeRel library postfix" FORCE)
			set(CMAKE_RELWITHDEBINFO_POSTFIX "d" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
		endif()
	elseif(POCO_MD)
		string(REPLACE "/MT" "/MD" ${CMAKE_CXX_FLAGS_DEBUG} {${CMAKE_CXX_FLAGS_DEBUG})
		string(REPLACE "/MT" "/MD" ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_RELEASE})
		string(REPLACE "/MT" "/MD" ${CMAKE_CXX_FLAGS_MINSIZEREL} ${CMAKE_CXX_FLAGS_MINSIZEREL})
		string(REPLACE "/MT" "/MD" ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} {${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

		if(POCO_STATIC)
			set(CMAKE_DEBUG_POSTFIX "mdd" CACHE STRING "Set Debug library postfix" FORCE)	
			set(CMAKE_RELEASE_POSTFIX "md" CACHE STRING "Set Release library postfix" FORCE)
			set(CMAKE_MINSIZEREL_POSTFIX "md" CACHE STRING "Set MinSizeRel library postfix" FORCE)
			set(CMAKE_RELWITHDEBINFO_POSTFIX "mdd" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
		else(POCO_STATIC)
			set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set Debug library postfix" FORCE)	
			set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix" FORCE)
			set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Set MinSizeRel library postfix" FORCE)
			set(CMAKE_RELWITHDEBINFO_POSTFIX "d" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
		endif(POCO_STATIC)
	endif()
endif(WIN32)



# OS Detection
include(CheckTypeSize)
find_package(Cygwin)

if(WIN32)
  add_definitions( -DPOCO_OS_FAMILY_WINDOWS -DUNICODE -D_UNICODE -D__LCC__)  #__LCC__ define used by MySQL.h
  set(SYSLIBS ${SYSLIBS} Iphlpapi.lib gdi32.lib)
  if(ENABLE_DATA_ODBC)
	set(SYSLIBS ${SYSLIBS} odbc32.lib)
  endif(ENABLE_DATA_ODBC)
endif(WIN32)

if (UNIX AND NOT ANDROID )
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  if (APPLE)
    add_definitions( -DPOCO_HAVE_IPv6 -DPOCO_NO_STAT64)
    set(SYSLIBS  dl)
  else (APPLE)
    add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
    set(SYSLIBS  pthread dl rt)
  endif (APPLE)
endif(UNIX AND NOT ANDROID )

if (CMAKE_SYSTEM MATCHES "SunOS")
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )
  set(SYSLIBS  pthread socket xnet nsl resolv rt dl)
endif(CMAKE_SYSTEM MATCHES "SunOS")

if (CMAKE_COMPILER_IS_MINGW)
  add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400  -DPOCO_WIN32_UTF8)
  add_definitions(-D_WIN32 -DMINGW32 -DWINVER=0x500 -DODBCVER=0x0300 -DPOCO_THREAD_STACK_SIZE)
endif (CMAKE_COMPILER_IS_MINGW)

if (CYGWIN)
#    add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400  -DPOCO_WIN32_UTF8)
endif (CYGWIN)

# SunPro C++
if (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
  add_definitions( -D_BSD_SOURCE -library=stlport4)
endif (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")

# iOS
if (IOS)
  add_definitions( -DPOCO_HAVE_IPv6 -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_STAT64 -DPOCO_NO_SHAREDLIBS -DPOCO_NO_NET_IFTYPES )
endif(IOS)

#Android
if (ANDROID)
  add_definitions( -DPOCO_ANDROID -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_WSTRING -DPOCO_NO_SHAREDMEMORY )
endif(ANDROID)