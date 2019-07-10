#ifndef ACCOUNT_SELECT_H
#define ACCOUNT_SELECT_H

#include <vector>

#include "graphics/nwindow.h"
#include "account/account.h"

class AccountSelect : public NWindow {

    std::vector<Account>& accounts;

    std::function<void(Account&)> accountSelectedHandler;

    std::vector<Account>::iterator selectedAccount;

public:
    AccountSelect(std::vector<Account>& accounts, 
        std::function<void(Account&)> accountSelectedHandler) 
        : NWindow(10), accounts(accounts), accountSelectedHandler(accountSelectedHandler) {
        selectedAccount = accounts.begin();
    } 
    
    void onDraw(bool isActive) const; 
    bool onInput(int key); 
    void onResize(); 


};

#endif