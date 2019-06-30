#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <vector>
#include <iostream>
#include <string>

#include "session.h"
#include "../utility/subject.h"
#include "../utility/serializable.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../providers/emailProvider.h"


class Account: public Subject, public Serializable {
    Session session;
    EmailProvider& provider;
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;
public:
    std::vector<std::string> getAllFolderPaths();
    Folder getFolderByPath(std::string, std::string);
    Email getEmailById(std::string);
    void sendEmail(Email);
    void addEmailToFolder(std::string, std::string);
    void removeEmailFromFolder(std::string, std::string);
    std::string addFolder(std::string, std::string);
    void removeFolder(std::string);
    bool login(std::string, std::string);
    void logout();

    Account(EmailProvider& provider);
    ~Account();
};

#endif