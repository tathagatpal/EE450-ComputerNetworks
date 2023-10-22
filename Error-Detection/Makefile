# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Werror

# Source files and output executable
SRC = ee450_PA1_tpal.cpp
TARGET = ee450_PA1_tpal

# Output file for saving the program's output
OUTPUT_FILE = output.txt

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Run the program and save output to a text file
run: $(TARGET)
	./$(TARGET) | tee $(OUTPUT_FILE)

# Clean build artifacts and output file
clean:
	rm -f $(TARGET) $(OUTPUT_FILE)

.PHONY: all run clean

