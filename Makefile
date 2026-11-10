# NUMERIX C11 build. Works with GNU Make, GCC or Clang, and MinGW GCC.

CC = gcc
AR = ar
ARFLAGS = rcs
CPPFLAGS ?= -Iinclude
CPPFLAGS += -Iplatform
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
LDLIBS ?= -lm

ifeq ($(OS),Windows_NT)
EXE := .exe
PLATFORM_SOURCE := platform/windows/timing.c
define MAKE_DIR
if not exist "$(subst /,\\,$(1))" mkdir "$(subst /,\\,$(1))"
endef
define RUN_TESTS
for %%t in ($(subst /,\\,$(TEST_BINS))) do @%%t || exit /b 1
endef
define RUN_ONE
$(subst /,\\,$(1))
endef
else
EXE :=
PLATFORM_SOURCE := platform/linux/timing.c
define MAKE_DIR
mkdir -p "$(1)"
endef
define RUN_TESTS
set -e; for test in $(TEST_BINS); do "./$$test"; done
endef
define RUN_ONE
./$(1)
endef
endif

BUILD_DIR := build
BIN_DIR := bin
LIB_DIR := lib
STATIC_LIB := $(LIB_DIR)/libnumerix.a
ALLOCATION_TEST_BIN := $(BIN_DIR)/test_allocation_failure$(EXE)

LIB_SOURCES := $(wildcard src/*.c src/algorithms/*.c src/core/*.c \
	               src/diagnostics/*.c src/execution/*.c src/expression/*.c \
	               src/intelligence/*.c src/piecewise/*.c src/benchmarking/*.c \
	               src/testing/*.c) $(PLATFORM_SOURCE)
LIB_SOURCES += src/cli/cli.c
LIB_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIB_SOURCES))
TEST_SOURCES := $(filter-out tests/test_allocation_failure.c,$(wildcard tests/test_*.c))
TEST_BINS := $(patsubst tests/test_%.c,$(BIN_DIR)/test_%$(EXE),$(TEST_SOURCES))
EXAMPLE_SOURCES := $(wildcard examples/demo_*.c)
EXAMPLE_BINS := $(patsubst examples/%.c,$(BIN_DIR)/%$(EXE),$(EXAMPLE_SOURCES))
CLI_BIN := $(BIN_DIR)/numerix_cli$(EXE)

.PHONY: all library demo cli test test-memory clean

all: library demo cli $(TEST_BINS)

library: $(STATIC_LIB)
demo: $(EXAMPLE_BINS)
cli: $(CLI_BIN)

$(STATIC_LIB): $(LIB_OBJECTS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: %.c
	@$(call MAKE_DIR,$(dir $@))
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%$(EXE): examples/%.c $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(BIN_DIR)/test_%$(EXE): tests/test_%.c $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(CLI_BIN): src/cli/main.c $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(BUILD_DIR) $(BIN_DIR) $(LIB_DIR):
	@$(call MAKE_DIR,$@)

test: $(TEST_BINS) $(ALLOCATION_TEST_BIN)
	@$(RUN_TESTS)
	@$(call RUN_ONE,$(ALLOCATION_TEST_BIN))

$(BUILD_DIR)/interpolation_allocfail.o: src/interpolation.c include/numerical_recipes.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Dmalloc=nr_test_malloc -c $< -o $@

$(ALLOCATION_TEST_BIN): tests/test_allocation_failure.c $(BUILD_DIR)/interpolation_allocfail.o | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDLIBS) -o $@

test-memory: $(BIN_DIR)/test_allocation_failure$(EXE)
	@$(call RUN_ONE,$(ALLOCATION_TEST_BIN))

ifeq ($(OS),Windows_NT)
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
	@if exist "$(LIB_DIR)\libnumerix.a" del /q "$(LIB_DIR)\libnumerix.a"
	@if exist "$(LIB_DIR)\libnumerical_recipes.a" del /q "$(LIB_DIR)\libnumerical_recipes.a"
else
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f $(LIB_DIR)/libnumerix.a $(LIB_DIR)/libnumerical_recipes.a
	rmdir $(LIB_DIR) 2>/dev/null || true
endif
