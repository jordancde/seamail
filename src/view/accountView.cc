
#include "./accountView.h"

#include <map>
#include <string>
#include <algorithm>

#include "events/accountEvent.h"

using namespace std;


pair<size_t, string> AccountView::folderPathToDisplayName(const std::string &path){
    auto lastSlash = path.rfind("/");
    string folderName = path;
    if(lastSlash != string::npos)
        folderName = path.substr(lastSlash+1);
    size_t level = count(path.begin(), path.end(), '/');
    return make_pair(level, folderName);
}
void AccountView::onDraw(bool isActive) const {
    auto folderPaths = getCachedFolderPaths();

    wmove(win, 1, 0);
    for(size_t fidx = 0; fidx < folderPaths.size(); ++fidx){
        const string& s = folderPaths.at(fidx);
        auto dispName = folderPathToDisplayName(s);
        waddch(win, ' ');
        for(size_t i = 0; i < dispName.first; ++i)
            wprintw(win, "\t");
        if(fidx == selectedFolderIndex)
            wattron(win, A_REVERSE);
        wprintw(win, "%s\n", dispName.second.c_str());
        wattroff(win, A_REVERSE);
    }
    wmove(win, 0, 2);
    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wprintw(win, "Folders");
    wattroff(win, A_REVERSE);
}

void AccountView::updateSelectedFolder(size_t idx) {
    auto folderPaths = getCachedFolderPaths();
    if(folderPaths.size() == 0)
        selectedFolderIndex = SIZE_MAX;
    else {
        selectedFolderIndex = idx;
        folderChangeHandler(folderPaths.at(selectedFolderIndex));
    }
    if(win) refresh();
}

void AccountView::notify(std::shared_ptr<Event> event) {
    auto ptr = std::dynamic_pointer_cast<AccountEvent>(event);
    if(!ptr) throw std::logic_error("Pointer to wrong event type!");

    switch(ptr->type){
        case AccountEventType::ACCOUNT_FOLDERS_CHANGED: {
            this->updateSelectedFolder(0);
            this->updateCachedFolderPaths();
            this->refresh();
            break;
        }
        default:
            break;
    }
}

void AccountView::onResize() {
    int x = 0;
    int y = 2;
    int w = maxx() / 6;
    int h = maxy() - y;
    resize(w,h);
    reframe(x,y,0,0,w,h);
}

bool AccountView::onInput(int key) {
    size_t max = getCachedFolderPaths().size();
    bool handled = false;
    switch(key){
    case 'k':
        if(selectedFolderIndex > 0){
            updateSelectedFolder(selectedFolderIndex - 1);
            refresh();
        }
        handled = true;
        break;
    case 'j':
        if(selectedFolderIndex < max - 1){
            updateSelectedFolder(selectedFolderIndex + 1);
            refresh();
        }
        handled = true;
        break;
    }
    // catch-all failsafe
    if(selectedFolderIndex < 0 || selectedFolderIndex >= max){
        updateSelectedFolder(0);
        refresh();
    }
    return handled;
}