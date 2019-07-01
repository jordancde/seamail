#ifndef _ACCOUNT_EVENT_H_
#define _ACCOUNT_EVENT_H_

#include <string>

#include "./event.h"
#include "../account/account.h"

enum AccountEventType {
    FOLDER_CONTENTS_CHANGED,
    ACCOUNT_FOLDERS_CHANGED,
    THREAD_CHANGED
};

class AccountEvent: public Event {
    AccountEventType type;
    std::string affectedId;
    Account& affectedAccount;

public:
    AccountEventType getType();
    std::string getAffectedId();
    Account& getAffectedAccount();

    AccountEvent(AccountEventType type, Account& acc, std::string id);
    ~AccountEvent();
};

#endif