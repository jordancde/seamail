
#include "graphics/toolbar.h"
#include <iostream>

Toolbar::Toolbar(size_t line, 
    std::list<std::string> menuItems, std::function<void(std::string)> handler) 
    : line(line), menuItems(std::move(menuItems)),
        handler(handler) {
    selected = this->menuItems.begin();
}

void Toolbar::onDraw(bool isActive) const {
    wmove(win, 0, 0);
    if(isActive)
        wattron(win, A_REVERSE);
    waddch(win, ACS_VLINE);
    wattroff(win, A_REVERSE);
    for(auto item = menuItems.begin(); item != menuItems.end(); ++item) {
        if(item == selected)
            wattron(win, A_REVERSE);
        wprintw(win, " %s ", item->c_str());
        wattroff(win, A_REVERSE);
        waddch(win, ACS_VLINE);
    }
}

void Toolbar::onInput(int key) {
    switch(key){
    case KEY_LEFT:
    case 'h':
        if(selected != menuItems.begin()) {
            --selected;
            refresh();
        }
    break;
    case KEY_RIGHT:
    case 'l':
        if(selected != --menuItems.end()) {
            ++selected;
            refresh();
        }
    break;
    }

}

void Toolbar::onResize()  {
    resize(0, line, maxx(), 1);    
    refresh();
}