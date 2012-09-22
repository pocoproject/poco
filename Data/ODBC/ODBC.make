#
# ODBC.make
#
# $Id: //poco/1.4/Data/ODBC/ODBC.make#1 $
#
# Makefile fragment for finding ODBC library
#

ifndef POCO_ODBC_LIB_DIR
POCO_ODBC_LIB_DIR = /usr/lib
endif

ifeq ($(LINKMODE),STATIC)
LIBLINKEXT = .a
else
LIBLINKEXT = $(SHAREDLIBLINKEXT)
endif

SYSLIBS += -L$(POCO_ODBC_LIB_DIR)

###########
# MinGW   #
###########
ifeq ($(POCO_CONFIG),MinGW)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX

###########
# Cygwin  #
###########
else ifeq ($(POCO_CONFIG),CYGWIN)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX
# CYGWIN platform has its own ODBC library in /lib/w32api 
SYSLIBS += -L/lib/w32api -lodbc32 -lodbccp32

############
# unixODBC #
############
else ifeq (0, $(shell test -e $(POCO_ODBC_LIB_DIR)/libodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC

############
# iODBC    #
############
else ifeq (0, $(shell test -e $(POCO_ODBC_LIB_DIR)/libiodbc$(LIBLINKEXT); echo$$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC

else
$(error No ODBC library found in $(POCO_ODBC_LIB_DIR). Please install unixODBC or iODBC and try again.)
endif
