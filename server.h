#ifndef SERVER_H_
#define SERVER_H_

class server()
{
	
private:
	int chatroom_size, number_of_clients;
public:
	Server()
	~Server()
	std::vector<Client> list_of_client;
	std::vector<Chatroom> list_of_chatroom;

	void read_head(struct message_head);

	void read_body(struct message_body);

	void add_client(Client client);

	void disconnect(Client client);

	void ban(Client client);
	
	void create_chatroom();

	void delete_chatroom(Chatroom chatroom);

	void save_state();

	void change_chatroom(string chatroom_name);

	void check_name(string client_name);
}

#endif
