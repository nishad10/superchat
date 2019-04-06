//Chatroom Header File
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include<sstream>
#include "clientNew.h"
#include "message.h"
#include "clientNew.h"
#ifndef chatroom_H
#define chatroom_H

class chatroom
{
	public:
	chatroom(std::string name, bool ispublic, int port);
	void join_chatroom();
	void set_public();
	void record_history(message myMessage);
	bool get_is_empty(); 
	bool get_is_lobby();
	bool get_is_public();
	void set_is_empty(bool is_empty);
	void set_is_lobby(bool Lobby);
	void set_is_public(bool is_public);
	std:: string display_previous(chatroom Chatroom);
	std::string get_name();
	void set_name(std::string name);
	int get_port();
	
	vector<clientNew> Client_list;
	vector<std::string> message_list;
	
	private:
	
	bool is_public;
	bool is_empty;
	bool Lobby;
	std:: string name;
	int port1;



};
#endif