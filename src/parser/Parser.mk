PARSER_DEPS := \
	$(SRC.D)/parser/parser.c \
	$(INCL.D)/parser/parser.h \
	$(OBJ.D)/nodes.o \
	$(OBJ.D)/array.o \
	$(OBJ.D)/text.o \

NODES_DEPS := \
	$(SRC.D)/parser/nodes.c \
	$(INCL.D)/parser/nodes.h

# -- Rules:

$(OBJ.D)/parser.o: $(PARSER_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/parser/parser.c

$(OBJ.D)/nodes.o: $(NODES_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/parser/nodes.c
