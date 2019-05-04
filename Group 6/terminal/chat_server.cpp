//
// chat_server.cpp
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
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:

  int chat_room_list_[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};

  void join(chat_participant_ptr participant)
  {
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->deliver(msg);
  }

  void leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
  }

  void deliver(const chat_message& msg)
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto participant: participants_)
      participant->deliver(msg);
  }

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:

  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
  {
  }

  void start()
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message& msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          std::cout << "Message tags: " << read_msg_.body()[read_msg_.body_length() - 5] << read_msg_.body()[read_msg_.body_length() - 4]
				        << read_msg_.body()[read_msg_.body_length() - 3] << read_msg_.body()[read_msg_.body_length() - 2]
				        << read_msg_.body()[read_msg_.body_length() - 1] << std::endl;
	  // Check if the message is not a server command
          if (read_msg_.body()[read_msg_.body_length() - 5] != 'S')
	  {
	    if (!ec)
            {
              room_.deliver(read_msg_);
              do_read_header();
            }
	    else
            {
              room_.leave(shared_from_this());
            }
	  }
	  // else, check if it *is* a server command
	  else if (read_msg_.body()[read_msg_.body_length() - 5] == 'S')
	  {
	    if (!ec)
            {
	      // check what type of command it is
	      if (read_msg_.body()[read_msg_.body_length() - 4] == '0' && read_msg_.body()[read_msg_.body_length() - 3] == '1')
	      {
	        char line[chat_message::max_body_length + 1];
	        std::string s = "Chatroom failed to be created";
	        for(int i = 1; i < 10; i++)
	        {
		  if(room_.chat_room_list_[i] == -1)
		  {
		    room_.chat_room_list_[i] = 0;
		    s = "Chatroom " + std::to_string(i) + " created";
		    break;
		  }
	        }
	        chat_message response_msg;

	        strcpy(line, s.c_str());
	        response_msg.body_length(std::strlen(line));

	        //add tags to message
	        line[response_msg.body_length()] = 'S';
	        line[response_msg.body_length() + 1] = '0';
	        line[response_msg.body_length() + 2] = '1';
	        line[response_msg.body_length() + 3] = '0';
	        line[response_msg.body_length() + 4] = '0';
	        line[response_msg.body_length() + 5] = '\0';
	        //re calculate msg.body_length()
	        response_msg.body_length(std::strlen(line));
	        std::memcpy(response_msg.body(), line, response_msg.body_length());
	        response_msg.encode_header();

		for(int i = 0; i < 10; i++)
		  std::cout << room_.chat_room_list_[i] << " ";
		std::cout << "\n";
                room_.deliver(response_msg);
                do_read_header();
              }
	      else if (read_msg_.body()[read_msg_.body_length() - 4] == '0' && read_msg_.body()[read_msg_.body_length() - 3] == '2')
	      {
	        char line[chat_message::max_body_length + 1];
	        std::string s = "Chatroom failed to be deleted";
		int i = int(read_msg_.body()[0]) - '0';
		if(room_.chat_room_list_[i] == 0)
		{
		  room_.chat_room_list_[i] = -1;
		  s = "Chatroom " + std::to_string(i) + " deleted";
		}
	        chat_message response_msg;

	        strcpy(line, s.c_str());
	        response_msg.body_length(std::strlen(line));

	        //add tags to message
	        line[response_msg.body_length()] = 'S';
	        line[response_msg.body_length() + 1] = '0';
	        line[response_msg.body_length() + 2] = '2';
	        line[response_msg.body_length() + 3] = '0';
	        line[response_msg.body_length() + 4] = '0';
	        line[response_msg.body_length() + 5] = '\0';
	        //re calculate msg.body_length()
	        response_msg.body_length(std::strlen(line));
	        std::memcpy(response_msg.body(), line, response_msg.body_length());
	        response_msg.encode_header();

		for(int i = 0; i < 10; i++)
		  std::cout << room_.chat_room_list_[i] << " ";
		std::cout << "\n";
                room_.deliver(response_msg);
                do_read_header();
              }
	      else if (read_msg_.body()[read_msg_.body_length() - 4] == '0' && read_msg_.body()[read_msg_.body_length() - 3] == '4')
	      {
	        char line[chat_message::max_body_length + 1];
		int i = int(read_msg_.body()[0]) - '0';
		int j = int(read_msg_.body()[1]) - '0';
		if(room_.chat_room_list_[i] == -1)
		{
		  line[0] = 'X';
		}
		else
		{
		  line[0] = read_msg_.body()[0];
		  // decrement amount of users in leaving chatroom by 1
		  room_.chat_room_list_[j]--;
		  // increment amount of users in joining chatroom by 1
		  room_.chat_room_list_[i]++;
		}
	        chat_message response_msg;
	        response_msg.body_length(std::strlen(line));

	        //add tags to message
	        line[response_msg.body_length()] = 'S';
	        line[response_msg.body_length() + 1] = '0';
	        line[response_msg.body_length() + 2] = '4';
	        line[response_msg.body_length() + 3] = '0';
	        line[response_msg.body_length() + 4] = '0';
	        line[response_msg.body_length() + 5] = '\0';
	        //re calculate msg.body_length()
	        response_msg.body_length(std::strlen(line));
	        std::memcpy(response_msg.body(), line, response_msg.body_length());
	        response_msg.encode_header();
		/**/std::cout << response_msg.body() << std::endl;

		for(int i = 0; i < 10; i++)
		  std::cout << room_.chat_room_list_[i] << " ";
		std::cout << "\n";
                room_.deliver(response_msg);
                do_read_header();
              }
	      else
              {
                room_.leave(shared_from_this());
              }
	    }
	    else
            {
              room_.leave(shared_from_this());
            }
	  }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
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
            room_.leave(shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  chat_room& room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
  chat_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

//----------------------------------------------------------------------

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

    std::list<chat_server> servers;
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
