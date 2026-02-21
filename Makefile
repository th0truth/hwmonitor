TARGET = hwmonitor

CC = gcc

SRC_DIRS = ./src ./external/cJSON/cJSON.c
BUILD_DIR= ./build
INC_DIRS = ./include ./external/cJSON

SRCS = $(shell find $(SRC_DIRS) -name "*.c") 
INCS = $(shell find $(INC_DIRS) -name "*.h")

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = $(INC_FLAGS)

$(BUILD_DIR)/$(TARGET).o: $(SRCS) $(INCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)