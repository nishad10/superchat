//Chatroom Header File
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include<sstream>
#include "client.h"
#include "message.h"
#ifndef chatroom_H
#define chatroom_H

class chatroom
{
	public:
	chatroom(std:: string name, Bool ispublic, int port);
	join_chatroom();
	set_public();
	record_history(Message myMessage);
	bool get_is_empty(); 
	bool get_is_lobby();
	bool get_is_public();
	set_is_empty(bool is_empty);
	set_is_lobby(bool Lobby);
	set_is_public(bool is_public);
	std:: string display_previous(chatroom Chatroom);
	std:: string get_name();
	set_name(std::string name);
	int get_port();
	
	vector<Client> Client_list;
	vector<std::string> message_list;
	
	private:
	
	bool is_public;
	bool is_empty;
	bool Lobby;
	std:: string name;
	int port1;



}
#endif