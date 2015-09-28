# Find the native PostgreSQL includes and library
#
#  POSTGRESQL_INCLUDE_DIR - Where to find libpq-fe.h, etc.
#  POSTGRESQL_LIB_DIR     - Where to find libpq
#  POSTGRESQL_LIB         - Full path to libpq
#  POSTGRESQL_FOUND       - True if PostgreSQL found.

#
# On Windows x64 CMake (32-bit) can't seem to find PostgreSQL directories
# $ENV{ProgramFiles} under C:\Program Files
#
find_path(POSTGRESQL_INCLUDE_DIR
    NAMES libpq-fe.h
    HINTS
    $ENV{POSTGRESQL_INCLUDE_DIR}
    $ENV{POSTGRESQL_INCLUDE_DIR}/include
    /Applications/Postgres.app/Contents/Versions/*/include
    $ENV{ProgramFiles}/PostgreSQL/*/include
    $ENV{SystemDrive}/PostgreSQL/*/include
    /usr/include/postgresql
    /usr/local/include/postgresql
    /usr/local/postgresql/include
    /usr/local/postgresql/*/include
    /opt/postgresql/include
    /opt/postgresql/*/include)

#
# NO_DEFAULT_PATH is needed for Mac OSX with the Postgres.app
# but it is not needed for Linux.  In fact for Linux the lib is in an
# odd system directory so the default paths must be searched
#
# On Windows x64 CMake (32-bit) can't seem to find PostgreSQL directories
# $ENV{ProgramFiles} under C:\Program Files
#

find_library(POSTGRESQL_LIB
             NAMES pq
             PATHS
             $ENV{POSTGRESQL_LIB_DIR}
             $ENV{POSTGRESQL_LIB_DIR}/lib
             /Applications/Postgres.app/Contents/Versions/*/lib
             $ENV{ProgramFiles}/PostgreSQL/*/lib
             $ENV{SystemDrive}/PostgreSQL/*/lib
             /usr/lib/postgresql
             /usr/lib/postgresql/*
             /usr/lib/postgresql/*/lib
             /usr/local/lib/postgresql
             /usr/local/lib/postgresql/*
             /usr/local/lib/postgresql/*/lib
             /usr/local/postgresql/lib
             /usr/local/postgresql/*/lib
             /opt/postgresql/lib
             /opt/postgresql/*/lib)
#             NO_DEFAULT_PATH)

if (POSTGRESQL_LIB)
    get_filename_component(POSTGRESQL_LIB_DIR ${POSTGRESQL_LIB} PATH)
endif (POSTGRESQL_LIB)

if (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIB_DIR)
    set(POSTGRESQL_FOUND TRUE)
    message(STATUS "PostgreSQL Include directory: ${POSTGRESQL_INCLUDE_DIR}  Library directory: ${POSTGRESQL_LIB_DIR}")
    include_directories(${POSTGRESQL_INCLUDE_DIR})
    link_directories(${POSTGRESQL_LIB_DIR})
else (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIB_DIR)
    message(STATUS "Couldn't find PostgreSQL")
endif (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIB_DIR)
