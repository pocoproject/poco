#
# MySQL.make
#
# Makefile fragment for finding MySQL library
#

MYSQL_CONFIG = $(shell which mysql_config)
ifneq (, $MYSQL_CONFIG)
  POCO_MYSQL_INCLUDE = $(shell mysql_config --include)
  POCO_MYSQL_LIB = $(shell mysql_config --libs)
else
  ifndef POCO_MYSQL_INCLUDE
    ifeq (0, $(shell test -d /usr/local/include/mysql; echo $$?))
      POCO_MYSQL_INCLUDE = -I/usr/local/include
    else
      ifeq (0, $(shell test -d /usr/local/opt/mysql-client/include; echo $$?))
        POCO_MYSQL_INCLUDE = -I/usr/local/opt/mysql-client/include
      else
        ifeq (0, $(shell test -d /opt/homebrew/opt/mysql-client/include; echo $$?))
          POCO_MYSQL_INCLUDE = -I/opt/homebrew/opt/mysql-client/include
        endif
      endif
    endif
  endif

  ifndef POCO_MYSQL_LIB
    ifeq (0, $(shell test -d /usr/local/include/mysql; echo $$?))
      POCO_MYSQL_LIB = -L/usr/local/lib
    else
      ifeq (0, $(shell test -d /usr/local/opt/mysql-client/lib; echo $$?))
        POCO_MYSQL_LIB = -L/usr/local/opt/mysql-client/lib
      else
        ifeq (0, $(shell test -d /opt/homebrew/opt/mysql-client/lib; echo $$?))
          POCO_MYSQL_LIB = -L/opt/homebrew/opt/mysql-client/lib
        endif
      endif
    endif
  endif
endif

ifdef POCO_MYSQL_INCLUDE
  INCLUDE += $(POCO_MYSQL_INCLUDE)
endif
ifdef POCO_MYSQL_LIB
  SYSLIBS += $(POCO_MYSQL_LIB)
endif
