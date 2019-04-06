#ifndef SERVER_H_
#define SERVER_H_
#include <string>
#include "clientNew.h"
#include "chatroom.h"
using namespace std;
class server
{
	
private:
	int chatroom_size, number_of_clients;
public:
	server();
    ~server();
	std::vector<clientNew> list_of_client;
	std::vector<chatroom> list_of_chatroom;

	void read_head(string message_head);

	void read_body(string message_body);

	void add_client(string clientName, int clientPort);

	void disconnect(string clientName, int clientPort);

	void ban(string clientName, int clientPort);
	
	void create_chatroom();

	void delete_chatroom(chatroom chatroom);

	void save_state();

	void change_chatroom(string chatroom_name);

	void check_name(string client_name);
};

#endif
