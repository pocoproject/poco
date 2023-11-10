#
# ODBC.make
#
# Makefile fragment for finding ODBC library
#

ifndef POCO_ODBC_INCLUDE
ifeq (0, $(shell test -e /usr/include/sql.h; echo $$?))
POCO_ODBC_INCLUDE = /usr/include
else ifeq (0, $(shell test -e /opt/homebrew/include; echo $$?))
POCO_ODBC_INCLUDE = /opt/homebrew/include
endif
endif

ifndef POCO_ODBC_LIB
ifeq (0, $(shell test -d /usr/lib/$(OSARCH)-linux-gnu; echo $$?))
POCO_ODBC_LIB = /usr/lib/$(OSARCH)-linux-gnu
else ifeq (0, $(shell test -d /usr/lib64; echo $$?))
POCO_ODBC_LIB = /usr/lib64
else ifeq (0, $(shell test -d /opt/homebrew/lib; echo $$?))
POCO_ODBC_LIB = /opt/homebrew/lib
else
POCO_ODBC_LIB = /usr/lib
endif
endif

ifeq ($(LINKMODE),STATIC)
LIBLINKEXT = .a
else
ifeq ($(OSNAME), CYGWIN)
LIBLINKEXT = $(IMPLIBLINKEXT)
else
LIBLINKEXT = $(SHAREDLIBLINKEXT)
endif
endif

INCLUDE += -I$(POCO_ODBC_INCLUDE)
SYSLIBS += -L$(POCO_ODBC_LIB)

##
## MinGW
##
ifeq ($(POCO_CONFIG),MinGW)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX

##
## unixODBC
##
else ifeq (0, $(shell test -e $(POCO_ODBC_LIB)/libodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbc
ifeq (0, $(shell test -e $(POCO_ODBC_LIB)/libodbcinst$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbcinst
endif
COMMONFLAGS += -DPOCO_UNIXODBC

##
## iODBC
##
else ifeq (0, $(shell test -e $(POCO_ODBC_LIB)/libiodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC -I/usr/include/iodbc

else
$(error No ODBC library found. Please install unixODBC or iODBC or specify POCO_ODBC_LIB and try again)
endif

