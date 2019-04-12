#include "client.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include "asio.hpp"
#include "message.h"

using namespace asio::ip::tcp;

client::client(asio::io_context& io_context, const tcp::resolver::results_type& endpoints) : io_context_(io_context), socket_(io_context) {
	//asio::async_connect(socket_, endpoints, boost::bind(&client::handle_connect, this, asio::placeholders::error));
	do_connect(endpoints);
}

/*void client::handle_connect(const asio::error_code& error) {
	if(!error) {
		asio::async_read(socket_, asio::buffer
	}
}*/

std::string client::get_nickname() {
	return client.nickname;
}

void client::set_nickname(std::string _nickname) {
	client.nickname = _nickname;
}

std::string client::get_IP_address() {
	return client.IP_address;
}

void client::set_port(int _port) {
	client.port = _port;
}

int client::get_port() {
	return client.port;
}

void client::set_IP_address(std::string _IP_address) {
	client.IP_address = _IP_address;
}

void client::ignore_user(std::string nickname){
	client.ignore_list.pushback(nickname);
}


client::~client() {
    //dtor
}

int main(int argc, char* argv[])
{
	try {
		std::cerr << "Usage: chat_client <host> <port>\n");
		return 1;
	}

	asio::io_context io_context;
	tcp::resolver resolver(io_context);
	tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);

	chat_client c(

	return 0;
}
