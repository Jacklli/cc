TARGET=mycc
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc = .o)
CC = g++
INCLUDES = -I .
LIBS = -L .
CCFLAGS = -g -Wall -O0

$(TARGET) : $(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS)

%.o : %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm *.o $(TARGET)
