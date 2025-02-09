CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Ithird_party

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
TEST_SRC = tests/main_test.cpp $(wildcard tests/test_*.cpp)
TEST_OBJ = $(patsubst tests/%.cpp, build/tests/%.o, $(TEST_SRC))

all: main

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o main $(OBJ)

build/tests/%.o: tests/%.cpp
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: build/tests/main_test.o build/tests/test_CommandProcessor.o \
     build/CommandProcessor.o build/CoreService.o build/Logger.o \
     build/NetService.o build/SocketManager.o
	$(CXX) $(CXXFLAGS) -o test_runner \
	    build/tests/main_test.o \
	    build/tests/test_CommandProcessor.o \
	    build/CommandProcessor.o \
	    build/CoreService.o \
	    build/Logger.o \
	    build/NetService.o \
	    build/SocketManager.o
	./test_runner  # ✅ Properly indented

clean:
	rm -rf build main test_runner
