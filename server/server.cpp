#include "server.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
using namespace std;

void do_accept()
  {
    server::acceptor_.async_accept([this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }

          do_accept();
        });
  }
server::server(asio::io_context& io_context,
      const tcp::endpoint& endpoint): acceptor_(io_context, endpoint){do_accept()};                                    // no constructor
 
  void server::read_head(string message_head) {
		
	}

	void server::read_body(string message_body) {

	}

	void server::add_client(string clientName, int clientPort) {

	}

	void server::disconnect(string clientName, int clientPort) {

	}

	void server::ban(string clientName, int clientPort) {

	}

	void server::create_chatroom() {
        
        

	}

	void server::delete_chatroom(chatroom chatroom) {

	}

	void server::save_state() {

	}

	void server::change_chatroom(string chatroom_name) {

	}

	void server::check_name(string client_name) {

	}


int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    asio::io_context io_context;

    std::list<server> servers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      servers.emplace_back(io_context, endpoint);
    }

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

