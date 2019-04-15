#include <ncurses.h>
#include <curses.h>
#include <string>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv){

//initialised the screen
//sets up memory
    initscr();
    cbreak();  //cntrl c exits the program
    noecho();

    int height,width, start_y, start_x;

    height = 24;
    width = 80;
    start_y = 0;
    start_x = 0;
    WINDOW * win = newwin(height, width, start_y, start_x);
    box(win,0,0);
    refresh();
    wrefresh(win);
    keypad(win,true);
    string choices[9] = {"Create Chatroom","Block User","Decrypt Message","Delete Message","Chatrooms List","LOBBY","Nick Name:","type>>","Attachment"};

    int choice;
    int highlight = 0;
    int i;

    while(1){
        for(i=0;i<9;i++){
            if(i == highlight){
                wattron(win,A_STANDOUT);
            }

            if(i == 0){
                WINDOW * win1 = newwin(3, 20, 0, 0);
                box(win1,0,0);
                refresh();
                wrefresh(win1);
                mvwprintw(win, 1, 1, choices[0].c_str());
            }

            if(i==1){
                WINDOW * win2 = newwin(3, 20, 0, 20);
                box(win2,0,0);
                refresh();
                wrefresh(win2);
                mvwprintw(win, 1, 21, choices[1].c_str());
            }
            if(i== 2){
                WINDOW * win3 = newwin(3, 20, 0, 40);
                box(win3,0,0);
                refresh();
                wrefresh(win3);
                mvwprintw(win, 1, 41, choices[2].c_str());
            }
            if(i==3){
                WINDOW * win4 = newwin(3, 20, 0, 60);
                box(win4,0,0);
                refresh();
                wrefresh(win4);
                mvwprintw(win, 1, 61, choices[3].c_str());
            }
            if(i==4){
                WINDOW * win5 = newwin(21, 20, 3, 0);
                box(win5,0,0);
                refresh();
                wrefresh(win5);
                mvwprintw(win, 4, 1, choices[4].c_str());
            }
            if(i==5){
                WINDOW * win6 = newwin(3, 60, 3, 20);
                box(win6,0,0);
                refresh();
                wrefresh(win6);
                mvwprintw(win, 4, 45, choices[5].c_str());
            }
            if(i==6){
                WINDOW * win7 = newwin(2, 12, 22, 20);
                box(win7,0,0);
                refresh();
                wrefresh(win7);
                mvwprintw(win, 22, 21, choices[6].c_str());
            }
            if(i==7 ){
                WINDOW * win8 = newwin(2, 36, 22, 32);
                box(win8,0,0);
                refresh();
                wrefresh(win8);
                mvwprintw(win, 22, 33, choices[7].c_str());
            }
            if(i==8){
                WINDOW * win9 = newwin(2, 12, 22, 68);
                box(win9,0,0);
                refresh();
                wrefresh(win9);
                mvwprintw(win, 22, 69, choices[8].c_str());
            }

            wattroff(win,A_STANDOUT);
        }



        choice = wgetch(win);
        switch(choice){
            case KEY_RIGHT:
                highlight++;
                if(highlight > 8){
                    highlight = 8;
                }
                break;

            case KEY_LEFT:
                highlight--;
                if(highlight < 0){
                    highlight = 0;
                }
                break;

            case KEY_UP:
                if(highlight == 6 || highlight == 7 || highlight == 8){
                    highlight = 4;break;
                }

                if(highlight == 4 || highlight == 5){
                    highlight = 0;break;
                }
                break;

            case KEY_DOWN:

                if(highlight == 0 || highlight == 1 || highlight == 2 || highlight == 3){
                    highlight = 4;break;
                }

                if(highlight == 4 || highlight == 5){
                    highlight = 6;break;
                }
                break;



            default:
                break;
        }
        if(choice == 10){
            break;
        }

    }

    getch();

    endwin();

    return 0;
}
