TARGET = cc
CC = g++
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc = .o)
INCLUDES = -I .
CXXFLAGS = -std=c++11 -g -O0

all: $(OBJS)
	$(CC) $(CXXFLAGS) $^ -o ${TARGET} $(INCLUDES)


clean:
	rm -rf *.o $(TARGET)
