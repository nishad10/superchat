#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

class message{

    public:
	message();
	virtual ~message();

	void create_command_header(message msg, int type);
        void create_message_header(message msg, int type);
        void create_file_header(message msg, int type);
        void join_chatroom();
        void leave();
        void log();
	void create_chatroom();
        void delete_chatroom();

	void read_header();
	void read_body();

        std::string encode(std::string head, std::string body);
        //std::string decode_head(std::string endc_str);
        //std::string decode_body(std::string endc_str);
        std::string encrypt(std::string body);
        std::string decrypt(std::string body);

        int head = -1;
        std::string body;
        int length;
	int max_length = 1024;

};
#endif
