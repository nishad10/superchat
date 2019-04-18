#include <ncurses.h> 
using namespace std;

int main(int argc, char** argv)
{
//initialised the screen
//sets up memory 
initscr();

int height,width, start_y, start_x;
height = 40;
width = 150;
start_y = start_x = 1;
WINDOW * win = newwin(height, width, start_y, start_x);
refresh();
box(win, 0, 0);
mvwprintw(win, 1, 1, "Lobby");
wrefresh(win);

height = 3;
width = 145;
start_y = 35; 
start_x = 1;
WINDOW * win1 = newwin(height, width, start_y, start_x);
refresh();
box(win1, 0, 0);
mvwprintw(win1, 1, 1, "type message here:");
wrefresh(win1);
 
int c= getch();
getch();
endwin();
return 0;
}
