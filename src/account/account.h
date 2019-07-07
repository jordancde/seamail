#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "../events/event.h"
#include "../models/email.h"
#include "../models/thread.h"
#include "../models/folder.h"
#include "../providers/emailProvider.h"
#include "../utility/observer.h"
#include "../utility/serializable.h"
#include "../utility/subject.h"
#include "./session.h"

class Account : public Subject, public Serializable {
    Session session;
    std::shared_ptr<EmailProvider> provider;
    std::string emailAddress;

    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    bool loggedIn;

    Account(std::shared_ptr<EmailProvider> provider, std::string emailAddress);
    Account() = default;

    bool operator==(const Account&) const;

    void login(std::string emailAddress, std::string password);
    void logout();

    Folder getFolderByPath(std::string path, std::string sort);
    std::vector<std::string> getAllFolderPaths();
    std::string addFolder(std::string folderPath);
    void removeFolder(std::string folderPath);

    Thread getThreadById(std::string threadId);
    void addThreadToFolder(std::string threadId, std::string folderPath);
    void removeThreadFromFolder(std::string threadId, std::string folderPath);

    Email getEmailById(std::string id);
    void sendEmail(Email email);

};

#endif