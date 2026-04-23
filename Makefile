TARGET = hwmonitor
CC = gcc
.DEFAULT_GOAL := all

SRC_DIRS = ./src ./vendor/cJSON/cJSON.c
BUILD_DIR = ./build
INC_DIRS = ./include ./vendor/cJSON
CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c11 -Wall -Wextra -Wpedantic -O2 $(INC_FLAGS)

TEST_BUILD_DIR = ./build/tests
TEST_CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c11 -Wall -Wextra -Wpedantic -O2 $(INC_FLAGS)
TEST_SUPPORT = tests/test_support.c vendor/cJSON/cJSON.c

SRCS = $(shell find $(SRC_DIRS) -name "*.c")
INCS = $(shell find $(INC_DIRS) -name "*.h")

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

BIN = $(BUILD_DIR)/$(TARGET)

all: $(BIN)

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_BUILD_DIR)/test_format_size: tests/test_format_size.c src/util.c
	$(CC) tests/test_format_size.c src/util.c $(TEST_SUPPORT) -o $@ $(TEST_CFLAGS)

$(TEST_BUILD_DIR)/test_parse_arguments: tests/test_parse_arguments.c src/util.c
	$(CC) tests/test_parse_arguments.c src/util.c $(TEST_SUPPORT) -o $@ $(TEST_CFLAGS)

.PHONY: test
test: $(TEST_BUILD_DIR) $(TEST_BUILD_DIR)/test_format_size $(TEST_BUILD_DIR)/test_parse_arguments
	$(TEST_BUILD_DIR)/test_format_size
	$(TEST_BUILD_DIR)/test_parse_arguments

$(BIN): $(SRCS) $(INCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) -o $(BIN) $(CFLAGS) -lcurl

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

PREFIX ?= /usr/local

.PHONY: install uninstall
install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
