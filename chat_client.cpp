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
#include <time.h>
 int height,width;
WINDOW * twin;
char nickname[25];
int uni_message = 8;
int enc = 0;
using namespace std;
using asio::ip::tcp;
void spellcheck(string message,int width,int height);
int edit_distance(string first_string, string second_string);


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
            time_t ltime;
            ltime = time(NULL);
           read_msg_.body()[read_msg_.body_length() -1] = '\0';
		move(uni_message, 0);
	//if(read_msg_.body()[read_msg_.body_length()-1]==0)

  		for(int i =0 ; i<10;i++){	    
		printw("%c",(read_msg_.body()[i]));
                }
                for(int i =10 ; i<read_msg_.body_length();i++){	    
		printw("%c",(read_msg_.body()[i]));

                }
                printw(" %s \n",asctime(localtime(&ltime)));
                
            refresh(); 
  
             werase(twin);
                     twin = newwin(3, width-20, height-3, 20);
                    box(twin,0,0);
                    refresh();
                    wrefresh(twin);
            //std::cout.write(read_msg_.body(), read_msg_.body_length());
            
            do_read_header();
            //
            uni_message++;
          }
          else
          {
            socket_.close();
          }
          move(height-2,25);
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
          
//

    initscr();
    cbreak();  //cntrl c exits the program
    int  start_y, start_x;
    int st_x = 40;
    int st_y = 20;
   
     
    WINDOW * nic_win = newwin(3, 40, st_y, st_x);
    box(nic_win,0,0);
    refresh();
    wrefresh(nic_win);
    move(21,41);
    printw("nickname:");
    getstr(nickname);
//
  
asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
  //  nickname_label:

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
    twin = newwin(3, width-20, height-3, 20);
    box(twin,0,0);
    refresh();
    wrefresh(twin);
    WINDOW * text_Center = newwin(height-12, width-14, 6, 0);
    box(text_Center,0,0);
    refresh();
    //wrefresh(text_Center);
    keypad(win,true);
