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

    void selectNextThread(){
        size_t threads = getFolder().threadIds.size();
        if(threads < 1)
            selectedThreadIndex = SIZE_MAX;
        else if(selectedThreadIndex < threads - 1)
            ++selectedThreadIndex;
    }

    void selectPreviousThread(){
        size_t threads = getFolder().threadIds.size();
        if(threads < 1)
            selectedThreadIndex == SIZE_MAX;
        else if(selectedThreadIndex > 0)
            --selectedThreadIndex;
    }


public:
    FolderView(Account& account,
                std::string watchingFolder,
                std::function<void(std::string)> threadChangeHandler = [](std::string){})
                : View(account), watchingFolder(watchingFolder), 
                    threadChangeHandler(threadChangeHandler) {}

    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    bool onInput(int key) override;
};


#endif