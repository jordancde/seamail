
#include "nwindow.h"

#include <utility>
#include <stdexcept>

NWindow::NWindow(size_t x, size_t y, size_t w, size_t h) {
    resize(x, y, w, h);
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

void NWindow::resize(size_t x, size_t y, size_t w, size_t h) {
    if(win == stdscr)
        return;
    if(win)
        delwin(win);
    win = newwin(h, w, y, x);
}

void NWindow::refresh() const {
    //wclear(win);
    onDraw();
    wnoutrefresh(win);
}

int NWindow::getKey() const {
    return wgetch(win);
}