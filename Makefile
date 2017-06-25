TARGET=mycc
CC = g++
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc = .o)
INCLUDES = -I .
LIBS = -L .
CXXFLAGS = -std=c++11 -g -O0

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

%.o : %.cc
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm *.o $(TARGET)
