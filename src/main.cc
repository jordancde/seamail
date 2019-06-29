
#include <ncurses.h>

int main()
{	
	initscr();			
	raw();				
	keypad(stdscr, TRUE);		
	noecho();			

        bool a = false;
        for(char c = 'A'; c <= 'Z'; ++c) {
            if(a)       attron(A_BOLD);
            else        attroff(A_UNDERLINE);
            printw("%c", c);
            a = !a;
        }
	refresh();			
    	getch();			
	endwin();			

	return 0;
}
