#include <iostream>
#include <string>

#include "message.h"

using namespace std;

// message header determines what type of message it is

// -1 = default message head
// 0 - chatroom message
// 1 - encrypted chatroom message
// 2 - file transfer message
// 3 - join chatroom command
// 4 - leave chatroom command
// 5 - logout command command
// 6 - create chatroom command
// 7 - delete chatroom command
// 8 - ignore user command
// 9 - ban user command (?)

message::message() {}
//virtual ~message::message() {}

void message::create_command_header(message msg, int type) {
	msg.head = type;
}

void message::create_message_header(message msg, int type) {
	msg.head = type;
}

void message::create_file_header(message msg, int type) {
	msg.head = type;
}

void message::join_chatroom() {

}

void message::leave() {

}

void message::log() {

}

void message::create_chatroom() {

}

void message::delete_chatroom() {

}
/*
string message::encode(std::string body, int code) {
	
}

string message::decode(std::string body, int code) {

}

string message::encrypt(std::string body) {

}

string message::decrypt(std::string body) {

}

int message::read_header(int message.head) {	
	return message.head;
}

string message::read_body(string message.body) {
	return message.body;
}
*/
int main()
{
	return 0;
}
