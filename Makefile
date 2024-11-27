# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Directories
SRC_DIR = src
INCLUDE_DIR = include
SHADERS_DIR = shaders
BUILD_DIR = build
BIN_DIR = bin

# Executable name
TARGET = $(BIN_DIR)/ArtGallery

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Libraries and includes
LIBS = -lGL -lglfw -lGLEW -lm
INCLUDES = -I$(INCLUDE_DIR)

# Rules
all: setup $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(TARGET) $(LIBS)
	@echo "Build successful! Executable: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiled: $< -> $@"

setup:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build and bin directories."

run: all
	./$(TARGET)

