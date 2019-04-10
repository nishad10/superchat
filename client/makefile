CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=~/Documents/asio-1.12.2/include

all:client

client: client.o message.o
	${CXX} -o client client.o -lpthread -lncurses
    
client.o: client.cpp message.cpp

clean:
	-rm -f client client.o message.o

