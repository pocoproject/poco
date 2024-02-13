# bison's version is too old on OSX, allow user to pass in custom path
BISON?=bison
FLEX?=flex

OS_TYPE=$(shell uname)
ifeq ($(OS_TYPE), Darwin)
BREW_PREFIX=$(shell brew --prefix)
BREW_INSTALLED=$(shell echo $(BREW_PREFIX) | wc -w | xargs)
ifeq ($(BREW_INSTALLED), 0)
$(error On macOS, Homebrew (see https://brew.sh) is required to install recent Bison and Flex versions)
endif
endif

BISON_VERSION=$(shell $(BISON) --version | head -n 1 | grep -o '[0-9]\.[0-9]\+')
BISON_VERSION_SUPPORTED=$(shell awk -v a=$(BISON_VERSION) -v b="3.0" 'BEGIN { print (a >= b) ? 1 : 0 }')
ifneq ($(BISON_VERSION_SUPPORTED), 1)
$(error Bison version $(BISON_VERSION) not supported. If you are using macOS, `bison` uses the system default instead of the brew version. Run BISON=$(BREW_PREFIX)/opt/bison/bin/bison make)
endif

FLEX_VERSION=$(shell $(FLEX) --version | head -n 1 | grep -o '[0-9]\.[0-9]\+')
FLEX_VERSION_SUPPORTED=$(shell awk -v a=$(FLEX_VERSION) -v b="2.6" 'BEGIN { print (a >= b) ? 1 : 0 }')
ifneq ($(FLEX_VERSION_SUPPORTED), 1)
$(error Flex version $(FLEX_VERSION) not supported. If you are using macOS, `flex` uses the system default instead of the brew version. Run FLEX=$(BREW_PREFIX)/opt/flex/bin/flex make)
endif

all: bison_parser.cpp flex_lexer.cpp

bison_parser.cpp: bison_parser.y
	$(BISON) bison_parser.y --output=bison_parser.cpp --defines=bison_parser.h --verbose

flex_lexer.cpp: flex_lexer.l
	! $(FLEX) flex_lexer.l 2>&1 | grep "warning"

clean:
	rm -f bison_parser.cpp flex_lexer.cpp bison_parser.h flex_lexer.h *.output

# Tests if the parser builds correctly and doesn't contain conflicts.
test:
	! $(BISON) bison_parser.y -v --output=conflict_test.cpp 2>&1 | grep "conflict" >&2
