LEXER_DEPS := \
	$(SRC.D)/lexer/lexer.c \
	$(INCL.D)/lexer/lexer.h \
	$(OBJ.D)/tokens.o

TOKENS_DEPS := \
	$(SRC.D)/lexer/tokens.c \
	$(INCL.D)/lexer/tokens.h

# -- Rules:

$(OBJ.D)/lexer.o: $(LEXER_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/lexer/lexer.c

$(OBJ.D)/tokens.o: $(TOKENS_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/lexer/tokens.c

