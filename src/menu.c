#include <ncurses.h>
#include <stdlib.h>
#include "main.c"



#define ENTER 10
#define ESCAPE 27
void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    init_pair(2,COLOR_BLACK,COLOR_WHITE);
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);
}

WINDOW **draw_menu(int start_col)
{

    WINDOW **items;
    items=(WINDOW **)malloc(4*sizeof(WINDOW *));

    items[0]=newwin(5,19,1,start_col);
    wbkgd(items[0],COLOR_PAIR(2));
    box(items[0],ACS_VLINE,ACS_HLINE);
    items[1]=subwin(items[0],1,17,2,start_col+1);
    wprintw(items[1],"LEVEL 1",1);
    items[2]=subwin(items[0],1,17,3,start_col+1);
    wprintw(items[2],"LEVEL 2",2);
    items[3]=subwin(items[0],1,17,4,start_col+1);
    wprintw(items[3],"INFO",3);



    wbkgd(items[1],COLOR_PAIR(1));
    wrefresh(items[0]);
    return items;
}
void delete_menu(WINDOW **items,int count)
{
    int i;
    for (i=0;i<count;i++)
        delwin(items[i]);
    free(items);
}
int scroll_menu(WINDOW **items,int count,int menu_start_col)
{
    int key;
    int selected=0;
    while (1) {
        key=getch();
        if (key==KEY_DOWN || key==KEY_UP) {
            wbkgd(items[selected+1],COLOR_PAIR(2));
            wnoutrefresh(items[selected+1]);
            if (key==KEY_DOWN) {
                selected=(selected+1) % count;
            } else {
                selected=(selected+count-1) % count;
            }
            wbkgd(items[selected+1],COLOR_PAIR(1));
            wnoutrefresh(items[selected+1]);
            doupdate();
        }else if (key==ESCAPE) {
            return -1;
        } else if (key==ENTER) {
            return selected;
        }
    }
}
int main()
{
    int key;
    WINDOW *messagebar;

    init_curses();

    bkgd(COLOR_PAIR(1));
    messagebar=subwin(stdscr,1,79,25,100);
    move(0,0);
    printw("Press ESC quits.");
    refresh();

    do {
        int selected_item;
        WINDOW **menu_items;

        werase(messagebar);
        wrefresh(messagebar);

        menu_items=draw_menu(100);
        selected_item=scroll_menu(menu_items,3,100);
        delete_menu(menu_items,4);
        if (selected_item<0)
            wprintw(messagebar,"You haven't selected any item.");
        else if(selected_item==0)
            //wprintw(messagebar,"Let's play lvl 1",1);
            main();
        else if(selected_item==1)
            wprintw(messagebar,"Let's play lvl 2",2);
        else if(selected_item==2)
            wprintw(messagebar,"Mario game created by Lucas Mouquet and Antoine Vandeplanque",3);



        touchwin(stdscr);
        key=getch();
        refresh();

    }
    while (key!=ESCAPE);

    delwin(messagebar);
    endwin();
    return 0;
}