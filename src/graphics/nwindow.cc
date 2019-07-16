
#include "nwindow.h"

#include <utility>
#include <stdexcept>

#include "graphics/compositor.h"

std::vector<WindowColor> NWindow::colors;

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

void NWindow::reframe(size_t wx, size_t wy, size_t x, size_t y, size_t w, size_t h) {
    _wx = wx;
    _wy = wy;
    _x = x;
    _y = y;
    _w = w;
    _h = h;
}

void NWindow::resize(size_t w, size_t h) {
    if(win)
        delwin(win);
    win = newpad(h, w);
    keypad(win, TRUE);

	wbkgd(win, COLOR_PAIR(cp));
    wattron(win, COLOR_PAIR(cp));
}

void NWindow::refresh() const {
    //wclear(win);

    onDraw(Compositor::instance().getActiveWindow() == this);
    pnoutrefresh(win, y(), x(), wy(), wx(), wy() + h(), wx() + w());
}
