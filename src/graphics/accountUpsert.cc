#include "./accountUpsert.h"

using namespace std;

void AccountUpsert::onDraw(bool isActive) const {
    werase(win);
    string pwdummy = string(password.length(), '*');
    pair<string,string> fieldNames[] = {
        {"Username", username == "" ? " " : username},
        {"Password", password == "" ? " " : pwdummy.c_str()},
        {"Register", newAccount ? "[x]" : "[ ]"},
    };
    wmove(win, 1,2);
    int index = 0;
    for(auto fields : fieldNames) {
        mvwprintw(win, cy()+1,2,"%-5s: ", fields.first.c_str());
        if(index == tabIndex)
            wattron(win, A_REVERSE);
        wprintw(win, "%s", fields.second.c_str());
        wattroff(win, A_REVERSE);
        ++index;
    }

    if(tabIndex == 3)
        wattron(win, A_REVERSE);
    mvwprintw(win, cy()+2,2,"%s", "Cancel");
    wattroff(win, A_REVERSE);

    if(tabIndex == 4)
        wattron(win, A_REVERSE);
    mvwprintw(win, cy()+1,2,"%s", "Login");
    wattroff(win, A_REVERSE);
    
    wmove(win, 0,1);

    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wmove(win, 0, 2);
    wprintw(win, "Login/Register account");
    wattroff(win, A_REVERSE);

}

bool AccountUpsert::onInput(int key) {
    bool handled = false;
    switch(key){
        case '\t':
        tabIndex = (tabIndex + 1) % 5;
        refresh();
        handled = true;
        break;
        default:
        switch(tabIndex){
            case 3:
            authHandler("","",false);
            handled = true;
            break;
            case 4:
            authHandler(username,password,newAccount);
            handled = true;
            break;
            case 0:{
                switch(key){
                    case KEY_BACKSPACE:
                    if(username.length() > 0)
                        username = username.substr(0, username.length() - 1);
                    break;
                    default:
                    char c = (char) key;
                    if(isprint(c))
                        username += c;
                    handled = true;
                    break;
                }
                refresh();
            }
            break;
            case 1:
                switch(key){
                    case KEY_BACKSPACE:
                    if(password.length() > 0)
                        password = password.substr(0, password.length() - 1);
                    break;
                    default:
                    char c = (char) key;
                    if(isprint(c))
                        password += c;
                    handled = true;
                    break;
                }
                refresh();
            break;
            case 2:
                switch(key){
                    case '\n':
                    newAccount = !newAccount;
                    handled = true;
                    break;
                }
                refresh();
            break;
        }
        break;
    }
    return handled;
}

void AccountUpsert::onResize() {
    int w = 100;
    int h = 10;
    int x = maxx() / 2 - w / 2;
    int y = maxy() / 2 - h / 2;

    resize(w, h);
    reframe(x,y,0,0,w,h);
}