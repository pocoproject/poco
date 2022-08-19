#
# MySQL.make
#
# Makefile fragment for finding MySQL library
#


# Note: linking order is important, do not change it.

ifndef POCO_MYSQL_INCLUDE
POCO_MYSQL_INCLUDE= -I./../include -I/usr/local/include/mysql -I/usr/include/mysql/ -I/usr/mysql/include/mysql -I/usr/local/mysql/include
endif

ifndef POCO_MYSQL_LIB
POCO_MYSQL_LIB= -L/usr/local/lib -L/usr/local/lib$(LIB64SUFFIX)/mysql -L/usr/lib$(LIB64SUFFIX)/mysql -L/usr/mysql/lib$(LIB64SUFFIX) -L/usr/mysql/lib$(LIB64SUFFIX)/mysql -L/usr/local/mysql/lib$(LIB64SUFFIX)
endif

ifdef POCO_MYSQL_INCLUDE
INCLUDE += -I$(POCO_MYSQL_INCLUDE)
endif
ifdef POCO_MYSQL_LIB
SYSLIBS += -L$(POCO_MYSQL_LIB)
endif

# Note: linking order is important, do not change it.
SYSLIBS += -lmysqlclient -lz -lpthread -ldl
