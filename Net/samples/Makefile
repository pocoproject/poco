#
# Makefile
#
# Makefile for Poco Net Samples
#

.PHONY: projects
clean distclean all: projects
projects:
	$(MAKE) -C dict $(MAKECMDGOALS)
	$(MAKE) -C TimeServer $(MAKECMDGOALS)
	$(MAKE) -C httpget $(MAKECMDGOALS)
	$(MAKE) -C HTTPTimeServer $(MAKECMDGOALS)
	$(MAKE) -C HTTPFormServer $(MAKECMDGOALS)
	$(MAKE) -C HTTPLoadTest $(MAKECMDGOALS)
	$(MAKE) -C download $(MAKECMDGOALS)
	$(MAKE) -C EchoServer $(MAKECMDGOALS)
	$(MAKE) -C Mail $(MAKECMDGOALS)
	$(MAKE) -C Ping $(MAKECMDGOALS)
	$(MAKE) -C WebSocketServer $(MAKECMDGOALS)
	$(MAKE) -C SMTPLogger $(MAKECMDGOALS)
	$(MAKE) -C ifconfig $(MAKECMDGOALS)
	$(MAKE) -C tcpserver $(MAKECMDGOALS)
