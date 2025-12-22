# - Try to find Bonjour 
# (See http://developer.apple.com/networking/bonjour/index.html)
# By default available on MacOS X.
# Check for libdns_sd
#
#  BONJOUR_INCLUDE_DIR - where to find dns_sd.h, etc.
#  BONJOUR_LIBRARIES   - List of libraries when using ....
#  BONJOUR_FOUND       - True if Bonjour libraries found.

set(BONJOUR_FOUND FALSE)
set(BONJOUR_LIBRARIES)

message(STATUS "Checking whether Bonjour is supported")

# Bonjour is built-in on MacOS X / iOS (i.e. available in libSystem)
if(NOT APPLE)
	IF (WIN32)
		FIND_PATH(BONJOUR_INCLUDE_DIR dns_sd.h
			PATHS $ENV{PROGRAMW6432}/Bonjour\ SDK/Include
			)
		FIND_LIBRARY(BONJOUR_LIBRARY
			NAMES dnssd
			PATHS $ENV{PROGRAMW6432}/Bonjour\ SDK/Lib/x64
			)
	ELSE(WIN32)
		find_path(BONJOUR_INCLUDE_DIR dns_sd.h 
			PATHS /opt/dnssd/include /usr/include  /usr/local/include
			)
		find_library(BONJOUR_LIBRARY 
			NAMES dns_sd
			PATHS /opt/dnssd/lib /usr/lib /usr/local/lib
			)
	ENDIF(WIN32)
	if(NOT BONJOUR_INCLUDE_DIR OR NOT BONJOUR_LIBRARY)
		return()
	else()
		set(BONJOUR_LIBRARIES ${BONJOUR_LIBRARY} )
		set(BONJOUR_FOUND TRUE)
	endif()
else()
	find_library(BONJOUR_LIBRARY CoreServices)
	set(BONJOUR_FOUND TRUE)
endif()


mark_as_advanced( FORCE
	BONJOUR_INCLUDE_DIR
	BONJOUR_LIBRARY
	)
