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
  char chatroom_ = '0';
  char username_[20];
  // int used to encode and decode messages, set to 1 by default
  int secret_code_ = 1;

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

  /*void login()
  {
    std::cout << "Enter username: ";
    std::cin >> this->username_;

    chat_message msg;
    msg.body_length(std::strlen(this->username_));

    //add tags to message
    line[msg.body_length()] = 'S';
    line[msg.body_length() + 1] = '0';
    line[msg.body_length() + 2] = '2';
    line[msg.body_length() + 3] = '0';
    line[msg.body_length() + 4] = '0';
    line[msg.body_length() + 5] = '\0';
    //re calculate msg.body_length()
    msg.body_length(std::strlen(this->username_));
    std::memcpy(msg.body(), this->username_, msg.body_length());
    msg.encode_header();
    this->write(msg);
  }*/

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
	  // check if the normal message was sent from the chatroom youre in.
	  if(read_msg_.body()[read_msg_.body_length() - 5] == this->chatroom_)
	  {
            if (!ec)
            {
	      if (read_msg_.body()[read_msg_.body_length() - 4] == '0' && read_msg_.body()[read_msg_.body_length() - 3] == '0')
	      {
	        //add new null character to message youre reading
	        //so it doesnt print the message tags
	        read_msg_.body()[read_msg_.body_length() - 1] = '\0';
	        //print up to the null character
                std::cout.write(read_msg_.body(), read_msg_.body_length() - 5);
                std::cout << "\n";
                do_read_header();
	      }
	      // decrypt the message before you print it
	      else if (read_msg_.body()[read_msg_.body_length() - 4] == '0' && read_msg_.body()[read_msg_.body_length() - 3] == '3')
	      {
	        //add new null character to message youre reading
	        //so it doesnt print the message tags
	        read_msg_.body()[read_msg_.body_length() - 1] = '\0';
	        //print up to the null character
		for(int i = 0; i < read_msg_.body_length() - 1; i++)
		  read_msg_.body()[i] = read_msg_.body()[i] - this->secret_code_;
                std::cout.write(read_msg_.body(), read_msg_.body_length() - 5);
                std::cout << "\n";
                do_read_header();
	      }
            }
	  }
	  // check if the server response is directed to you
	  else if(read_msg_.body()[read_msg_.body_length() - 5] == 'S' &&
		  read_msg_.body()[read_msg_.body_length() - 4] == '0' &&
		  read_msg_.body()[read_msg_.body_length() - 3] == '4' &&
		  read_msg_.body()[read_msg_.body_length() - 2] == '0' &&
		  read_msg_.body()[read_msg_.body_length() - 1] == '0')
	  {
	    if (!ec)
            {
	      if(read_msg_.body()[0] != 'X')
	      {
		this->chatroom_ = read_msg_.body()[0];
		std::cout << "Changed rooms to " << this->chatroom_ << "!\n";
	      }
	      else
		std::cout << "Failed to changes rooms\n";
            }
	  }
	  else if(read_msg_.body()[read_msg_.body_length() - 5] == 'S' &&
		  read_msg_.body()[read_msg_.body_length() - 2] == '0' &&
		  read_msg_.body()[read_msg_.body_length() - 1] == '0')
	  {
	    if (!ec)
            {
	      //add new null character to message youre reading
	      //so it doesnt print the message tags
	      read_msg_.body()[read_msg_.body_length() - 1] = '\0';
	      //print up to the null character
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
    
    //c.login();

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      if(strcmp(line, "./commands") == 0 || strcmp(line, "./help") == 0)
	std::cout << "Commands:\n"
		  << "./commands or ./help for a list of commands\n"
		  << "./createchatroom to create a new chatroom\n"
		  << "./deletechatroom to delete an empty chatroom\n"
		  << "./secretmessage to send an encoded message\n"
		  << "./secretcode to set your secret code for encoding/decoding secret messages\n"
		  << "./changeroom to change chat rooms"
	 	  << "\n";
      else if(strcmp(line, "./createchatroom") == 0)
      {
	chat_message msg;
        msg.body_length(std::strlen(line));
	
        //add tags to message
        line[msg.body_length()] = 'S';
        line[msg.body_length() + 1] = '0';
        line[msg.body_length() + 2] = '1';
        line[msg.body_length() + 3] = '0';
        line[msg.body_length() + 4] = '0';
        line[msg.body_length() + 5] = '\0';
        //re calculate msg.body_length()
        msg.body_length(std::strlen(line));
        std::memcpy(msg.body(), line, msg.body_length());
        msg.encode_header();
        c.write(msg);
	
      }
      else if(strcmp(line, "./deletechatroom") == 0)
      {
	std::cout << "Enter chatroom number to delete (1-9): ";
	char input[6];
	std::cin >> input[0];
	if (input[0] == '0')
	  std::cout << "Error. Can't delete the lobby.\n";
	else
	{
	  chat_message msg;
          msg.body_length(std::strlen(input));
          //add tags to message
          input[msg.body_length()] = 'S';
          input[msg.body_length() + 1] = '0';
          input[msg.body_length() + 2] = '2';
          input[msg.body_length() + 3] = '0';
          input[msg.body_length() + 4] = '0';
          input[msg.body_length() + 5] = '\0';
          //re calculate msg.body_length()
          msg.body_length(std::strlen(input));
          std::memcpy(msg.body(), input, msg.body_length());
          msg.encode_header();
          c.write(msg);
	}
      }
      else if(strcmp(line, "./secretmessage") == 0)
      {
	std::cout << "Enter your message: ";
	char message[chat_message::max_body_length + 1];
	std::cin.getline(message, chat_message::max_body_length + 1);
	chat_message msg;
	msg.body_length(std::strlen(message));
	for(int i = 0; i < msg.body_length(); i++)
	  message[i] = message[i] + c.secret_code_;
	//add tags to message
	message[msg.body_length()] = c.chatroom_;
	message[msg.body_length() + 1] = '0';
	message[msg.body_length() + 2] = '3';
	message[msg.body_length() + 3] = '0';
	message[msg.body_length() + 4] = '0';
	message[msg.body_length() + 5] = '\0';
	//re calculate msg.body_length()
	msg.body_length(std::strlen(message));
	std::memcpy(msg.body(), message, msg.body_length());
	msg.encode_header();
	c.write(msg);
      }
      else if(strcmp(line, "./changeroom") == 0)
      {
	std::cout << "Enter chatroom number to move to: ";
	char input[6];
	std::cin >> input[0];
	if (input[0] == c.chatroom_)
	  std::cout << "Error. Already in that chatroom.\n";
	else
	{
	  input[1] = c.chatroom_;
	  chat_message msg;
          msg.body_length(std::strlen(input));
          //add tags to message
          input[msg.body_length()] = 'S';
          input[msg.body_length() + 1] = '0';
          input[msg.body_length() + 2] = '4';
          input[msg.body_length() + 3] = '0';
          input[msg.body_length() + 4] = '0';
          input[msg.body_length() + 5] = '\0';
          //re calculate msg.body_length()
          msg.body_length(std::strlen(input));
          std::memcpy(msg.body(), input, msg.body_length());
          msg.encode_header();
          c.write(msg);
	}
      }
      else if(strcmp(line, "./secretcode") == 0)
      {
	std::cout << "Enter your secret code: ";
	std::cin >> c.secret_code_;
      }
      else
      {
        chat_message msg;
        msg.body_length(std::strlen(line));

        //add tags to message
        line[msg.body_length()] = c.chatroom_;
        line[msg.body_length() + 1] = '0';
        line[msg.body_length() + 2] = '0';
        line[msg.body_length() + 3] = '0';
        line[msg.body_length() + 4] = '0';
        line[msg.body_length() + 5] = '\0';
        //re calculate msg.body_length()
        msg.body_length(std::strlen(line));
        std::memcpy(msg.body(), line, msg.body_length());
        msg.encode_header();
        c.write(msg);
      }
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
