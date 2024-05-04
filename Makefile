.SILENT:

CXX = g++
CXXFLAGS = -std=c++20 -Wall -lraylib

SRCS = src/*.cpp
TARGET = bin/spring_simulation

build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: build
	./$(TARGET)
