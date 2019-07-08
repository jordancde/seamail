#include "./folderView.h"

#include "events/accountEvent.h"

using namespace std;

void FolderView::onResize(){
    int x = maxx() / 6;
    int y = 1;
    int w = maxx() / 3;
    int h = maxy() - 1;
    resize(w,h);
    reframe(x,y,0,0,w,h);
}

void FolderView::onDraw(bool isActive) const {
    wmove(win, 1, 0);
    
    wmove(win, 0, 2);
    if(isActive)
        wattron(win, A_REVERSE);
    box(win, 0, 0);
    wprintw(win, "Folder - %s", watchingFolder.c_str());
    wattroff(win, A_REVERSE);
}

void FolderView::notify(std::shared_ptr<Event> event) {
    auto ptr = std::dynamic_pointer_cast<AccountEvent>(event);
    if(!ptr)throw std::logic_error("Pointer to wrong event type!");

    switch(ptr->type){
        case AccountEventType::ACCOUNT_FOLDERS_CHANGED: {
            // folder might get deleted
            this->refresh();
            break;
        }
        case AccountEventType::FOLDER_CONTENTS_CHANGED: {
            this->refresh();
        }
        default:
            break;
    }
}

void FolderView::onInput(int key) {

}