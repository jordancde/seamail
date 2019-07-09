BIN 		:= seamail

DEV_FLAGS 	:= -g
FLAGS		:= -Wall -std=c++17 -MMD $(DEV_FLAGS)

SDIR 		:= src
DDIR		:= dep

INC		:= -I$(DDIR)/json
LIBS		:= -lncurses 

CXX 		:= g++
CXXFLAGS  	:= $(FLAGS) -I$(SDIR) $(INC)

SOURCES 	:= $(shell find $(SDIR) -name '*.cc')
#SOURCES 	:= src/main.cc $(shell find $(SDIR)/graphics -name '*.cc')

OBJECTS		:= $(SOURCES:$(SDIR)/%.cc=$(SDIR)/%.o)
DEPENDS 	:= $(OBJECTS:.o=.d)

SHELL		:= /bin/bash

# TEST SPECIFIC DEFS
MAIN_FILE	:= $(SDIR)/main.cc
TEST_SOURCES	= $(filter-out $(MAIN_FILE),$(SOURCES)) $(TEST_FILE)
TEST_OBJECTS	= $(TEST_SOURCES:%.cc=%.o)

C_NONE	:= \x1b[0m
C_OK	:= \x1b[32;01m
C_ERROR	:= \x1b[31;01m
C_WARN	:= \x1b[33;01m

S_OK	:= $(C_OK)[OK]$(C_NONE)
S_ERROR := $(C_ERROR)[ERROR]$(C_NONE)
S_WARN	:= $(C_WARN)[WARNING]$(C_NONE)

.PHONY: clean all run force

all: $(BIN)
ifeq ($(TEST_MODE),1)
$(BIN): $(TEST_OBJECTS) force
	@$(CXX) $(TEST_OBJECTS) -o $@ $(LIBS)
	@valgrind --error-exitcode=1 --log-fd=9 --leak-check=full 9> $(TEST_FILE).vg \
		./$(BIN) > $(TEST_FILE).out; \
	if ! [ $$? -eq 0 ]; then \
		echo -e "$(S_ERROR) test $(TEST_FILE) failed!"; \
		cat $(TEST_FILE).vg $(TEST_FILE).out; \
	else \
		echo -e "$(S_OK) test $(TEST_FILE) passed!"; \
	fi;
else
$(BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
endif

-include $(DEPENDS)

clean:
	rm -rf $(BIN)
	find $(SDIR) -name '*.o' -delete -print
	find $(SDIR) -name '*.d' -delete -print

run: all
	./$(BIN)
