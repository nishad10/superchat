//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  // initialize to a - character, to show you have opened the client
  // but have not yet logged on and have not entered the lobby
  char chatroom_ = '-';
  char username[chat_message::max_body_length + 1];
  char server_response;

  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

  void log_on()
  {
    int i = 1;
    while(i) {
      std::cout << "Enter a username: ";
      std::cin.getline(this->username, chat_message::max_body_length + 1);
      
      chat_message msg;
      msg.body_length(std::strlen(this->username));

      //add tags to message
      this->username[msg.body_length()] = '0';
      //tag "-01--" is a log in command
      this->username[msg.body_length() + 1] = '0';
      this->username[msg.body_length() + 2] = '1';
      this->username[msg.body_length() + 3] = '0';
      this->username[msg.body_length() + 4] = '0';
      this->username[msg.body_length() + 5] = '\0';
      //re calculate msg.body_length()
      msg.body_length(std::strlen(this->username));
      std::memcpy(msg.body(), this->username, msg.body_length());
      msg.encode_header();
      this->write(msg);
      this->chatroom_ = 'S';
      if(server_response) {
	i = 0;
	this->chatroom_ = '0';
      }
    }
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
	  if(read_msg_.body()[read_msg_.body_length() - 5] == this->chatroom_) {
	    read_msg_.body()[read_msg_.body_length() - 1] = '\0';
	    std::cout.write(read_msg_.body(), read_msg_.body_length() -5);
	    this->server_response = read_msg_.body()[read_msg_.body() - 3];
	  }
	  else if(read_msg_.body()[read_msg_.body_length() - 5] == this->chatroom_)
	  {
	    if (!ec)
	    {
	      //add new null character to message youre reading
	      //so it doesnt print the message tags
	      read_msg_.body()[read_msg_.body_length() - 1] = '\0';
	      //print up to the new null character
	      std::cout.write(read_msg_.body(), read_msg_.body_length() - 5);
	      std::cout << "\n";
	      do_read_header();
	    }
	  }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);

    chat_client c(io_context, endpoints);

    c.log_on();

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::string check_message = line;
      if(check_message == "./logout") {
	std::cout << "Logging out . . ." << std::endl;
	break;
      }
      //add tags to message
      //first tag is the room you're in
      line[msg.body_length()] = c.chatroom_;
      //tags 2-3 are the message types
      line[msg.body_length() + 1] = '0';
      line[msg.body_length() + 2] = '0';
      //tags 4-5 are the user id's
      line[msg.body_length() + 3] = '0';
      line[msg.body_length() + 4] = '0';
      //add the null character at the end
      line[msg.body_length() + 5] = '\0';
      //re calculate msg.body_length()
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
