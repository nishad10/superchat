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
#include <ncurses.h>
#include <curses.h>
#include <string>
#include <bits/stdc++.h>

char nickname[25];
using namespace std;
using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
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
	  
          if (!ec)
          {
            
            read_msg_.body()[read_msg_.body_length() -1] = '\0';
            std::cout.write(read_msg_.body(), read_msg_.body_length())<< "\n";
            
            do_read_header();
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

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
//

    initscr();
    cbreak();  //cntrl c exits the program
    int height,width, start_y, start_x;
    int st_x = 40;
    int st_y = 20;
   
    nickname_label: 
    WINDOW * nic_win = newwin(3, 40, st_y, st_x);
    box(nic_win,0,0);
    refresh();
    wrefresh(nic_win);
    move(21,41);
    printw("nickname:");
    getstr(nickname);
    
    endwin();
    //wrefresh(nic_win);
    refresh();

    getmaxyx(stdscr,height,width);
    //height = height - 5;
    start_y = 0;
    start_x = 0;
    WINDOW * win = newwin(height, width, start_y, start_x);
    box(win,0,0);
    refresh();
    wrefresh(win);
    WINDOW * twin = newwin(3, width-20, height-3, 20);
    box(twin,0,0);
    refresh();
    wrefresh(twin);
    WINDOW * text_Center = newwin(height-12, width-14, 6, 0);
    box(text_Center,0,0);
    refresh();
    wrefresh(text_Center);
    keypad(win,true);
    string choices[10] = {"Create Chatroom","Block User","Decrypt Message","Delete Message","Attachment","Exit","Chatrooms List","LOBBY","Nick Name:","  Send Message >>"};
    choices[8] = nickname;
    int choice;
    int highlight = 0;
    int i;
    int sendmessage = 0;
    LOOP:
    while(1){
        for(i=0;i<10;i++){
            if(i == highlight){
                wattron(win,A_STANDOUT);
            }

            if(i == 0){//create chatroom
                WINDOW * win1 = newwin(3, 17, 0, 0);
                box(win1,0,0);
                refresh();
                wrefresh(win1);
                mvwprintw(win, 1, 1, choices[0].c_str());
            }

            if(i==1){//block user
                WINDOW * win2 = newwin(3, 12, 0, 17);
                box(win2,0,0);
                refresh();
                wrefresh(win2);
                mvwprintw(win, 1, 18, choices[1].c_str());
            }
            if(i== 2){//decrypt message
                WINDOW * win3 = newwin(3, 17, 0, 29);
                box(win3,0,0);
                refresh();
                wrefresh(win3);
                mvwprintw(win, 1, 30, choices[2].c_str());
            }
            if(i==3){//Delete message
                WINDOW * win4 = newwin(3, 16, 0, 46);
                box(win4,0,0);
                refresh();
                wrefresh(win4);
                mvwprintw(win, 1, 47, choices[3].c_str());
            }
            if(i==4){//Attachment
                WINDOW * win9 = newwin(3, 12, 0, 62);
                box(win9,0,0);
                refresh();
                wrefresh(win9);
                mvwprintw(win, 1, 63, choices[4].c_str());
            }
            if(i==5){//Exit
                WINDOW * win9 = newwin(3, width-74, 0, 74);
                box(win9,0,0);
                refresh();
                wrefresh(win9);
                mvwprintw(win, 1, 75, choices[5].c_str());
            }
            if(i==6){//Chatroom List
                //WINDOW * win5 = newwin(height-3, 20, 3, 0);
                //box(win5,0,0);
                refresh();
                //wrefresh(win5);
                //mvwprintw(win, 4, 1, choices[6].c_str());
            }
            if(i==7){//Lobby
                WINDOW * win6 = newwin(3, width-20, 3, 0);
                box(win6,0,0);
                refresh();
                wrefresh(win6);
                mvwprintw(win, 4, 45, choices[7].c_str());
            }
            if(i==8){//Nick Name
                WINDOW * win7 = newwin(3, 20, height-6, 0);
                box(win7,0,0);
                refresh();
                wrefresh(win7);
                mvwprintw(win, height-5, 1, choices[8].c_str());
            }
            if(i==9){//Send Message>>
                WINDOW * win8 = newwin(3, 20, height-3, 0);
                box(win8,0,0);
                refresh();
                wrefresh(win8);
                mvwprintw(win, height-2, 4, choices[9].c_str());
            }

            wattroff(win,A_STANDOUT);
        }
        //move(height-2,21);



        choice = wgetch(win);
        switch(choice){
            case KEY_RIGHT:
                highlight++;
                if(highlight > 9){
                    highlight = 9;
                }
                break;

            case KEY_LEFT:
                highlight--;
                if(highlight < 0){
                    highlight = 0;
                }
                break;

            case KEY_UP:
                if(highlight == 6 || highlight == 7){
                    highlight = 0;break;
                }

                if(highlight == 8){
                    highlight = 6;break;
                }
                if(highlight == 9){
                    highlight = 8;break;
                }
                break;

            case KEY_DOWN:

                if(highlight == 0 || highlight == 1 || highlight == 2 || highlight == 3 || highlight == 4 || highlight == 5){
                    highlight = 6;break;
                }

                if(highlight == 6 || highlight == 7){
                    highlight = 8;break;
                }
                if(highlight == 8){
                    highlight = 9;break;
                }
                break;



            default:
                break;
        }
        if(choice == 10){
            break;
        }

    }


    if(highlight==5){
        endwin();
        return 0;
    }

    if(highlight==8){
    goto nickname_label;
    }
    if(highlight == 9){
        sendmessage = 1;
        int message_number =0;
        int size=0;
        char message[500];

        i = 0;
        while(sendmessage == 1){
            i = 0;
            size = 0;

            move(8,0);

            while(1){

                message[i] = getch();
                //if()

                if(message[i] == '\n'){
                    //werase(twin);
                    //WINDOW * twin = newwin(3, width-20, height-3, 20);
                    //box(twin,0,0);
                    //refresh();
                    //wrefresh(twin);
                    break;
                }
                if(message[i] == '~'){
                    sendmessage = 0;
                    highlight = 0;
                    goto LOOP;
                }

                i++;
                size++;
                refresh();
            }
int nickname_length=strlen(nickname);
for(i=0;i< nickname_length;i++){
line[i]= nickname[i];
}
for(i= nickname_length;i<10;i++){
line[i] =' ';
}

for(i=10;i<size+10;i++){
line[i]= message[i-10];
}
 chat_message msg;
printw("\n");
      msg.body_length(std::strlen(line));
      line[msg.body_length()] = '0';
      line[msg.body_length() + 1] = '\0';
      msg.body_length(std::strlen(line));
      /*for(int i = msg.body_length(); i >= 0; i--)
	line[i+1] = line[i];
      line[0] = '0';*/
      //std::cout << "You sent:" << line << std::endl;
      std::memcpy(msg.body(), line, msg.body_length());
for(i=0;i<size+10;i++){
line[i]= '\0';
}
      msg.encode_header();
      c.write(msg);
      printf("\n");

            //message_number++;
            //move(6+message_number,21);
            //printw("%s: ",nickname);
	   /* for(i=0;i<size;i++){
                if(message_number<height-9){
                    //printw("%c",message[i]);
                }
                else{
                    werase(text_Center);
                    WINDOW * text_Center = newwin(height-9, width-20, 6, 20);
                    box(text_Center,0,0);
                    refresh();
                    wrefresh(text_Center);
                    //move(5+message_number,21);
                    message_number = 0;
                    break;
                }
            }*/

            refresh();
            //printw("\n");
        }
    }

    getch();

    endwin();

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
