CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/Users/shiba/Desktop/asio-1.12.2/include

all:server

server: server.o message.o chatroom.o
	${CXX} -o server server.o -lpthread -lncurses
    
server.o: server.cpp message.cpp chatroom.cpp

clean:
	-rm -f server server.o message.o chatroom.o

