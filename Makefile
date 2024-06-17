SRC.D := src
OBJ.D := objects
TEST.D := tests
BUILD.D := build
TEST.OD := ${BUILD.D}/tests
INCL.D := include

CC := gcc
CFLAGS := -Wall -g -iquote ${INCL.D}
CDFLAGS := -g -iquote ${INCL.D} -O0
CCMD := ${CC} ${CFLAGS}
TCMD := ${CC} ${CDFLAGS}

all:
	echo "All :D"

parser.o: nodes.o ${SRC.D}/parser/parser.c ${INCL.D}/parser/parser.h
	${CCMD} -c ${SRC.D}/parser/parser.c \
		-o ${OBJ.D}/parser.o

nodes.o: ${SRC.D}/parser/nodes.c ${INCL.D}/parser/nodes.h
	${CCMD} -c ${SRC.D}/parser/nodes.c \
		-o ${OBJ.D}/nodes.o

repl.o: lexer.o ${SRC.D}/repl.c ${INCL.D}/repl.h
	${CCMD} -c ${SRC.D}/repl.c -o ${OBJ.D}/repl.o

lexer.o: tokens.o ${SRC.D}/lexer.c ${INCL.D}/lexer.h
	${CCMD} -c ${SRC.D}/lexer.c -o ${OBJ.D}/lexer.o

tokens.o: ${SRC.D}/tokens.c ${INCL.D}/tokens.h
	${CCMD} -c ${SRC.D}/tokens.c -o ${OBJ.D}/tokens.o


.PHONY: test-parser
test-parser: parser.o nodes.o lexer.o ${INCL.D}/test-utils.h
	@${TCMD} ${OBJ.D}/parser.o \
		${OBJ.D}/nodes.o \
		${OBJ.D}/tokens.o \
		${OBJ.D}/lexer.o \
		${SRC.D}/utils.c \
		${TEST.D}/test-utils.c \
		${TEST.D}/parser.c \
		-o ${TEST.OD}/parser

	@chmod +x ${TEST.OD}/parser
	@./${TEST.OD}/parser

.PHONY: test-nodes
test-nodes: nodes.o ${INCL.D}/test-utils.h
	@${CCMD} ${OBJ.D}/nodes.o ${OBJ.D}/tokens.o \
		${TEST.D}/test-utils.c \
		${TEST.D}/nodes.c \
		-o ${TEST.OD}/nodes

	@chmod +x ${TEST.OD}/nodes
	@./${TEST.OD}/nodes

.PHONY: test-token
test-token: tokens.o ${INCL.D}/test-utils.h
	@${CCMD} ${OBJ.D}/tokens.o  \
		${TEST.D}/test-utils.c \
		${TEST.D}/token.c \
		-o ${TEST.OD}/tokens

	@chmod +x ${TEST.OD}/tokens
	@./${TEST.OD}/tokens

.PHONY: test-repl
test-repl: repl.o lexer.o ${INCL.D}/test-utils.h
	@${CCMD} \
		${OBJ.D}/repl.o ${OBJ.D}/lexer.o ${OBJ.D}/tokens.o \
		${TEST.D}/test-utils.h \
		${TEST.D}/repl.c \
		-o ${TEST.OD}/repl

	@chmod +x ${TEST.OD}/repl
	@./${TEST.OD}/repl

.PHONY: test-lexer ${INCL.D}/test-utils.h
test-lexer: lexer.o
	@${CCMD} ${OBJ.D}/tokens.o \
		${TEST.D}/test-utils.h \
		${TEST.D}/lexer.c \
		-o ${TEST.OD}/lexer

	@chmod +x ${TEST.OD}/lexer
	@./${TEST.OD}/lexer:w


.PHONY: clean
clean:
	rm -rf ${BUILD.D}/*
	rm -rf ${OBJ.D}

.PHONY: mkdirs
mkdirs:
	@mkdir -p ${SRC.D} ${OBJ.D} ${BUILD.D} ${TEST.OD}
