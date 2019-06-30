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
    Session* session;
    EmailProvider& provider;
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

public:
    std::vector<std::string> getAllFolderPaths();
    Folder getFolderByPath(std::string path, std::string sort);
    Email getEmailById(std::string id);
    void sendEmail(Email email);
    void addEmailToFolder(std::string emailId, std::string folderPath);
    void removeEmailFromFolder(std::string emailId, std::string folderId);
    std::string addFolder(std::string folderName, std::string parentPath);
    void removeFolder(std::string folderPath);

    bool login(std::string username, std::string password);
    void logout();

    Account(EmailProvider& provider);
    ~Account();
};

#endif