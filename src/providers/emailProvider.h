#ifndef EMAILPROVIDER_H
#define EMAILPROVIDER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "../utility/serializable.h"
#include "../account/session.h"
#include "../models/folder.h"
#include "../models/email.h"
#include "../models/thread.h"

class EmailProvider: public Serializable {

protected:
    // email to folder paths
    std::unordered_map<std::string, std::vector<std::string>> accountFolderPaths;
    // "/email/path" to folders
    std::unordered_map<std::string, Folder> folders;
    // "/email/threadId" to threads
    std::unordered_map<std::string, Thread> threads;
    // "/email/emailId" to emails
    std::unordered_map<std::string, Email> emails;

public:
    EmailProvider();
    virtual ~EmailProvider()=0;
    virtual std::vector<std::string> getAllFolderPaths(Session& ctx);
    virtual Folder getFolderByPath(Session& ctx, std::string folderPath, std::string sort);
    virtual Email getEmailById(Session& ctx, std::string emailId);
    virtual void sendEmail(Session& ctx, Email email);
    virtual Thread getThreadById(Session& ctx, std::string threadId);
    virtual void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath);
    virtual void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath);
    virtual std::string addFolder(Session& ctx, std::string folderPath);
    virtual void removeFolder(Session& ctx, std::string folderPath);
    virtual Session getSession(std::string emailAddress, std::string password);
};

#endif

    