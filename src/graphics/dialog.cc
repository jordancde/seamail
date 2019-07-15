
#include "./dialog.h"

void Dialog::onDraw(bool isActive) const {
    wmove(win, 1, 2);
    mvwprintw(win, cy()+1,cx(), "%s", message.c_str());
    wattron(win, A_REVERSE);
    mvwprintw(win, h()-3, w()/2-3, "[ OK ]");
    wattroff(win, A_REVERSE);

    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wmove(win, 0, 2);
    wprintw(win, "%s", title.c_str());
    wattroff(win, A_REVERSE);

}

bool Dialog::onInput(int key) {
    switch(key){
        case '\n':
            dialogClosedHandler();
            break;
    }
    return true;
}

void Dialog::onResize() {
    int w = dialogW;
    int h = dialogH;
    int x = maxx() / 2 - w / 2;
    int y = maxy() / 2 - h / 2;

    resize(w, h);
    reframe(x,y,0,0,w,h);
}