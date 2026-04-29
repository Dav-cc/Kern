# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

# Directories
LEXER_DIR = Lexer
PARSER_DIR = parser
BUILD_DIR = build

# Source files
MAIN_SRC = main.c
LEXER_SRC = $(LEXER_DIR)/lexer.c
PARSER_SRC = $(PARSER_DIR)/parser.c

# Object files
MAIN_OBJ = $(BUILD_DIR)/main.o
LEXER_OBJ = $(BUILD_DIR)/lexer.o
PARSER_OBJ = $(BUILD_DIR)/parser.o

# Output executable
TARGET = kern

# Default target
.PHONY: all
all: $(TARGET)

# Link the executable
$(TARGET): $(MAIN_OBJ) $(LEXER_OBJ) $(PARSER_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "Build complete: $(TARGET)"

# Compile main.c
$(MAIN_OBJ): $(MAIN_SRC) $(LEXER_DIR)/lexer.h $(PARSER_DIR)/parser.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile lexer.c
$(LEXER_OBJ): $(LEXER_SRC) $(LEXER_DIR)/lexer.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile parser.c
$(PARSER_OBJ): $(PARSER_SRC) $(PARSER_DIR)/parser.h $(LEXER_DIR)/lexer.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned build files"

# Run the executable
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Run with test file
.PHONY: test
test: $(TARGET)
	./$(TARGET) test.c

# Check for memory leaks (requires valgrind)
.PHONY: valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build files"
	@echo "  run      - Build and run the executable"
	@echo "  test     - Build and run with test.c"
	@echo "  valgrind - Run with valgrind memory check"
	@echo "  help     - Show this help message"
