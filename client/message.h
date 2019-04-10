#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

class message{

    public:
	void create_command_header();
        void create_message_header();
        void create_file_header();
        void join_chatroom();
        void leave();
        void log();
	void create_chatroom();
        void delete_chatroom();

        std::string encode(std::string head, std::string body);
        std::string decode_head(std::string endc_str);
        std::string decode_body(std::string endc_str);
        void encrypt(std::string body);
        void decrypt(std::string body);

        int head;
        std::string body;
        int length;


};
#endif
