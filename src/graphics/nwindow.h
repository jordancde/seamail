#ifndef NWINDOW_H_
#define NWINDOW_H_

#include <cstdlib>
#include <ncurses.h>

class NWindow {
    void swap(NWindow&) noexcept;

protected:
    
    WINDOW *win;
public:
    NWindow(size_t x, size_t y, size_t w, size_t h);
    NWindow();
    virtual ~NWindow() = 0;

    NWindow(NWindow&&);
    NWindow(const NWindow&) = delete;
    NWindow& operator=(const NWindow&) = delete;
    NWindow& operator=(NWindow&&);

    size_t x() { return getbegx(win); }
    size_t y() { return getbegy(win); }
    size_t w() { return getmaxx(win) - x(); }
    size_t h() { return getmaxy(win) - y(); }
    size_t cx() { return getcurx(win); }
    size_t cy() { return getcury(win); }

    void move(size_t x, size_t y) { wmove(win, y, x); }

    // Redraw the ncurses window to screen
    void refresh() const;
    int getKey() const;

    virtual void draw(size_t nw, size_t nh) const {}
    virtual void onInput(int key) {}
};

#endif
