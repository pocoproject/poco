#
# Makefile
#
# Makefile for Poco AI MCP HTTP (Streamable HTTP client and server transports)
#

include $(POCO_BASE)/build/rules/global

objects = SessionFactory Body EventStreamReader Headers HTTPClient \
	ServerTransport ServerRequestHandler

target         = PocoAIMCPHTTP
target_version = $(LIBVERSION)
target_libs    = PocoAIMCP PocoNet PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/lib
