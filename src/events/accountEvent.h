#ifndef _ACCOUNT_EVENT_H_
#define _ACCOUNT_EVENT_H_

#include <string>

#include "event.h"
#include "../account/account.h"

enum AccountEventType {
    FOLDER_CONTENTS_CHANGED,
    ACCOUNT_FOLDERS_CHANGED,
    THREAD_CHANGED,
    EMAIL_CHANGED
};

class AccountEvent: public Event {

public:
    AccountEventType getType();
    std::string getAffectedEmailId();
    Account& getAffectedAccount();
    std::string getAffectedFolderName();

    AccountEvent(AccountEventType, std::string);
    AccountEvent(AccountEventType, Account&);
    ~AccountEvent();
};

#endif