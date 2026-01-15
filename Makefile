.PHONY: clean

# name of final executable
TARGET_EXEC := bf

# project directory structure
SRC_DIR := src       # C source files
BUILD_DIR := build   # object files
BIN_DIR := bin       # final binary

# compiler
CC := gcc
CFLAGS := -Wall -Wextra -pedantic

# all source files in SRC_DIR
SRCS := $(wildcard src/*.c)
# map source file names to object file names in BUILD_DIR
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# link all object files into final executable
$(BIN_DIR)/$(TARGET_EXEC).out: $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# compile each source file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# remove all build artifacts
clean:
	rm -rf $(BUILD_DIR)/*
