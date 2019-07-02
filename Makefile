BIN 		:= seamail

FLAGS		:= -Wall -std=c++17 -MMD 
DEV_FLAGS 	:= -g

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
