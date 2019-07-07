
#ifndef ACCOUNT_VIEW_H_
#define ACCOUNT_VIEW_H_

#include <map>
#include <string>
#include <optional>

#include "view/view.h"
#include "account/account.h"

class AccountView : public View {

    static std::pair<size_t, std::string> folderPathToDisplayName(const std::string &path);
    size_t selectedFolderIndex = SIZE_MAX;

    void updateSelectedFolder();

    std::function<void(std::string folderPath)> folderChangeHandler;

public:
    AccountView(std::shared_ptr<Account> account,
                std::function<void(std::string folderPath)> folderChangeHandler = [](std::string){})
        : View(account), folderChangeHandler(folderChangeHandler) {
        onResize();
        updateSelectedFolder();
    }
    
    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    void onInput(int key) override;
};

#endif