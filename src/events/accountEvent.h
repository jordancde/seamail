#ifndef _ACCOUNT_EVENT_H_
#define _ACCOUNT_EVENT_H_

#include "event.h"
#include "../account/account.h"

enum AccountEventType {
    FOLDER_CONTENTS_CHANGED,
    ACCOUNT_FOLDERS_CHANGED,
    THREAD_CHANGED,
    EMAIL_CHANGED
}

class AccountEvent: public Event {

public:
    AccountEventType getType();
    string getAffectedEmailId();
    Account& getAffectedAccount();
    string getAffectedFolderName();

    AccountEvent(AccountEventType,string);
    AccountEvent(AccountEventType,Account&);
    ~AccountEvent();
};

#endif