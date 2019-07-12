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

    wmove(win, cy()+2,0);
    if(tabIndex == 3)
        wattron(win, A_REVERSE);
    mvwprintw(win, cy(),w()/2-12,"%s", " [ Cancel ] ");
    wattroff(win, A_REVERSE);

    if(tabIndex == 4)
        wattron(win, A_REVERSE);
    wprintw(win, "%s", " [ Login ] ");
    wattroff(win, A_REVERSE);
    
    wmove(win, 0,1);

    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wmove(win, 0, 2);
    wprintw(win, "Login/Register account");
    wattroff(win, A_REVERSE);

}

void AccountUpsert::selectNextItem(){
    tabIndex = (tabIndex + 1) % 5;
    refresh();
}

void AccountUpsert::selectPreviousItem(){
    tabIndex = (tabIndex + 5 - 1) % 5;
    refresh();
}

bool AccountUpsert::onInput(int key) {
    switch(key){
        case '\t':
        case KEY_DOWN:
            selectNextItem();
            break;
        case KEY_UP:
            selectPreviousItem();
            break;
        default:
            switch(tabIndex){
                case 3:
                    if(key == '\n'){
                        authHandler("","",false);
                    }
                    break;
                case 4:
                    if(key == '\n'){
                        authHandler(username,password,newAccount);
                    }
                    break;
                case 0:
                    switch(key){
                        case '\n':
                            selectNextItem();
                            break;
                        case KEY_BACKSPACE:
                            if(username.length() > 0)
                                username = username.substr(0, username.length() - 1);
                            break;
                        default:
                            char c = (char) key;
                            if(isprint(c))
                                username += c;
                            break;
                    }
                    refresh();
                    break;
                case 1:
                    switch(key){
                        case '\n':
                            selectNextItem();
                            break;
                        case KEY_BACKSPACE:
                            if(password.length() > 0)
                                password = password.substr(0, password.length() - 1);
                            break;
                        default:
                            char c = (char) key;
                            if(isprint(c))
                                password += c;
                            break;
                    }
                    refresh();
                    break;
                case 2:
                    switch(key){
                        case '\n':
                        newAccount = !newAccount;
                        break;
                    }
                    refresh();
                break;
            }
            break;
    }
    return true;
}

void AccountUpsert::onResize() {
    int w = 100;
    int h = 9;
    int x = maxx() / 2 - w / 2;
    int y = maxy() / 2 - h / 2;

    resize(w, h);
    reframe(x,y,0,0,w,h);
}