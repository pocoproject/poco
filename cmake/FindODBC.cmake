#
# Find the ODBC driver manager includes and library.
#
# ODBC is an open standard for connecting to different databases in a
# semi-vendor-independent fashion.  First you install the ODBC driver
# manager.  Then you need a driver for each separate database you want
# to connect to (unless a generic one works).  VTK includes neither
# the driver manager nor the vendor-specific drivers: you have to find
# those yourself.
#
# This module defines
# ODBC_INCLUDE_DIRECTORIES, where to find sql.h
# ODBC_LIBRARIES, the libraries to link against to use ODBC
# ODBC_FOUND.  If false, you cannot build anything that requires MySQL.

if(WIN32)
	get_filename_component(kit_dir "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot]" REALPATH)
	get_filename_component(kit81_dir "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot81]" REALPATH)
	set(ODBC_LIBRARIES_HINTS
		"C:/Program Files/ODBC/lib"
		"C:/ODBC/lib/debug"
		"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib"
	)
	if(X64)
		list(APPEND ODBC_LIBRARIES_HINTS "${kit81_dir}/Lib/winv6.3/um/x64")
		list(APPEND ODBC_LIBRARIES_HINTS "${kit_dir}/Lib/win8/um/x64")
	else()
		list(APPEND ODBC_LIBRARIES_HINTS "${kit81_dir}/Lib/winv6.3/um/x86")
		list(APPEND ODBC_LIBRARIES_HINTS "${kit_dir}/Lib/win8/um/x86")
	endif()
	find_library(ODBC_LIBRARIES
		NAMES odbc32
		HINTS
		${ODBC_LIBRARIES_HINTS}
		DOC "Specify the ODBC driver manager library here."
	)
	set(ODBC_INCLUDE_DIRECTORIES_HINTS
		"C:/Program Files/ODBC/include"
		"C:/Program Files/Microsoft SDKs/Windows/v7.0/include"
		"C:/Program Files/Microsoft SDKs/Windows/v6.0a/include"
		"C:/ODBC/include"
	)
	if(ODBC_LIBRARIES)
		string(FIND ${ODBC_LIBRARIES} ${kit_dir} kit_found)
		if(${kit_found} EQUAL 0)
			list(APPEND ODBC_INCLUDE_DIRECTORIES_HINTS "${kit_dir}/Include/um")
		endif()
		string(FIND ${ODBC_LIBRARIES} ${kit81_dir} kit81_found)
		if(${kit81_found} EQUAL 0)
			list(APPEND ODBC_INCLUDE_DIRECTORIES_HINTS "${kit81_dir}/Include/um")
		endif()
	endif()
	find_path(ODBC_INCLUDE_DIRECTORIES
		NAMES sql.h
		HINTS
		${ODBC_INCLUDE_DIRECTORIES_HINTS}
		DOC "Specify the directory containing sql.h."
	)
elseif(MINGW)
	set(ODBC_INCLUDE_DIRECTORIES ".")
	set(ODBC_LIBRARIES odbc32)
else()
	find_path(ODBC_INCLUDE_DIRECTORIES
		NAMES sql.h
		HINTS
		/usr/include
		/usr/include/odbc
		/usr/include/iodbc
		/usr/local/include
		/usr/local/include/odbc
		/usr/local/include/iodbc
		/usr/local/odbc/include
		/usr/local/iodbc/include
		DOC "Specify the directory containing sql.h."
	)
	find_library(ODBC_LIBRARIES
		NAMES iodbc odbc iodbcinst odbcinst odbc32
		HINTS
		/usr/lib
		/usr/lib/odbc
		/usr/lib/iodbc
		/usr/local/lib
		/usr/local/lib/odbc
		/usr/local/lib/iodbc
		/usr/local/odbc/lib
		/usr/local/iodbc/lib
		DOC "Specify the ODBC driver manager library here."
	)
endif()
	
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODBC
	DEFAULT_MSG
	ODBC_INCLUDE_DIRECTORIES
	ODBC_LIBRARIES
	)

mark_as_advanced(ODBC_FOUND ODBC_LIBRARIES ODBC_INCLUDE_DIRECTORIES)
