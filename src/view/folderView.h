#ifndef FOLDER_VIEW_H
#define FOLDER_VIEW_H

#include "view/view.h"
#include "account/account.h"

class FolderView : public View {

    std::string watchingFolder;
    std::function<void(std::string)> threadChangeHandler;

    Folder getFolder() const {
        return account.getFolderByPath(watchingFolder);
    }

    size_t selectedThreadIndex = SIZE_MAX;


    void updateThreadIndex(size_t index){
        size_t threads = getFolder().threadIds.size();
        if(threads < 1)
            selectedThreadIndex = SIZE_MAX;
        else if(index >= 0 && index < threads){
            selectedThreadIndex = index;
            threadChangeHandler(getFolder().threadIds.at(selectedThreadIndex));
        }
        if(win) refresh();
    }

    void selectNextThread(){
        updateThreadIndex(selectedThreadIndex + 1);
    }

    void selectPreviousThread(){
        updateThreadIndex(selectedThreadIndex - 1);
    }


public:
    FolderView(Account& account,
                std::string watchingFolder,
                std::function<void(std::string)> threadChangeHandler = [](std::string){})
                : View(account), watchingFolder(watchingFolder), 
                    threadChangeHandler(threadChangeHandler) {
        updateThreadIndex(0);
    }

    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    bool onInput(int key) override;
};


#endif