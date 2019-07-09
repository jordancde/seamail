#ifndef LOCALEMAILPROVIDER_H
#define LOCALEMAILPROVIDER_H

#include <vector>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "../account/session.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../models/thread.h"
#include "../account/account.h"
#include "emailProvider.h"

class LocalEmailProvider: public EmailProvider {
    std::unordered_map<std::string, std::size_t> accounts; // email to pwd hash

    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    LocalEmailProvider() = default;

    void addAccount(std::string emailAddress, std::string password);

    Session getSession(std::string emailAddress, std::string password) override;

    Folder getFolderByPath(Session& ctx, std::string folderPath) override;
    std::vector<std::string> getAllFolderPaths(Session& ctx) override;
    void addFolder(Session& ctx, std::string folderPath) override;
    void removeFolder(Session& ctx, std::string folderPath) override;

    Thread getThreadById(Session& ctx, std::string threadId) override;
    void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath) override;
    void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath) override;
    
    void setEmailStatus(Session& ctx, std::string emailId, bool read) override;
    
    Email getEmailById(Session& ctx, std::string emailId) override;
    void sendEmail(Session& ctx, Email email) override;
};

#endif
