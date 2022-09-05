#
# MySQL.make
#
# Makefile fragment for finding MySQL library
#


# Note: linking order is important, do not change it.

ifdef POCO_MYSQL_INCLUDE
INCLUDE += -I$(POCO_MYSQL_INCLUDE)
else
INCLUDE += -I./../include -I/usr/local/include/mysql -I/usr/include/mysql/ -I/usr/mysql/include/mysql -I/usr/local/mysql/include -I/usr/local/opt/mysql-client/include -I/usr/local/opt/mysql-client/include/mysql  
endif

ifdef POCO_MYSQL_LIB
SYSLIBS += -L$(POCO_MYSQL_LIB)
else
SYSLIBS += -L/usr/local/lib -L/usr/local/lib$(LIB64SUFFIX)/mysql -L/usr/lib$(LIB64SUFFIX)/mysql -L/usr/mysql/lib$(LIB64SUFFIX) -L/usr/mysql/lib$(LIB64SUFFIX)/mysql -L/usr/local/mysql/lib$(LIB64SUFFIX) -L/usr/local/opt/mysql-client/lib
endif

# Note: linking order is important, do not change it.
SYSLIBS += -lmysqlclient -lz -lpthread -ldl
