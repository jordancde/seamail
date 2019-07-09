#ifndef EMAILPROVIDER_H
#define EMAILPROVIDER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "../account/session.h"
#include "../models/email.h"
#include "../models/folder.h"
#include "../models/thread.h"
#include "../utility/serializable.h"

class EmailProvider : public Serializable {
protected:
    std::unordered_map<std::string, std::vector<std::string>> folderPaths; // "email" to folder paths
    std::unordered_map<std::string, Folder> folders; // "/email/path" to folders
    std::unordered_map<std::string, Thread> threads; // "/email/threadId" to threads
    std::unordered_map<std::string, Email> emails; // "/email/emailId" to emails

    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    static std::string sentPath;
    static std::string inboxPath;
    static std::string deletedPath;

    EmailProvider() = default;
    virtual ~EmailProvider() = default;

    virtual Session getSession(std::string emailAddress, std::string password) = 0;

    virtual Folder getFolderByPath(Session& ctx, std::string folderPath) = 0;
    virtual std::vector<std::string> getAllFolderPaths(Session& ctx) = 0;
    virtual void addFolder(Session& ctx, std::string folderPath) = 0;
    virtual void removeFolder(Session& ctx, std::string folderPath) = 0;

    virtual Thread getThreadById(Session& ctx, std::string threadId) = 0;
    virtual void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath) = 0;
    virtual void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath) = 0;
    virtual void setEmailStatus(Session& ctx, std::string emailId, bool read) = 0;
    virtual Email getEmailById(Session& ctx, std::string emailId) = 0;
    virtual void sendEmail(Session& ctx, Email email) = 0;

    bool operator==(const EmailProvider&) const;
};

#endif
