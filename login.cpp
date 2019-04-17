
#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

/*
int CheckName (vector <string> Name, string name, vector <string> Ban)
{
	for (auto i: Names)
	{
		if(Names[i]==name)
		{
			mvprintw((row/2)+3,((col- 20)/2),"Sorry! Name already taken");
					getch();
					flag=false;
					wrefresh(Outside);
					int wclear(WINDOW *Outside);
					//wrefresh(Outside);
					//refresh();
					endwin();
					
		return 2;
		break;
		}
		else if (Ban[i]==name)
		{
			mvprintw((row/2)+3,((col- 20)/2),"Sorry! You can't enter");
					getch();
					flag=false;
					wrefresh(Outside);
					int wclear(WINDOW *Outside);
					//wrefresh(Outside);
					//refresh();
					endwin();
					
		return 1;
		break;
		}
		else
		{
			return 0;
		}
	}
}
*/
 
int main()
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
					//flagInput=false;
				}
		}
		//wrefresh(Outside);
		getch();
		endwin();

 return 0;
}
