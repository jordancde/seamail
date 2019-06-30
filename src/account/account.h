#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "session.h"
#include "../utility/subject.h"
#include "../utility/serializable.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../providers/emailProvider.h"
#include <vector>
#include <iostream>

class Account: public Subject, public Serializable {
    Session session;
    EmailProvider& provider;
    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;
public:
    vector<string> getAllFolderPaths();
    Folder getFolderByPath(string, string);
    Email getEmailById(string);
    void sendEmail(Email);
    void addEmailToFolder(string, string);
    void removeEmailFromFolder(string,string)
    string addFolder(string,string);
    void removeFolder(string);
    bool login(string,string);
    void logout();

    Account(EmailProvider& provider);
    ~Account();
};

#endif