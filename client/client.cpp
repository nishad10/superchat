#include "client.h"
client::client(std::string _nickname, std::string _IP_address, int _port)
{
	nickname = _nickname;
	IP_address = _IP_address;
	port = _port;
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

void client::set_IP_address(std::string _IP_address) {
	client.IP_address = _IP_address;
}

void client::ignore_user(std::string nickname){
	client.ignore_user.pushback(nickname);
}


client::~client() {
    //dtor
}

int main()
{


	return 0;
}
