#include "client.h"

client::client(string _nickname, string _IP_address, int _port)
{
nickname = _nickname;
IP_address = _IP_address;
port = _port;
}
void client::get_nickname(){

}
void client::set_nickname(string _nickname){

}
void client::get_IP_address(){

}
void client::set_IP_address(string _IP_address){

}
void client::create_chatroom(){

}
void client::delete_chatroom(){

}
void client::ignore_user(){

}
void client::create_command_header(){

}
void client::create_message_header(){

}
void client::create_file_header(){

}
void client::join_chatroom(){

}
void client::leave(){

}
void client::log(){

}

client::~client()
{
    //dtor
}
