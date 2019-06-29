BIN := seamail

SDIR := src

FLAGS	:= -Wall -std=c++14 -MMD
DEV_FLAGS := -g
LIB := -Lm 
INC := -I$(IDIR)

SHELL	:= /bin/bash

CXX 	:= g++
CXXFLAGS  := $(FLAGS) $(LIB) $(INC)

SOURCES := $(shell find $(SDIR) -name '*.cc')
OBJECTS	:= $(SOURCES:$(SDIR)/%.cc=%.o)
DEPENDS := $(OBJECTS:.o=.d)

.PHONY: clean all

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

-include $(DEPENDS)

clean:
	rm -rf $(BIN)
	find $(SDIR) -name '*.o' -delete
	find $(SDIR) -name '*.d' -delete

