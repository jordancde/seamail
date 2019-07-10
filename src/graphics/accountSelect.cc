#include "./accountSelect.h"

void AccountSelect::onDraw(bool isActive) const {

    wmove(win, 0,1);
    for(auto acciter = accounts.begin(); acciter != accounts.end(); ++acciter){
        auto acc = *acciter;
        auto email = acc.getEmailAddress();
        if(acciter == selectedAccount)
            wattron(win, A_REVERSE);
        mvwprintw(win, cy()+1,1,"%s",email.c_str());
        wattroff(win, A_REVERSE);
    }

    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wmove(win, 0, 2);
    wprintw(win, "Select account");
    wattroff(win, A_REVERSE);

}

bool AccountSelect::onInput(int key) {
    bool handled = false;
    switch(key){
        case 'k':
        if(selectedAccount != --accounts.end()){
            ++selectedAccount;
            refresh();
        }
        handled = true;
        break;
        case 'j':
        if(selectedAccount != accounts.begin()){
            --selectedAccount;
            refresh();
        }
        handled = true;
        break;
        case '\n':
        accountSelectedHandler(*selectedAccount);
        handled = true;
        break;
        default:
        break;
    }
    return handled;
}

void AccountSelect::onResize() {
    int w = 100;
    int h = 10;
    int x = maxx() / 2 - w / 2;
    int y = maxy() / 2 - h / 2;

    resize(w, h);
    reframe(x,y,0,0,w,h);
}