

#include "compositor.h"

#include <algorithm>
#include <csignal>
#include <stdexcept>
#include <utility>


std::unique_ptr<Compositor> Compositor::_instance;

void Compositor::resizeHandler(int sig){
    // refresh stdscr so that getmaxyx works
    endwin();
    ::refresh();
    clear();
    Compositor::instance().resize();
    Compositor::instance().refresh();
}

bool compare(const std::shared_ptr<NWindow>& a, const std::shared_ptr<NWindow>& b){
    if(a->getZindex() == b->getZindex())
        return a < b;
    return a->getZindex() < b->getZindex();
}
    
Compositor::Compositor() : windows(compare) {
    activeWindow = windows.end();
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
    if(activeWindow != windows.end()){
        auto cachedActiveWindow = *activeWindow;
        windows.insert(window);
        activeWindow = windows.find(cachedActiveWindow);
    } else {
        windows.insert(window);
    }
    window->onResize();
}

void Compositor::removeWindow(std::shared_ptr<NWindow> window) {
    auto it = windows.find(window);
    if(it != windows.end()){
        if(it == activeWindow){
            windows.erase(it);
            if(windows.size() < 1)
                throw std::logic_error{"No windows remaining!"};
            else {
                bool set = false;
                while(!(set = setActiveWindow(activeWindowStack.top()))){
                    activeWindowStack.pop();
                }
                if(set)
                    activeWindowStack.pop();
                else
                    setActiveWindow(*windows.begin());
            }
        } else {
            if(activeWindow != windows.end()){
                auto cachedActiveWindow = *activeWindow;
                windows.erase(it); 
                activeWindow = windows.find(cachedActiveWindow);
            } else {
                windows.erase(it);
            }
        }
    } 
    refresh();
}

bool Compositor::setActiveWindow(std::shared_ptr<NWindow> window) {
    auto it = windows.find(window);
    if(it != windows.end()) {
        activeWindow = it;
        activeWindowStack.push(window);
        return true;
    }
    return false;
}

NWindow* Compositor::getActiveWindow() {
    return (*activeWindow).get();
}


void Compositor::resize() {
    for(auto win : windows)
        win->onResize();
}
void Compositor::refresh() {
    // hack, causes flickering
    ::refresh();
    ::clear();
    for(auto win : windows)
        win->refresh();
    update();
}

void Compositor::update() {
    doupdate();
}

void Compositor::run() {
    while(!shouldquit){
        if(activeWindow == windows.end())
            throw std::logic_error("No active window is defined!");

        int ch = (*activeWindow)->get();

        bool handled = (*activeWindow)->onInput(ch);
        if(!handled){
            switch(ch){
            case 'q':
                shouldquit = true;
                break;
            case 'p':
                break;
            case 27:
                (*activeWindow)->nodelay(TRUE);
                ch = (*activeWindow)->get();
                (*activeWindow)->nodelay(FALSE);
                if(ch == ERR) {     // escape
                    activeWindow = windows.begin();
                    refresh();
                } else { // ALT + ch

                }
                break;
            case ']':
                if(activeWindow != (--windows.end()))
                    ++activeWindow;
                refresh();
                break;
            case '[':
                if(activeWindow != windows.begin())
                    --activeWindow;
                refresh();
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                int idx = ch - '0';
                if((size_t) idx < windows.size()){
                    activeWindow = windows.begin();
                    std::advance(activeWindow, idx);
                }
                refresh();
                break;
            }
            default:
                break;
            }
        }
        update();
    }
}

void Compositor::quit(){shouldquit=true;}