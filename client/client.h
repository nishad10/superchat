#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <string>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include "asio.hpp"
#include "message.h"

using namespace std;

class client
{
    
    private:
        string nickname;
        string IP_address;
        vector<client> ignore_list;
        int port;
    
    public:
        client(string _nickname, string _IP_address, int _port);
        virtual ~client();
        string get_nickname();
        void set_nickname(string _nickname);
        string get_IP_address();
	void set_port(int _port);
	int get_port();
	void set_IP_address(string _IP_address);
        

        void ignore_user();


};

#endif // CLIENT_H
