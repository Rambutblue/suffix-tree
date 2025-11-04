# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O2

# Target name
TARGET = test

# Source files
SOURCES = solution.cpp test.cpp

# Rule to build the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to run the tests
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Rule to clean up generated files
.PHONY: clean
clean:
	rm -f $(TARGET) *.o
