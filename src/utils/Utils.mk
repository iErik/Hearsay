ARRAY_DEPS := \
	$(SRC.D)/utils/array.c \
	$(INCL.D)/utils/array.h \
	$(INCL.D)/common.h

TEXT_DEPS := \
	$(SRC.D)/utils/text.c \
	$(INCL.D)/utils/text.h \
	$(INCL.D)/common.h


$(OBJ.D)/array.o: $(ARRAY_DEPS)
	@$(CCMD) -o $@ -c $(SRC.D)/utils/array.c

$(OBJ.D)/text.o: $(TEXT_DEPS)
	@$(CCMD) -o $@ -c $(SRC.D)/utils/text.c
