CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/home/alex/Documents/asio-1.12.2/include

all: client

client.o: client.cpp message.cpp

client: client.o message.o
	${CXX} -o client client.o -lpthread -lncurses -lasio

clean:
	-rm -f client client.o message.o

