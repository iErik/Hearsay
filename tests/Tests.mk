UTILS.H := \
	$(INCL.D)/utils/array.h \
	$(INCL.D)/utils/text.h
UTILS.O := \
	$(OBJ.D)/array.o \
	$(OBJ.D)/text.o

LEXER.H := \
	$(INCL.D)/lexer/lexer.h \
	$(INCL.D)/lexer/tokens.h
LEXER.O := \
	$(OBJ.D)/lexer.o \
	$(OBJ.D)/tokens.o

PARSER.H := \
	$(INCL.D)/parser/nodes.h \
	$(INCL.D)/parser/parser.h
PARSER.O := \
	$(OBJ.D)/parser.o \
	$(OBJ.D)/nodes.o

REPL.O := \
	$(OBJ.D)/repl.o

COMMON_DEPS := \
	$(UTILS.O) \
	$(TESTS.D)/test-utils.c


.PHONY: test-repl
test-repl: $(T-OUT.D)/repl
	@./$<


.PHONY: test-parser
test-parser: $(T-OUT.D)/parser
	@./$<

.PHONY: test-nodes
test-nodes: $(T-OUT.D)/nodes
	@./$<


.PHONY: test-lexer
test-lexer: $(T-OUT.D)/lexer
	@./$<

.PHONY: test-tokens
test-tokens: $(T-OUT.D)/tokens
	@./$<


.PHONY: test-utils
test-utils: $(T-OUT.D)/utils
	@./$<


REPL_DEPS := \
	$(LEXER.O) \
	$(PARSER.O) \
	$(REPL.O) \
	$(TESTS.D)/repl.c
$(T-OUT.D)/repl: $(COMMON_DEPS) $(REPL_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@


PARSER_DEPS := \
	$(PARSER.O) \
	$(LEXER.O) \
	$(TESTS.D)/parser.c
$(T-OUT.D)/parser: $(COMMON_DEPS) $(PARSER_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@

NODES_DEPS := \
	$(OBJ.D)/nodes.o \
	$(TESTS.D)/nodes.c
$(T-OUT.D)/nodes: $(COMMON_DEPS) $(NODES_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@


LEXER_DEPS := \
	$(LEXER.O) \
	$(TESTS.D)/lexer.c
$(T-OUT.D)/lexer: $(COMMON_DEPS) $(LEXER_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@

TOKENS_DEPS := \
	$(OBJ.D)/tokens.o \
	$(TESTS.D)/tokens.c
$(T-OUT.D)/tokens: $(COMMON_DEPS) $(TOKENS_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@


UTILS_DEPS := \
	$(OBJ.D)/array.o \
	$(TESTS.D)/utils/array.c \
	$(TESTS.D)/utils/main.c
$(T-OUT.D)/utils: $(COMMON_DEPS) $(UTILS_DEPS)
	@$(TCMD) -o $@ $^
	@chmod +x $@
