#include <ncurses.h>
#include <curses.h>
#include <string>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>


using namespace std;

string login ()
{
	 char getInput []="Pick a cool name: ";		/* message to be appeared on the screen */
	 char str[25];
	 vector <string> Names ;
	 int row,col;				/* to store the number of rows and *
						 * the number of colums of the screen */
	 initscr();				/* start the curses mode */
			/* get the number of rows and columns */
	 bool flagInput=true;
		while(flagInput)
		{
			 bool flag= true;
			getmaxyx(stdscr,row,col);
			 WINDOW * Outside = newwin(10, 55,(row/2)-5,((col- 20)/2)-5);
			 refresh();
			 box(Outside, 0, 0);
			mvprintw(row/2,(col- 20)/2,"%s",getInput);
			wrefresh(Outside);
			getstr(str);
			//wrefresh(Outside);
			string name(str);
				
			//for (int i=0;i<Names.size();i++)
			for (auto i: Names)
			{
				if (i == name)
				{
					mvprintw((row/2)+3,((col- 20)/2),"Sorry! Name already taken");
					getch();
					flag=false;
					wrefresh(Outside);
					int wclear(WINDOW *Outside);
					//wrefresh(Outside);
					//refresh();
					endwin();
					break;
				}
			}
			if(flag)
				{
					Names.push_back(name);
					mvprintw((row/2)+3,((col- 20)/2), "Name you chose is: %s", str);
					getch();
					wrefresh(Outside);
					int wclear(WINDOW *Outside);
					//wrefresh(Outside);
					//refresh();
					endwin();
					flagInput=false;
					return str;
				}
		}
		//wrefresh(Outside);
		getch();
		endwin();
}

int main(int argc, char** argv){

//initialised the screen
//sets up memory
	string name=login();
    initscr();
    cbreak();  //cntrl c exits the program
    int height,width, start_y, start_x;

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
    WINDOW * text_Center = newwin(height-9, width-20, 6, 20);
    box(text_Center,0,0);
    refresh();
    wrefresh(text_Center);
    keypad(win,true);
    string choices[10] = {"Create Chatroom","Block User","Decrypt Message","Delete Message","Attachment","Exit","Chatrooms List","LOBBY","Nick Name:","  Send Message >>"};
    choices[8] = name;
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
                WINDOW * win5 = newwin(height-3, 20, 3, 0);
                box(win5,0,0);
                refresh();
                wrefresh(win5);
                mvwprintw(win, 4, 1, choices[6].c_str());
            }
            if(i==7){//Lobby
                WINDOW * win6 = newwin(3, width-20, 3, 20);
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

    if(highlight == 9){
        sendmessage = 1;
        int message_number =0;
        int size=0;
        char message[500];

        i = 0;
        while(sendmessage == 1){
            i = 0;
            size = 0;

            move(height-2,21);

            while(1){

                message[i] = getch();
                //if()

                if(message[i] == '\n'){
                    werase(twin);
                    WINDOW * twin = newwin(3, width-20, height-3, 20);
                    box(twin,0,0);
                    refresh();
                    wrefresh(twin);
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

            message_number++;
            move(6+message_number,21);
            for(i=0;i<size;i++){
                if(message_number<height-9){
                    printw("%c",message[i]);
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
            }

            refresh();
            printw("\n");
        }
    }

    getch();

    endwin();

    return 0;

}
