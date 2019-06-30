
#include "nwindow.h"

#include <utility>

NWindow::NWindow(size_t x, size_t y, size_t w, size_t h) {
    win = newwin(h, w, y, x);

}

NWindow::NWindow() {
    win = stdscr;
}

NWindow::~NWindow() {
    if(win && win != stdscr) {
        delwin(win);
    }
    win = nullptr;
}

NWindow::NWindow(NWindow&& o) : win(o.win) {
    o.win = nullptr;
}

NWindow& NWindow::operator=(NWindow&& o) {
    if(this != &o){
        this->swap(o);
    }
    return *this;
}

void NWindow::swap(NWindow& o) noexcept {
    std::swap(win, o.win);
}

void NWindow::refresh() const {
    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    draw(nw, nh);
    wrefresh(win);
}

int NWindow::getKey() const {
    return wgetch(win);
}