
#ifndef ACCOUNT_VIEW_H_
#define ACCOUNT_VIEW_H_

#include <map>
#include <string>
#include <optional>

#include "view/view.h"
#include "account/account.h"

class AccountView : public View {

    static std::pair<size_t, std::string> folderPathToDisplayName(const std::string &path);
public:
    AccountView(std::shared_ptr<Account> account)
        : View(account) {
            onResize();
    }
    
    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
};

#endif