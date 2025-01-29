# https://www.gnu.org/software/make/manual/make.html#Overview
# Compiler and flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CXX = clang++
else
    CXX = g++
endif

CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude `pkg-config --cflags opencv4` -I/opt/homebrew/include
LDFLAGS = `pkg-config --libs opencv4` -L/opt/homebrew/lib -lavcodec -lavformat -lavutil -lswscale


# Directories
SRC_DIR = imgutil
TEST_DIR = tests
BUILD_DIR = build
BIN_DIR = bin

# Executable target
EXECUTABLE = $(BIN_DIR)/app

# Find all source and test files
SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILES = $(shell find $(TEST_DIR) -name '*.cpp')

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_FILES))

# Default target
all: $(EXECUTABLE)

# Build the main executable
$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp	
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile test files into object files (if needed for a test executable)
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run tests (example placeholder, assuming test framework like Catch2 or Google Test)
test: $(TEST_OBJ_FILES) $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/tests $(LDFLAGS)
	$(BIN_DIR)/tests

# Run the application
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean test
