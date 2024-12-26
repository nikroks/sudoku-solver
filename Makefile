# Compiler
CXX = g++

RM = rm

# Compiler flags
CXXFLAGS = -Wall -g -std=c++11 -stdlib=libc++

TARGET = sudoku_solver.out

# Source files
SRCS = sudoku_solver.cpp

build: sudoku_solver.o
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: sudoku_solver.o
	./$(TARGET)

clean:
	$(RM) -rf *.out *.o *.out.dSYM