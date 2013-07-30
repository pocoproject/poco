#
# ODBC.make
#
# $Id: //poco/1.4/Data/ODBC/ODBC.make#2 $
#
# Makefile fragment for finding ODBC library
#

ifndef ODBCINCDIR
ODBCINCDIR = /usr/include
endif

ifndef ODBCLIBDIR
ifeq (0, $(shell test -d /usr/lib/$(OSARCH)-linux-gnu; echo $$?))
ODBCLIBDIR = /usr/lib/$(OSARCH)-linux-gnu
else ifeq (0, $(shell test -d /usr/lib64; echo $$?))
ODBCLIBDIR = /usr/lib64
else 
ODBCLIBDIR = /usr/lib
endif
endif

ifeq ($(LINKMODE),STATIC)
LIBLINKEXT = .a
else ifeq ($(POCO_CONFIG),CYGWIN)
LIBLINKEXT = .dll.a
else
LIBLINKEXT = $(SHAREDLIBLINKEXT)
endif

INCLUDE += -I$(ODBCINCDIR)
SYSLIBS += -L$(ODBCLIBDIR)

ifeq ($(POCO_CONFIG),MinGW)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX
else ifeq (0, $(shell test -e $(ODBCLIBDIR)/libodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbc
COMMONFLAGS += -DPOCO_UNIXODBC
else ifeq (0, $(shell test -e $(ODBCLIBDIR)/libiodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC -I/usr/include/iodbc
else
$(error No ODBC library found. Please install unixODBC or iODBC or specify ODBCLIBDIR and try again)
endif
