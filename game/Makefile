# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Iinclude  # Include the 'include' directory for header files

# Linker flags
LDFLAGS = -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system

# Output executable name
TARGET = main

# Source files
SRCS = main.cpp  # Correct path to RoundedRectangle.cpp

# Object files
OBJS = main.o   # Adjust path for RoundedRectangle.o

# Default target to build the program
all: $(TARGET)

# Linking the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compiling the source files to object files
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
