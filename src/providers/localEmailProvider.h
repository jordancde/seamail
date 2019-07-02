#ifndef LOCALEMAILPROVIDER_H
#define LOCALEMAILPROVIDER_H

#include <vector>
#include <string>
#include <unordered_map>

#include "../account/session.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../models/thread.h"
#include "../account/account.h"
#include "emailProvider.h"

class LocalEmailProvider: public EmailProvider {
    // email to pwd hash
    std::unordered_map<std::string, std::size_t> accounts;

    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;

public:
    LocalEmailProvider();
    ~LocalEmailProvider() override;

    // copy & move not allowed since entity adt
    LocalEmailProvider(const LocalEmailProvider&) = delete;
    LocalEmailProvider(LocalEmailProvider&&) = delete;
    LocalEmailProvider& operator=(const LocalEmailProvider&) = delete;
    LocalEmailProvider& operator=(LocalEmailProvider&&) = delete;

    std::vector<std::string> getAllFolderPaths(Session& ctx) override;
    Folder getFolderByPath(Session& ctx, std::string folderPath, std::string sort) override;
    Email getEmailById(Session& ctx, std::string emailId) override;
    void sendEmail(Session& ctx, Email email) override;
    Thread getThreadById(Session& ctx, std::string threadId) override;
    void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath) override;
    void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath) override;
    std::string addFolder(Session& ctx, std::string folderPath) override;
    void removeFolder(Session& ctx, std::string folderPath) override;
    Session getSession(std::string emailAddress, std::string password) override;
    void addAccount(std::string emailAddress, std::string password);
};

#endif

    