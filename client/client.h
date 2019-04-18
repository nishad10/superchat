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
#include "message.hpp"

using namespace std;

typedef std::deque<message> chat_message_queue;

class client
{
    
    private:
        string nickname;
        string IP_address;
        vector<client> ignore_list;
        int port;
	asio::io_context& io_context_;
	asio::ip::tcp::socket socket_;
	chat_message_queue write_msgs_;
    
    public:
        client(asio::io_context& io_context, const asio::ip::tcp::resolver::results_type& endpoints);
        virtual ~client();

        string get_nickname();
        void set_nickname(string _nickname);
        string get_IP_address();
	void set_port(int _port);
	int get_port();
	void set_IP_address(string _IP_address);

        void do_connect(const asio::ip::tcp::resolver::results_type& endpoints);
	void close();
	void write(const message& msg);
	void do_write();

        void ignore_user();


};

#endif // CLIENT_H
