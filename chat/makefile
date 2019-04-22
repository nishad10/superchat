CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/home/alex/Documents/asio-1.12.2/include

all: client server

chat_client.o: chat_client.cpp chat_message.hpp

chat_server.o: chat_server.cpp chat_message.hpp

server: chat_server.o
	${CXX} -o chat_server chat_server.o -lpthread

client: chat_client.o
	${CXX} -o chat_client chat_client.o -lpthread

clean:
	-rm -f chat_client chat_client.o chat_server chat_server.o

