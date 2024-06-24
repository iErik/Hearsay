REPL_DEPS := \
	$(OBJ.D)/lexer.o \
	$(OBJ.D)/tokens.o \
	$(SRC.D)/repl/repl.c \
	$(INCL.D)/repl/repl.h \

# -- Rules:

$(OBJ.D)/repl.o: $(REPL_DEPS)
	${CCMD} -o $@ -c $(SRC.D)/repl/repl.c

