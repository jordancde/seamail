
#include "nwindow.h"

#include <utility>
#include <stdexcept>

#include "graphics/compositor.h"

NWindow::NWindow() {
    // hack
    resize(0, 0, 0, 0);
}

NWindow::~NWindow() {
    if(win) {
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
    if(win)
        delwin(win);
    win = newwin(h, w, y, x);
    keypad(win, TRUE);
}

void NWindow::refresh() const {
    //wclear(win);

    onDraw(Compositor::instance().getActiveWindow() == this);
    wnoutrefresh(win);
}
