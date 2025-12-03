# =========================================================
#  Makefile for SFML 2.6.2 (MinGW / VS Code)
# =========================================================

# [CRITICAL] Force make to use Windows Command Prompt
SHELL = cmd.exe

# 1. PATHS
# Matches the path you provided in cpp_properties.json
SFML_PATH = C:/Cpp/Libraries/SFML-2.6.2

# 2. COMPILER SETTINGS
CXX      = g++
# -std=c++17: Modern C++ features
# -Iheaders: Look in your local 'headers' folder
# -I...: Look in the SFML include folder
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iheaders -I"$(SFML_PATH)/include"

# 3. LINKER SETTINGS
# ADDED: -lsfml-audio (Required for sound)
LDFLAGS  = -L"$(SFML_PATH)/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# 4. DIRECTORIES
SRC_DIR   = src
BUILD_DIR = build
TARGET    = $(BUILD_DIR)/HyperDash.exe

# 5. FILES
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# 6. TARGETS
all: $(TARGET)

# Link Step
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile Step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Step
clean:
	if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"

# Run Step
run: $(TARGET)
	$(TARGET)

.PHONY: all clean run