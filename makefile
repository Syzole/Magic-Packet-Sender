# choose compiler and target name
CXX = g++
TARGET = magicPacketSender

# Define the source files and the object files
SRCS = magicPacketSender.cpp
OBJS = $(SRCS:.cpp=.o)

# choose flags
CXXFLAGS = -Wall -g -std=c++11
LDFLAGS = -lws2_32

# default target
all: $(TARGET)

test: test.cpp test.o
	$(CXX) $(CXXFLAGS) -o test test.o $(LDFLAGS)


test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp -o test.o

# link all object files into the target
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean up the object files and the target
clean:
	del /Q *.o $(TARGET).exe test.exe

# phony targets
.PHONY: all clean
