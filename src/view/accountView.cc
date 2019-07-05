
#include "./accountView.h"

#include <map>
#include <string>
#include <algorithm>

#include "events/accountEvent.h"

using namespace std;


pair<int, string> AccountView::folderPathToDisplayName(const std::string &path){
    auto lastSlash = path.rfind("/");
    string folderName = path;
    if(lastSlash != string::npos)
        folderName = path.substr(lastSlash+1);
    size_t level = count(path.begin(), path.end(), '/');
    return make_pair(level, folderName);
}
void AccountView::onDraw(bool isActive) const {
    auto folderPaths = account->getAllFolderPaths();

    sort(folderPaths.begin(), folderPaths.end());

    wmove(win, 1, 0);
    for(const string& s : folderPaths){
        pair<int, string> dispName = folderPathToDisplayName(s);
        waddch(win, ' ');
        for(size_t i = 0; i < dispName.first; ++i)
            wprintw(win, "\t");
        wprintw(win, "%s\n", dispName.second.c_str());
    }
    wmove(win, 0, 2);
    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wattroff(win, A_REVERSE);
    wprintw(win, "Folders");
}

void AccountView::notify(std::shared_ptr<Event> event) {
    auto ptr = std::dynamic_pointer_cast<AccountEvent>(event);
    if(!ptr) throw std::logic_error("Pointer to wrong event type!");

    switch(ptr->getType()){
        case AccountEventType::ACCOUNT_FOLDERS_CHANGED:
            this->refresh();
            break;
    }
}

void AccountView::onResize() {
    resize(0, 1, maxx()  / 4, maxy()-1);
}

