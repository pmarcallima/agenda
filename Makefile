# Project Settings
debug ?= 0
NAME := agenda 
SRC_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./include
LIB_DIR := ./lib
TESTS_DIR := ./tests
BIN_DIR := ./bin

# Generate paths for all object files
OBJS := $(patsubst %.c,%.o, $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/**/*.c))

# Compiler settings
CC := clang-14
LINTER := clang-tidy-14
FORMATTER := clang-format-14

# GTK 3.0 settings
PKG_CONFIG := pkg-config
GTK_FLAGS := $(shell $(PKG_CONFIG) --cflags --libs gtk+-3.0)

# Compiler and Linker flags Settings:
CFLAGS := -Iinclude -std=gnu17 -D _GNU_SOURCE -D __STDC_WANT_LIB_EXT1__ -Wall -Wextra -pedantic $(GTK_FLAGS)
LDFLAGS := -lm $(GTK_FLAGS)

ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -Oz
endif

# Targets

# Build executable
$(NAME): format lint dir $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $(patsubst %, build/%, $(OBJS))

# Build object files and third-party libraries
$(OBJS): dir
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $*.c

# Run CUnit tests
test: dir
	@$(CC) $(CFLAGS) -lcunit -o $(BIN_DIR)/$(NAME)_test $(TESTS_DIR)/*.c
	@$(BIN_DIR)/$(NAME)_test

# Run linter on source directories
lint:
	@$(LINTER) --config-file=.clang-tidy $(SRC_DIR)/* $(INCLUDE_DIR)/*  -- $(CFLAGS)

# Run formatter on source directories
format:
	@$(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/* 

# Run valgrind memory checker on executable
check: $(NAME)
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< --help
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< --version
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< -v

# Setup dependencies for build and development
setup:
	# Update apt and upgrade packages
	@sudo apt update
	@sudo DEBIAN_FRONTEND=noninteractive apt upgrade -y

	# Install OS dependencies
	@sudo apt install -y bash libarchive-tools lsb-release wget software-properties-common gnupg

	# Install LLVM tools required for building the project
	@wget https://apt.llvm.org/llvm.sh
	@chmod +x llvm.sh
	@sudo ./llvm.sh 18
	@rm llvm.sh

	# Install Clang development tools
	@sudo apt install -y clang-tools-18 clang-format-18 clang-tidy-18 valgrind bear

	# Install CUnit testing framework
	@sudo apt install -y libcunit1 libcunit1-doc libcunit1-dev

	# Install GTK 3.0
	@sudo apt install -y libgtk-3-dev

	# Cleanup
	@sudo apt autoremove -y

# Setup build and bin directories
dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Clean build and bin directories
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run bear to generate compile_commands.json
bear:
	bear --exclude $(LIB_DIR) make $(NAME)

.PHONY: lint format check setup dir clean bear

