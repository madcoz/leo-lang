# Copyright (c) 2015, Alister Chan. All rights reserved.
# See the README file for license conditions and contact info.

# makefile for leo boostrap compiler

CXX_SRC := $(sort \
         $(shell find . -name '*.cc'))
FLEX_SRC := ./flex
BISON_SRC := ./bison
BIN_DIR :=./bin
BISON := bison
FLEX := flex
CXX_OBJ := $(patsubst %.cc,%.o,$(CXX_SRC))
CXX_DEP := $(patsubst %.cc,%.d,$(CXX_SRC))
CXXFLAGS += -std=gnu++11
CPPFLAGS += -MD -MP
CC := $(CXX)
CFLAGS += $(CXXFLAGS)
RM := rm -f

.PHONY: all
all: $(BIN_DIR) $(BIN_DIR)/lc

$(BIN_DIR):
	mkdir -p $@

$(BIN_DIR)/lc: $(BISON_SRC)/parser.tab.o $(FLEX_SRC)/lex.o $(CXX_OBJ)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@	
	
$(FLEX_SRC)/lex.c: $(FLEX_SRC)/lex.l
	$(FLEX) -t $< > $@

$(BISON_SRC)/parser.tab.cc: $(BISON_SRC)/parser.y
	$(BISON) -o $@ $<

.PHONY: clean
clean:
	$(RM) -r $(BIN_DIR)
    
-include $(CXX_DEP)