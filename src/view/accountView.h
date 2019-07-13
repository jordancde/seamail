
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

    void updateSelectedFolder(size_t idx);

    std::vector<std::string> sortedFolderPaths;
    void updateCachedFolderPaths() {
        sortedFolderPaths = account.getAllFolderPaths();
        sort(sortedFolderPaths.begin(), sortedFolderPaths.end());
    }
    const std::vector<std::string>& getCachedFolderPaths() const {
        return sortedFolderPaths;
    }

    void moveFolderToEnd(std::vector<std::string>&, std::string, bool) const;

    std::function<void(std::string folderPath)> folderChangeHandler;

public:
    AccountView(Account& account,
                std::function<void(std::string folderPath)> folderChangeHandler = [](std::string){})
        : View(account, 1), folderChangeHandler(folderChangeHandler) {
        updateCachedFolderPaths();
        updateSelectedFolder(0);
    }
    
    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    bool onInput(int key) override;
};

#endif