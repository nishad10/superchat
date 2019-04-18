#include "chatroom.h"



  chatroom::chatroom(

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
          if (!ec)
          {
            room_.deliver(read_msg_);
            do_read_header();
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
/*
chatroom::chatroom(std::string name1, bool ispublic1, int port1)
    {
    name=name1;
    is_public=ispublic1;
    port=port1;
    }
	void chatroom::join_chatroom(clientNew cnew)
	{
    clientList.push_back(cnew);
	}
	void chatroom::record_history(message myMessage)
	{
		std::string msg=message::decode_body(myMessage);
        messageList.push_back(msg);
	}
	bool chatroom::get_is_empty()
	{
    return is_empty;
	}
	bool chatroom::get_is_lobby()
	{
	return Lobby;	
	}
	bool chatroom::get_is_public()
	{
	return is_public;	
	}
	void chatroom::set_is_empty(bool is_empty1)
	{
    is_empty=is_empty1;
	}
	void chatroom::set_is_lobby(bool Lobby1)
	{
    Lobby=Lobby1;
	}
	void chatroom::set_is_public(bool is_public1)
	{
    is_public=is_public1;
	}
	std::string chatroom::display_previous(chatroom Chatroom)
	{
    int j=messageList.size();
    for(i=0;i<j;i++)
    {
        return messageList(j--)
    }
    
	}
	std::string chatroom::get_name()
	{
    return name;
	}
	void chatroom::set_name(std::string name1)
	{
    name=name1;
	}
	int chatroom::get_port()
	{
    return port;
	}*/


	