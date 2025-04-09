# Makefile for the Computer Communications and Networking Project

# Compiler and flags
CC = gcc-11
CFLAGS = -Wall -Wextra -g -I.

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TEST_DIR = tests

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.o,$(TEST_FILES))

# Main object file (exclude from tests)
MAIN_OBJ = $(BUILD_DIR)/main.o

# Objects for lib (exclude main)
LIB_OBJ = $(filter-out $(MAIN_OBJ),$(OBJ_FILES))

# Executables
TARGET = $(BUILD_DIR)/network_sim
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%,$(TEST_FILES))

# Default target
all: directories $(TARGET)

# Rule for creating build directory
directories:
	mkdir -p $(BUILD_DIR)

# Rule for compiling the main program
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for compiling source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling test files
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for building test executables
$(BUILD_DIR)/test_%: $(BUILD_DIR)/test_%.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Build all tests
tests: directories $(TEST_TARGETS)

# Run all tests
run_tests: tests
	for test in $(TEST_TARGETS); do \
		echo "Running $$test"; \
		$$test; \
		echo ""; \
	done

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        : Build the main program (default target)"
	@echo "  tests      : Build all test executables"
	@echo "  run_tests  : Build and run all tests"
	@echo "  clean      : Remove all build files"
	@echo "  help       : Display this help message"

# Individual test targets
ipv4_test: directories $(BUILD_DIR)/test_ipv4
	$(BUILD_DIR)/test_ipv4

network_test: directories $(BUILD_DIR)/test_network
	$(BUILD_DIR)/test_network

# Phony targets
.PHONY: all clean directories help tests run_tests ipv4_test network_test
