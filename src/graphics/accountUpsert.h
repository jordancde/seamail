#ifndef ACCOUNT_UPSERT_H
#define ACCOUNT_UPSERT_H

#include <vector>

#include "graphics/nwindow.h"
#include "account/account.h"

class AccountUpsert : public NWindow {

    typedef std::function<void(std::string username, std::string password, bool newAccount)> auth_handler_t;
    auth_handler_t authHandler;

    int tabIndex = 0;
    std::string username;
    std::string password;
    bool newAccount = false;

    void selectNextItem();
    void selectPreviousItem();
    
public:
    AccountUpsert(auth_handler_t authHandler) : NWindow(10), authHandler(authHandler) {}

    void onDraw(bool isActive) const;
    bool onInput(int key);
    void onResize();

};

#endif