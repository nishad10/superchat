CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/Users/shiba/Downloads/asio-1.12.2/include

all:client

client: client.o message.o
	${CXX} -o client client.cpp message.cpp -lpthread -lncurses
    
client.o: client.cpp client.h
    g++ --std=c++11 -c client.cpp
    
message.o: message.cpp message.h
    g++ --std=c++11 -c message.cpp

clean:
	-rm -f client client.o message.o

