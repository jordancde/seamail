#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <vector>
#include <iostream>
#include <memory>
#include <string>

#include "./session.h"
#include "../events/event.h"
#include "../utility/observer.h"
#include "../utility/subject.h"
#include "../utility/serializable.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../providers/emailProvider.h"

class Account: public Subject, public Serializable {
    Session session;
    EmailProvider& provider;
    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;

    static std::string sentPath;
    static std::string inboxPath;
    static std::string deletedPath;

public:
    std::vector<std::string> getAllFolderPaths();
    Folder getFolderByPath(std::string path, std::string sort);
    Email getEmailById(std::string id);
    void sendEmail(Email email);
    void addThreadToFolder(std::string threadId, std::string folderPath);
    void removeThreadFromFolder(std::string threadId, std::string folderPath);
    std::string addFolder(std::string folderPath);
    void removeFolder(std::string folderPath);

    bool login(std::string emailAddress, std::string password);
    void logout();

    Account(EmailProvider& provider);
    virtual ~Account();
};

#endif