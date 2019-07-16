#ifndef NWINDOW_H_
#define NWINDOW_H_

#include <cstdlib>
#include <ncurses.h>
#include <nlohmann/json.hpp>

struct WindowColor {
    short fg;
    short bg;

    WindowColor(short fg = COLOR_WHITE, short bg = COLOR_BLACK) : fg(fg), bg(bg) {}
    bool operator==(const WindowColor& o){ return fg == o.fg && bg == o.bg;}

    friend void to_json(nlohmann::json& json, const WindowColor& o){
        json["fg"] = o.fg;
        json["bg"] = o.bg;
    }

    friend void from_json(const nlohmann::json& json, WindowColor& o){
        o.fg = json["fg"];
        o.bg = json["bg"];
    }
};
class NWindow {
    void swap(NWindow&) noexcept;

protected:
    // Pointer to active ncurses PAD (not WINDOW!)
    WINDOW *win;

    // Modify the active frame of the pad (i.e. region of pad rendered to screen)
    void reframe(size_t wx, size_t wy, size_t x, size_t y, size_t w, size_t h);

    // Resizes the ncurses pad to the specified dimensions
    void resize(size_t w, size_t h);

    int zindex;
    size_t _wx, _wy;
    size_t _x, _y, _w, _h;

    static std::vector<WindowColor> colors;
    int cp;
protected:
    size_t wx() const { return _wx; }                 // pad x
    size_t wy() const { return _wy; }                 // pad y
    size_t x() const { return _x; }                 // frame x
    size_t y() const { return _y; }                 // frame y
    size_t w() const { return getmaxx(win) - x(); }                 // frame width
    size_t h() const { return getmaxy(win) - y(); }                 // frame height
    size_t cx() const { return getcurx(win); }      // cursor x
    size_t cy() const { return getcury(win); }      // cursor y
    size_t maxx() const { return getmaxx(stdscr); } // stdscr width
    size_t maxy() const { return getmaxy(stdscr); } // stdscr height

public:
    NWindow(int zindex = 0, WindowColor color = WindowColor{}) : zindex(zindex)  {
        auto it = std::find(colors.begin(), colors.end(), color);
        if(it != colors.end()){
            cp = std::distance(colors.begin(), it) +1;
        } else {
            colors.push_back(color);
            cp = colors.size();
            init_pair(cp, color.fg, color.bg);
        }
        reframe(0,0,0,0,0,0);
        resize(0, 0);
    }
    virtual ~NWindow() = 0;

    NWindow(NWindow&&);
    NWindow(const NWindow&) = delete;
    NWindow& operator=(const NWindow&) = delete;
    NWindow& operator=(NWindow&&);

    int getZindex(){
        return zindex;
    }

    void move(size_t x, size_t y) { wmove(win, y, x); }
    int get() const { return ::wgetch(win); }
    void nodelay(bool b) { ::nodelay(win, b); }

    // Redraw the ncurses window to screen
    void refresh() const;

    virtual void onDraw(bool isActive) const {}
    
    // return true if handled
    virtual bool onInput(int key) { return false; }
    virtual void onResize() {}
};

#endif
