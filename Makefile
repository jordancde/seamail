BIN 		:= seamail

FLAGS		:= -Wall -std=c++17 -MMD 
DEV_FLAGS 	:= -g
LIBS		:= -lncurses 

SDIR 		:= src

CXX 		:= g++
CXXFLAGS  	:= $(FLAGS) -I$(SDIR)

SOURCES 	:= $(shell find $(SDIR) -name '*.cc')
OBJECTS		:= $(SOURCES:$(SDIR)/%.cc=$(SDIR)/%.o)
DEPENDS 	:= $(OBJECTS:.o=.d)

SHELL		:= /bin/bash

.PHONY: clean all run

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

-include $(DEPENDS)

clean:
	rm -rf $(BIN)
	find $(SDIR) -name '*.o' -delete
	find $(SDIR) -name '*.d' -delete

run: all
	./$(BIN)
