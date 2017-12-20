CPPFLAGS = -g -lm -Wall -O0 -std=c++14
LIBDIR = ./Libraries
OBJS = build/interval.o build/autodiff.o build/taylor.o build/taylor_double.o
BUILD_DIR := ./build
EXERCISES = $(shell find ./ -name exercise*.cpp)
CL1DIR = ./ComputerLab1
CL2DIR = ./ComputerLab2
CL3DIR = ./ComputerLab3
CL4DIR = ./ComputerLab4
CL5DIR = ./ComputerLab5

# Complete build

all : libs exercises

# Libraries

$(BUILD_DIR)/%.o : $(LIBDIR)/%.cpp $(LIBDIR)/%.hpp $(LIBDIR)/rounding.h
	mkdir -p $(dir $@)
	g++ $(CPPFLAGS) -c $< -o $@

libs : $(OBJS)

# Executables

$(CL1DIR)/%.x : $(CL1DIR)/%.cpp $(OBJS) ./ComputerLab1/exercise3.h
	g++ $(CPPFLAGS) $< $(OBJS) -o $@

$(CL2DIR)/%.x : $(CL2DIR)/%.cpp $(OBJS)
	g++ $(CPPFLAGS) $< $(OBJS) -o $@

$(CL3DIR)/%.x : $(CL3DIR)/%.cpp $(OBJS)
	g++ $(CPPFLAGS) $< $(OBJS) -o $@

$(CL4DIR)/%.x : $(CL4DIR)/%.cpp $(OBJS)
	g++ $(CPPFLAGS) $< $(OBJS) -o $@

$(CL5DIR)/%.x : $(CL5DIR)/%.cpp $(OBJS)
	g++ $(CPPFLAGS) $< $(OBJS) -o $@

exercises : $(subst .cpp,.x,$(shell find ./ -name exercise*.cpp))

# Cleaning

clean :
	rm -r ./build
	rm $(shell find ./ -name exercise*.x)
