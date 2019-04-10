#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <string>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "message.h"

using namespace std;

class client
{
    
    private:
        string nickname;
        string IP_address;
        vector<client> ignore_users;
        int port;
    
    public:
        client(string _nickname, string _IP_address, int _port);
        virtual ~client();
        string get_nickname();
        void set_nickname(string _nickname);
        void get_IP_address();
        void set_IP_address(string _IP_address);
        
        void ignore_user();


};

#endif // CLIENT_H
