#include <ncurses.h> 
#include <string>
using namespace std;

int main(int argc, char** argv)
{

//initialised the screen
//sets up memory 
initscr();

int height,width, start_y, start_x;
cbreak(); //cntrl c exits the program
//main win
start_y = start_x = 1;
getmaxyx(stdscr,height,width);
height = height- 5;
width = width -5;
WINDOW * win = newwin(height, width, start_y, start_x);
refresh();
// main win box
box(win, 0, 0);
mvwprintw(win, 1, 3, "Lobby");
wrefresh(win);
//
WINDOW * win1 = newwin(5,width-4 , height-8,  3);
refresh();
box(win1, 0, 0);
wrefresh(win1);
// print win
WINDOW * pwin1 = newwin(45,165 ,4,32);
refresh();
box(pwin1, 0, 0);
wrefresh(pwin1);
refresh();

// keypad menu window
WINDOW * mwin = newwin(45,27 ,4,3);
refresh();
box(mwin, 0, 0);
wrefresh(mwin);
//meanu system
int i=0;
keypad(mwin,true);
string choices[3] ={"Walk","Run","Send Message"};
int choice;
int highlight = 0;
LOOP:
while(1){
	for(i=0;i<3;i++){
		if(i == highlight){
 		wattron(mwin, A_REVERSE);
                mvwprintw(mwin, i+1,1,choices[i].c_str());
		wattroff(mwin, A_REVERSE);
                }
	}
	choice = wgetch(mwin);

	switch(choice)
	{
     	 case KEY_UP:
		highlight--;
		break;
		case KEY_DOWN:
		highlight++;
		break;
		default:
		break;
	}
		if(choice == 10)
		break;
	}
		



int message_number =0;
int size=0;
int message[500];
int send_message = 0;
if(highlight == 2){
send_message = 1;
}
while(send_message){
i = 0;
size=0;
move(height-7,4);

wrefresh(win1);
wrefresh(win);

while(1){



 		message[i] = getch();
 		 if(message[i] == '\n'){
   		 endwin();
                  break;}
                 if(message[i] == '+'){
   		 endwin();
                 goto LOOP;
                  }
		 i++;
		size++;
		refresh();
}
wrefresh(win1);
	message_number++;
	move(message_number+5,35);
	for(i=0;i<size;i++){
           
   	printw("%c",message[i]);
	}
refresh();
printw("\n");
}

getch();
endwin();
return 0;
}
