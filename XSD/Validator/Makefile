#
# Makefile
#
# Makefile for Poco XSDValidator
#

include $(POCO_BASE)/build/rules/global

objects = Validator

target         = PocoXSDValidator
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

# libxml2 powers XSD schema validation (Poco's XML module is DOM/SAX only).
# Sourced as an external dependency - see platform/dependencies/libxml2/.
# Supported setups: system package (apt/dnf/brew), nix-shell, or vcpkg on
# Windows (see vcpkg.json). pkg-config is used to locate headers and link
# flags portably across all of them.
INCLUDE       += $(shell pkg-config --cflags libxml-2.0 2>/dev/null || echo -I/usr/include/libxml2)
SYSLIBS       += $(shell pkg-config --libs libxml-2.0 2>/dev/null || echo -lxml2)

include $(POCO_BASE)/build/rules/lib
