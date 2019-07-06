MAINSOURCE := test/EchoServer.cpp
SOURCE := $(wildcard *.cpp base/*.cpp)

OBJS := $(patsubst %.cpp, %.o, $(SOURCE))

TARGET := EchoServer
CC := g++
LIBS := -pthread
CXXFLAGS := -std=c++11 -rdynamic -g



$(TARGET) : $(OBJS) test/EchoServer.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)

.PHONY : clean
clean : 
	-rm $(TARGET) $(OBJS) test/EchoServer.o
