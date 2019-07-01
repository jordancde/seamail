

#include "compositor.h"

#include <algorithm>
#include <csignal>
#include <stdexcept>


std::unique_ptr<Compositor> Compositor::_instance;

void Compositor::resizeHandler(int sig){
    // refresh stdscr so that getmaxyx works
    endwin();
    ::refresh();
    clear();
    Compositor::instance().resize();
    Compositor::instance().refresh();
}

Compositor::Compositor(){
    std::signal(SIGWINCH, resizeHandler);

	initscr();			
	raw();				
	keypad(stdscr, TRUE);		
	noecho();			
    curs_set(0);
}

Compositor::~Compositor() {
	endwin();			
}

Compositor& Compositor::instance(){
    if(!_instance)
        _instance = std::unique_ptr<Compositor>(new Compositor());
    return *_instance;
}

void Compositor::addWindow(std::shared_ptr<NWindow> window) {
    windows.push_back(window);
}

void Compositor::removeWindow(std::shared_ptr<NWindow> window) {
    auto it = std::find(windows.begin(), windows.end(), window);
    if(it != windows.end()){
        if(*it == activeWindow)
            throw std::logic_error("Cannot remove active window!");
        windows.erase(it); 
    } else {
        throw std::out_of_range("Window being removed does not exist!");
    }
}

void Compositor::setActiveWindow(std::shared_ptr<NWindow> window) {
    auto it = std::find(windows.begin(), windows.end(), window);
    if(it != windows.end()) {
        activeWindow = *it;
    } else {
        throw std::out_of_range("Window being set as active does not exist!");
    }
}

void Compositor::resize() {
    for(auto win : windows)
        win->onResize();
}
void Compositor::refresh() {
    for(auto win : windows)
        win->refresh();
    update();
}

void Compositor::update() {
    doupdate();
}

void Compositor::run() {
    bool quit = false;
    while(!quit){
        if(!activeWindow)
            throw std::logic_error("No active window is defined!");

        int ch = activeWindow->getKey();

        switch(ch){
        case 'q':
            quit = true;
            break;
        case 'p':
            break;
        case KEY_TAB:
        default:
            activeWindow->onInput(ch);
            break;
        }

        update();
    }
}