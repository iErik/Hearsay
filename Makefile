SRC.D   := src
OBJ.D   := objects
INCL.D  := include
BUILD.D := build

T-OUT.D := $(BUILD.D)/tests
TESTS.D := tests

BASE_FLAGS := -iquote $(INCL.D)
BUILD_FLAGS := $(BASE_FLAGS) -Wall -03
DEBUG_FLAGS := $(BASE_FLAGS) -g -w -O0 -D_FORTIFY_SOURCE=0

CC      := gcc
CCMD    := ${CC} ${DEBUG_FLAGS}
TCMD    := ${CC} ${DEBUG_FLAGS}

all:
	echo "All :D"

.PHONY: clean
clean:
	rm -rf ${BUILD.D}/*
	rm -rf ${OBJ.D}

.PHONY: mkdirs
mkdirs:
	@mkdir -p ${SRC.D} ${OBJ.D} ${BUILD.D} ${OBJ.D} ${T-OUT.D}

include src/lexer/Lexer.mk
include src/parser/Parser.mk
include src/repl/Repl.mk
include src/utils/Utils.mk
include tests/Tests.mk
