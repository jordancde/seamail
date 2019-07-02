#ifndef NWINDOW_H_
#define NWINDOW_H_

#include <cstdlib>
#include <ncurses.h>

class NWindow {
    void swap(NWindow&) noexcept;

protected:
    WINDOW *win;

    void resize(size_t x, size_t y, size_t w, size_t h);
public:
    NWindow(size_t x, size_t y, size_t w, size_t h);
    NWindow();
    virtual ~NWindow() = 0;

    NWindow(NWindow&&);
    NWindow(const NWindow&) = delete;
    NWindow& operator=(const NWindow&) = delete;
    NWindow& operator=(NWindow&&);

    size_t x() const { return getbegx(win); }
    size_t y() const { return getbegy(win); }
    size_t w() const { return getmaxx(win) - x(); }
    size_t h() const { return getmaxy(win) - y(); }
    size_t cx() const { return getcurx(win); }
    size_t cy() const { return getcury(win); }
    size_t maxx() const { return getmaxx(stdscr); }
    size_t maxy() const { return getmaxy(stdscr); }

    void move(size_t x, size_t y) { wmove(win, y, x); }

    // Redraw the ncurses window to screen
    void refresh() const;
    int getKey() const;

    virtual void onDraw() const {}
    virtual void onInput(int key) {}
    virtual void onResize() {}
};

#endif
