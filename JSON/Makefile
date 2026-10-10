#
# Makefile
#
# Makefile for Poco JSON
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/JSON/include/Poco/JSON

objects = Array Object Parser ParserImpl Handler \
	Stringifier ParseHandler PrintHandler Query \
	JSONException Template TemplateCache pdjson

# poco build system looks for sources in src/
ifdef POCO_UNBUNDLED
	SYSLIBS += -lpdjson
else
	objects += pdjson
	prebuild = $(shell \
		ln -sf $(POCO_BASE)/dependencies/pdjson/src/pdjson.c  src/ && \
		ln -sf $(POCO_BASE)/dependencies/pdjson/src/pdjson.h  src/ \
	)
	INCLUDE += -I$(POCO_BASE)/dependencies/pdjson/src
endif

target         = PocoJSON
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
