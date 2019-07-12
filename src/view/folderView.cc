#include "./folderView.h"

#include "events/accountEvent.h"

using namespace std;

void FolderView::onResize(){
    int x = maxx() / 6;
    int y = 2;
    int w = maxx() / 3;
    int h = maxy() - y;
    resize(w,h);
    reframe(x,y,0,0,w,h);
}

void FolderView::onDraw(bool isActive) const {
    werase(win);
    wmove(win, 1, 1);
    
    auto folder = getFolder(); 
    size_t cindex = 0;
    for(auto threadId = folder.threadIds.begin(); threadId != folder.threadIds.end(); ++threadId){
        auto thread = account.getThreadById(*threadId);
        time_t latestDate = 0; 
        bool read = true;
        if(cindex == selectedThreadIndex){
            wattron(win, A_REVERSE);
            mvwhline(win, cy(), 0, ' ', w());
            wattroff(win, A_REVERSE);
        }
        for(auto emailId = thread.emailIds.begin(); emailId != thread.emailIds.end(); ++emailId){
            auto email = account.getEmailById(*emailId);
            latestDate = max(latestDate, email.dateTime);
            read &= email.read;
        }
        struct tm* timeinfo = localtime(&latestDate);
        const char* desctime = asctime(timeinfo);
        auto lastFrom = account.getEmailById(thread.emailIds.back()).from;

        mvwprintw(win, cy()+1, 1, "%s", lastFrom.c_str());
        if(!read)
            wattron(win, A_REVERSE);
        mvwprintw(win, cy() + 1, 2, "%s", thread.title.c_str());
        wattroff(win, A_REVERSE);
        mvwprintw(win, cy(), w() - strlen(desctime), "%s", desctime);
        mvwhline(win, cy(), 0, ACS_HLINE, w());
        if(thread.emailIds.size() > 1){
            string ticker = (thread.emailIds.size()-1) + " more...";
            mvwprintw(win, cy()+1, w()/2-ticker.length()/2, "%s", ticker.c_str());
            mvwhline(win, cy()+1, 0, ACS_HLINE, w());
        }

        size_t displayCount = 0;
        for(auto emailId = thread.emailIds.begin(); emailId != thread.emailIds.end(); ++emailId){
            auto email = account.getEmailById(*emailId);
            if(++displayCount > 2)
                break;
        }
        wmove(win, cy()+1, cx());
        ++cindex;
    } 


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
            updateThreadIndex(0);
        }
        default:
            break;
    }
}

bool FolderView::onInput(int key) {
    switch(key){
        case KEY_DOWN:
        case 'j':
            selectNextThread();
            return true;
        case KEY_UP:
        case 'k':
            selectPreviousThread();
            return true;
    }
    return false;
}