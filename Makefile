TARGET = hwmonitor
CC = gcc
CXX = g++
.DEFAULT_GOAL := all

BUILD_DIR = ./build
INC_DIRS = ./include ./vendor/cJSON
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2 $(INC_FLAGS)
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 $(INC_FLAGS)

TEST_BUILD_DIR = ./build/tests
TEST_CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c11 -Wall -Wextra -Wpedantic -O2 $(INC_FLAGS)
TEST_SUPPORT = tests/test_support.c vendor/cJSON/cJSON.c

ifeq ($(OS),Windows_NT)
	DETECTED_PLATFORM := windows
else
	DETECTED_PLATFORM := linux
endif

PLATFORM ?= $(DETECTED_PLATFORM)

COMMON_SRCS = \
	src/common/hwmonitor.c \
	src/common/display.c \
	src/common/theme.c \
	src/common/util.c \
	src/api/http.c \
	src/api/groq.c \
	vendor/cJSON/cJSON.c

ifeq ($(PLATFORM),linux)
	PLATFORM_SRCS := $(shell find src/platform/linux -name "*.c")
	PLATFORM_CFLAGS := -D_POSIX_C_SOURCE=200809L
	PLATFORM_LDLIBS := -lcurl
else ifeq ($(PLATFORM),windows)
	PLATFORM_SRCS := $(shell find src/platform/windows \( -name "*.c" -o -name "*.cpp" \))
	PLATFORM_CFLAGS :=
	PLATFORM_LDLIBS := -ladvapi32 -lole32 -loleaut32 -lwbemuuid -liphlpapi -lsetupapi -lpdh -lcurl
else ifeq ($(PLATFORM),macos)
	PLATFORM_SRCS := $(shell find src/platform/macos \( -name "*.c" -o -name "*.cpp" \) 2>/dev/null)
	PLATFORM_CFLAGS :=
	PLATFORM_LDLIBS := -lcurl
else
	$(error Unsupported PLATFORM '$(PLATFORM)'. Use linux, windows, or macos)
endif

SRCS = $(COMMON_SRCS) $(PLATFORM_SRCS)
INCS = $(shell find $(INC_DIRS) -name "*.h")
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS))) \
       $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRCS)))

CFLAGS += $(PLATFORM_CFLAGS)

BIN = $(BUILD_DIR)/$(TARGET)

all: $(BIN)

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_BUILD_DIR)/test_format_size: tests/test_format_size.c src/common/util.c
	$(CC) tests/test_format_size.c src/common/util.c $(TEST_SUPPORT) -o $@ $(TEST_CFLAGS)

$(TEST_BUILD_DIR)/test_parse_arguments: tests/test_parse_arguments.c src/common/util.c
	$(CC) tests/test_parse_arguments.c src/common/util.c $(TEST_SUPPORT) -o $@ $(TEST_CFLAGS)

.PHONY: test
test: $(TEST_BUILD_DIR) $(TEST_BUILD_DIR)/test_format_size $(TEST_BUILD_DIR)/test_parse_arguments
	$(TEST_BUILD_DIR)/test_format_size
	$(TEST_BUILD_DIR)/test_parse_arguments

$(BIN): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $(BIN) $(PLATFORM_LDLIBS)

$(BUILD_DIR)/%.o: %.c $(INCS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp $(INCS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
