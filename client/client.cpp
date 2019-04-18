#include "client.h"
#include "message.hpp"

using asio::ip::tcp;

client::client(asio::io_context& io_context, const tcp::resolver::results_type& endpoints) : io_context_(io_context), socket_(io_context) {
	do_connect(endpoints);
}

void client::do_connect(const tcp::resolver::results_type& endpoints) {
	asio::async_connect(socket_, endpoints, [this](std::error_code ec, tcp::endpoint) {
		if(!ec) {
			//read_header();
		}
	});
}

void client::close() {
	asio::post(io_context_, [this]() {
		socket_.close();
	});
}

void client::write(const message& msg) {
    asio::post(io_context_,
        [this, msg]() {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress) {
            do_write();
          }
        });
}

void client::do_write() {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
          }
          else {
            socket_.close();
          }
        });
}

/*
void message::read_header() {

}

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
*/
int main(int argc, char* argv[])
{
	try {
		if (argc != 3) {
			std::cerr << "Usage: chat_client <host> <port>\n";
			return 1;
		}

		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(argv[1], argv[2]);

		client c(io_context, endpoints);

		std::thread t([&io_context]() {
			io_context.run();
		});

		char line[message::max_length+1];

		while(std::cin.getline(line, message::max_length+1)) {
			message msg;
			msg.body_length(std::strlen(line));
			std::memcpy(msg.body(), line, msg.body_length());
			c.write(msg);
		}		

		c.close();
		t.join();

	}
	
	catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
