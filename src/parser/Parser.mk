PARSER_DEPS := \
	$(SRC.D)/parser/parser.c \
	$(INCL.D)/parser/parser.h \
	$(OBJ.D)/nodes.o \
	$(OBJ.D)/nodeList.o \
	$(OBJ.D)/array.o \
	$(OBJ.D)/text.o \
	$(OBJ.D)/mem.o

NODES_DEPS := \
	$(SRC.D)/parser/nodes.c \
	$(INCL.D)/parser/nodes.h \
	$(OBJ.D)/mem.o

NODELIST_DEPS := \
	$(SRC.D)/parser/nodeList.c \
	$(OBJ.D)/mem.o

# -- Rules:

$(OBJ.D)/parser.o: $(PARSER_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/parser/parser.c

$(OBJ.D)/nodes.o: $(NODES_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/parser/nodes.c

$(OBJ.D)/nodeList.o: $(NODESLIST_DEPS)
	$(CCMD) -o $@ -c $(SRC.D)/parser/nodeList.c
