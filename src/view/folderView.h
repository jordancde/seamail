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