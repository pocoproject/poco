#
# Makefile
#
# $Id$
#
# Makefile for Poco JSON
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/JSON/include/Poco/JSON

POCO_JSON_PARSER := $(if $(POCO_JSON_PARSER),$(POCO_JSON_PARSER),PD_JSON_PARSER)

objects = Array Object Parser ParserImpl Handler \
	Stringifier ParseHandler PrintHandler Query \
	JSONException Template TemplateCache

ifeq ($(POCO_JSON_PARSER),PD_JSON_PARSER)
	objects += pd_json
endif

target         = PocoJSON
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
