CXX=g++

CXXFLAGS=-WAll -O0 -g -std=c++11

all: server

server.o: server.cpp spellCHeck.h chatroom.h message.h

server: server.o
    ${CXX} -o server server.o -lpthread
    
clean:
    -rm -fr server server.o spellCheck.o chatroom.o message.o
    
