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

    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;

public:
    EmailProvider();
    virtual ~EmailProvider()=0;
    virtual std::vector<std::string> getAllFolderPaths(Session& ctx)=0;
    virtual Folder getFolderByPath(Session& ctx, std::string folderPath, std::string sort)=0;
    virtual Email getEmailById(Session& ctx, std::string emailId)=0;
    virtual void sendEmail(Session& ctx, Email email)=0;
    virtual Thread getThreadById(Session& ctx, std::string threadId)=0;
    virtual void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath)=0;
    virtual void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath)=0;
    virtual std::string addFolder(Session& ctx, std::string folderPath)=0;
    virtual void removeFolder(Session& ctx, std::string folderPath)=0;
    virtual Session getSession(std::string emailAddress, std::string password)=0;
};

#endif

    