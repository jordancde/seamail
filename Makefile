NAME :=

OBJECTS :=

TEST_DIR := tests
FLAGS	:= -std=c++14 -MMD
DEV_FLAGS := -Wall -Werror -g

C_NONE	:= \x1b[0m
C_OK	:= \x1b[32;01m
C_ERROR	:= \x1b[31;01m
C_WARN	:= \x1b[33;01m

S_OK	:= $(C_OK)[OK]$(C_NONE)
S_ERROR := $(C_ERROR)[ERROR]$(C_NONE)
S_WARN	:= $(C_WARN)[WARNING]$(C_NONE)

SHELL	:= /bin/bash

CXXFLAGS  := $(FLAGS)

ifeq ($(TEST),1)
	TEST_BIN := $(NAME)_test
	CXXFLAGS += $(DEV_FLAGS)
else
	TEST_BIN := $(NAME)
endif

SAMP_BIN := $(NAME)

CXX 	:= g++
DEPENDS := $(OBJECTS:.o=.d)

.PHONY: test clean all

all: $(TEST_BIN)

$(TEST_BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

-include $(DEPENDS)

clean:
	rm -f $(TEST_BIN)
	rm -f $(TEST_DIR)/*.{out,vg}
	rm -f {*.d,*.o}

test: $(TEST_BIN)
ifeq ($(TEST),1)
	@fail=0; \
	test_fail=0; \
	cmp $(SAMP_BIN) $(TEST_BIN); \
	if [ $$? -eq 0 ]; then \
		echo -e "$(S_WARN) the sample and test executable are identical!"; \
		exit 1; \
	fi; \
	for test in $(TEST_DIR)/*.in; \
	do \
		test_fail=0; \
		valgrind --error-exitcode=1 --log-fd=9 --leak-check=full 9> $$test.samp.vg ./$(SAMP_BIN) < $$test > $$test.samp.out; \
		samp_err=$$?; \
		valgrind --error-exitcode=1 --log-fd=9 --leak-check=full 9> $$test.test.vg ./$(TEST_BIN) < $$test > $$test.test.out; \
		test_err=$$?; \
		if [ $$samp_err -ne $$test_err ]; then \
			echo -e "$(S_ERROR) $$test causes memory error on only one of samp/test!"; \
			diff --color=always $$test.test.vg $$test.samp.vg; \
			test_fail=1; \
		elif [ $$test_err -ne 0 ]; then \
		    echo -e "$(S_WARN) $$test causes memory error for both samp and test! (skipping)"; \
		    test_fail=0; \
		fi; \
		./$(SAMP_BIN) < $$test > $$test.samp.out; \
		diff --color=always $$test.test.out $$test.samp.out; \
		if ! [ $$? -eq 0 ]; then \
		    echo -e "$(S_ERROR) $$test.samp.out and $$test.test.out differ!";\
		    test_fail=1; \
		else \
		    echo -e "$(S_OK) $$test.samp.out and $$test.test.out match!";\
		fi; \
		if [ $$test_fail -eq 0 ]; then \
		    echo -e "$(S_OK) test $$test passed!";\
		else \
		    fail=1; \
		fi; \
	done; \
	if [ $$fail -ne 0 ]; then \
	    echo -e "$(S_ERROR) at least one test failed!"; \
	    exit 1; \
	fi; \
	echo -e "$(S_OK) all tests passed!";
else
	@echo "Cannot run local tests in Marmoset mode!"
endif