nickname_label:    
string choices[10] = {"Create Chatroom","Encrypt","Decrypt","Delete Message","Attachment","Exit","Chatrooms List","LOBBY","Nick Name:","Send/Receive"};
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

            if(i==1){//Encrypt
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

    if(highlight==1){
	if(enc!=0){
        enc = 0;
        }
else{
        WINDOW *encr; 
	encr = newwin(10, 40, height - 18,width - 40 );
        box(encr,0,0);
    	refresh();
   	wrefresh(encr);
    	move(height - 17,width - 39);
    	printw("Enter a secret no:");
        char encc = getch();    
        enc =  encc - '0';
}

        goto LOOP;  
     }


    if(highlight==2){
    char dm[20];
    WINDOW *de_win;
    de_win = newwin(10, 40, height - 18,width - 40 );
    box(de_win,0,0);
    refresh();
    wrefresh(de_win);
    move(height - 17,width - 39);
    
    printw("Enter a secret no:");
        char encc = getch();    
        enc =  encc - '0';
    i =0 ;
    int chk =0;
move(height - 16,width - 39);
printw("Message to derypt:");
    while(chk==0){
    dm[i]= getch();
   
    		if(dm[i]=='\n'){
		move(height - 16,width - 39);
		 printw ("Decrypted message: ");
		    for(i=0;i<strlen(dm);i++){
			    printw("%c",dm[i]-enc);
			    }
            
    //werase(de_win);
    //wrefresh(de_win);
    getch();
    
     			chk = 1;
goto LOOP;
    		}
 i++;
    }   
 
   
    }
    if(highlight==5){
        endwin();
        return 0;
    }

    if(highlight==8){
    //goto nickname_label;
  
    nic_win = newwin(3, 40, st_y, st_x);
    box(nic_win,0,0);
    refresh();
    wrefresh(nic_win);
    move(21,41);
    printw("nickname:");
    getstr(nickname);
    werase(nic_win);
    wrefresh(nic_win);
    goto nickname_label;

    
    }
  if(highlight == 9){
        sendmessage = 1;
        int message_number =0;
        int size=0;
        char message[500];
        string copy_m;
        char ch;

        i = 0;
        int r;
        while(sendmessage == 1){
            i = 0;
            size = 0;
            int space_count = 0;
            copy_m = "";
            move(height-2,21);
          
           while(1){

                ch = getch();

                if(ch == '\n'){
                    werase(twin);
                    WINDOW * twin = newwin(3, width-20, height-3, 20);
                    box(twin,0,0);
                    refresh();
                    wrefresh(twin);
                    break;
                }

                else if(ch == '~'){
                    sendmessage = 0;
                    highlight = 0;
                    goto LOOP;
                }

                 else if(ch == ' '){
                    space_count = i+1;

                    copy_m = "";
                }

                else if(ch == ' '){
                    space_count = i+1;

                    copy_m = "";
                }
                message[i] = ch+enc;
                copy_m = copy_m + message[i];
                spellcheck(copy_m,width,height);

                i++;
                size++;
                refresh();
            }

if(message[0]=='!'){
goto LOOP;
}


size = strlen(message);
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
//for(i=0;i<size+10;i++){
//line[i]= '\0';
//}
      msg.encode_header();
      //move(uni_message+1,0);
	
      c.write(msg);
for(i=0;i<size;i++){
message[i]='\0';
}
for(i=0;i<size+10;i++){
line[i]='\0';
}
	//wprintw(text_Center,"\b");
	//wrefresh(text_Center);
   //   move(uni_message,0);
 
     // printf("\n");

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
//

void spellcheck(string message,int width,int height){
	FILE *fp1 = fopen("dict.txt","r");

	int dict_line;
	char dict_word[500];
	int i = 0;
	int j = 0;

	if(fp1 == NULL){
        printf("Can't open the file\n");
        exit(1);
	}

    char temp2;

    fscanf(fp1,"%d",&dict_line);

    fscanf(fp1,"%c",&temp2);

    char Dict_arr[dict_line][500];

    while(fgets(dict_word,500,fp1) != NULL){
            strcpy(Dict_arr[j],dict_word); //strcpy
            j++;
    }

    int Min_arr[dict_line];
    int editDist, minimum_ed;
    int min_counter=0;
    int k;

        for(j=0;j<dict_line;j++){
            editDist = edit_distance(message,Dict_arr[j]);
            if(j==0){
                minimum_ed = editDist;
                Min_arr[min_counter]= j;
                min_counter++;
            }
            else{
                if (editDist == minimum_ed){
                    Min_arr[min_counter]= j;
                    min_counter++;
                }

                else if(editDist < minimum_ed){
                    minimum_ed = editDist;
                    min_counter = 0;
                    Min_arr[min_counter]= j;
                    min_counter++;
                }
            }


        }
        WINDOW * spell = newwin(3, width-20, height-5, 20);
        box(spell,0,0);
        refresh();
        wrefresh(spell);
        mvwprintw(spell, 1, 1, "Spell Check>> ");
        wrefresh(spell);

        for(k=0;k<min_counter;k++){
            if(k==0){
                mvwprintw(spell,1,15,Dict_arr[Min_arr[k]]);
                wrefresh(spell);
            }
            if(k==1){
                mvwprintw(spell,1,30,Dict_arr[Min_arr[k]]);
                wrefresh(spell);
            }
            if(k==2){
                mvwprintw(spell,1,45,Dict_arr[Min_arr[k]]);
                wrefresh(spell);
            }

            if(k==3){
                mvwprintw(spell,1,60,Dict_arr[Min_arr[k]]);
                wrefresh(spell);
            }
        }
    werase(spell);
    fclose(fp1);

}

int edit_distance(string first_string, string second_string){
    int i,j;
    int len1 = first_string.size();
    int len2 = second_string.size();
    int arr[len1+1][len2+1];
    int min;
    for(i=0;i<=len1;i++){
        for(j=0;j<=len2;j++){
            if (i==0 && j==0){
                arr[i][j] = 0;
            }
            else if(i==0 ){
                arr[i][j] = arr[0][j-1]+1;
            }
            else if(j==0){
                arr[i][j] = arr[i-1][0] +1;
            }
            else{
                if(first_string[i-1] == second_string[j-1]){
                    if(arr[i-1][j]+1 < arr[i][j-1]+1 && arr[i-1][j]+1 < arr[i-1][j-1]+0){
                        min = arr[i-1][j]+1;
                    }
                    else if(arr[i-1][j]+1 > arr[i][j-1]+1 && arr[i][j-1]+1 < arr[i-1][j-1]+0){
                        min = arr[i][j-1]+1;
                    }
                    else{
                        min = arr[i-1][j-1]+0;
                    }
                }

                if(first_string[i-1] != second_string[j-1]){
                    if(arr[i-1][j]+1 < arr[i][j-1]+1 && arr[i-1][j]+1 < arr[i-1][j-1]+1){
                        min = arr[i-1][j]+1;//top
                    }
                    else if(arr[i-1][j]+1 > arr[i][j-1]+1 && arr[i][j-1]+1 < arr[i-1][j-1]+1){
                        min = arr[i][j-1]+1;//left
                    }
                    else{
                        min = arr[i-1][j-1]+1;
                    }
                }
                arr[i][j] = min;
            }

        }
    }

    for(i=0;i<=len1;i++){
        for (j=0;j<=len2;j++){
            if(i == len1 && j == len2){
                return arr[i][j];
            }
        }
    }
}
