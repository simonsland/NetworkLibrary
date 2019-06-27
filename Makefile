MAINSOURCE := test/EchoServer.cpp
SOURCE := $(wildcard *.cpp base/*.cpp)

OBJS := $(patsubst *.cpp *.o $(SOURCE))

TARGET := EchoServer
CC := g++;
LIBS := -pthread
CFLAGS := -std=C++11 -g

$(TARGET) : $(OBJS) test/EchoServer.o
       $(CC) $(CFLAGS) -o $@ $^ $(LIBS)


