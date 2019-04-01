CXX=g++

CXXFLAGS=-WAll -O0 -g -std=c++11

all:

client.o: client.cpp spellCHeck.h message.h

server: server.o
    ${CXX} -o client client.o -lpthread
    
clean:
    -rm -fr server client.o spellCheck.o message.o
    