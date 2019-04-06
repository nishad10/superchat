CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/Users/shiba/Downloads/asio-1.12.2/include

all:server

server: server.o message.o chatroom.o
	${CXX} -o server server.cpp message.cpp chatroom.cpp -lpthread -lncurses
    
server.o: server.cpp server.h
    g++ --std=c++11 -c server.cpp
    
message.o: message.cpp message.h
    g++ --std=c++11 -c message.cpp

chatroom.o: chatroom.cpp chatroom.h
    g++ --std=c++11 -c chatroom.cpp

clean:
	-rm -f server server.o message.o chatroom.o

