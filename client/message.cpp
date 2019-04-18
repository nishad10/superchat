#include <iostream>
#include <string>

#include "message.h"

using namespace std;

// message header determines what type of message it is

// 0 - chatroom message
// 1 - file transfer
// 2 - server message (may split up and use head to
//			determine exactly what command)
void create_command_header() {

}

void create_message_header() {

}

void create_file_header() {

}

void join_chatroom() {

}

void leave() {

}

void log() {

}

void create_chatroom() {

}

void delete_chatroom() {

}

string encode(string head, string body) {
	
}

string decode_head(string endc_str) {

}

string decode_body(string endc_str) {

}

void encrypt(std::string body) {

}

void decrypt(std::string body) {

}

int main()
{
	return 0;
}
