# Production-grade userspace C build/test/debug helpers

CC      := gcc
CSTD    := -std=c11
WARN    := -Wall -Wextra -Wpedantic -Werror
OPT     := -O2
DBG     := -g3

INCS    := -Iinclude
DEFS    :=

BUILD   := build
APP     := taskmgr
TEST    := test_runner

SRC_APP := \
  src/main.c \
  src/cli.c \
  src/input.c \
  src/log.c \
  src/task_db.c \
  src/ds_queue.c \
  src/ds_stack.c \
  src/ds_list.c

SRC_TEST := \
  tests/test_runner.c \
  tests/test_queue.c \
  tests/test_stack.c \
  tests/test_list.c

OBJ_APP  := $(patsubst src/%.c,$(BUILD)/src/%.o,$(SRC_APP))
OBJ_TEST := $(patsubst tests/%.c,$(BUILD)/tests/%.o,$(SRC_TEST)) \
            $(filter-out $(BUILD)/src/main.o,$(OBJ_APP))

CPPFLAGS := $(CSTD) $(WARN) $(OPT) $(INCS) $(DEFS)

.PHONY: all clean run test sanitize debug dirs

all: dirs $(BUILD)/$(APP)

dirs:
	@mkdir -p $(BUILD)/src $(BUILD)/tests

$(BUILD)/src/%.o: src/%.c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD)/tests/%.o: tests/%.c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD)/$(APP): $(OBJ_APP)
	$(CC) $(CPPFLAGS) $^ -o $@

run: all
	$(BUILD)/$(APP)

test: dirs $(BUILD)/$(TEST)
	$(BUILD)/$(TEST)

$(BUILD)/$(TEST): $(OBJ_TEST)
	$(CC) $(CPPFLAGS) $^ -o $@

sanitize: CPPFLAGS := $(CSTD) $(WARN) -O1 $(DBG) $(INCS) $(DEFS) -fsanitize=address,undefined -fno-omit-frame-pointer
sanitize: clean test

debug: CPPFLAGS := $(CSTD) $(WARN) $(DBG) -O0 $(INCS) $(DEFS)
debug: clean all
	@echo "Run: gdb -q $(BUILD)/$(APP)"

clean:
	rm -rf $(BUILD)

