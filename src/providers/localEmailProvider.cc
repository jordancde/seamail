#include "localEmailProvider.h"

#include "../exceptions/notImplementedException.h"
#include "../exceptions/authenticationFailedException.h"

LocalEmailProvider::LocalEmailProvider() {}

LocalEmailProvider::~LocalEmailProvider() {}

std::vector<std::string> LocalEmailProvider::getAllFolderPaths(Session& ctx) {
    return accountFolderPaths[ctx.getEmailAddress()];
}

Folder LocalEmailProvider::getFolderByPath(Session& ctx, std::string folderPath, std::string sort) {
    if (sort == "none") {
        return folders[ctx.getEmailAddress() + "/" + folderPath];
    } else {
        throw NotImplementedException();
    }
}

Email LocalEmailProvider::getEmailById(Session& ctx, std::string emailId) {
    return emails[ctx.getEmailAddress() + "/" + emailId];
}

void LocalEmailProvider::sendEmail(Session& ctx, Email email) {
    std::string user = ctx.getEmailAddress();

    // sending
    emails[user + "/" + email.id] = email;
    std::string threadId = email.threadId;
    if (threadId == "new") {
        Thread t = Thread(email.subject);
        threads[user + "/" + t.id] = t;
        threadId = t.id;
        folders[user + "/sent"].threadIds.emplace_back(threadId);
    }
    threads[user + "/" + threadId].emailIds.emplace_back(email.id);

    //receiving
    for (auto recipient: email.to) {
        emails[recipient + "/" + email.id] = email;
        if (email.threadId == "new") {
            Thread t = Thread(email.subject);
            t.emailIds.emplace_back(email.id);
            threads[recipient + "/" + t.id] = t;
            folders[recipient + "/inbox"].threadIds.emplace_back(threadId);
        } else {
            for (auto& t: threads) {
                if (t.second.title == email.subject && t.first.substr(1, t.first.find("/", 1)) == recipient) {
                    t.second.emailIds.emplace_back(email.id);
                    break;
                }
            }
        }
    }
}

Thread LocalEmailProvider::getThreadById(Session& ctx, std::string threadId) {
    return threads[ctx.getEmailAddress() + "/" + threadId];
}

void LocalEmailProvider::addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath) {
    folders[ctx.getEmailAddress() + "/" + folderPath].
        threadIds.emplace_back(threads[ctx.getEmailAddress() + "/" + threadId].id);
}

void LocalEmailProvider::removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath) {
    Thread t = threads[ctx.getEmailAddress() + "/" + threadId];
    Folder& f = folders[ctx.getEmailAddress() + "/" + folderPath];
    

    for (auto i = f.threadIds.begin(); i != f.threadIds.end(); ++i) {
        if (*i == t.id) {
            f.threadIds.erase(i);
            break;
        }
    }

    folders[ctx.getEmailAddress() + "/deleted"].threadIds.emplace_back(t.id);
}

std::string LocalEmailProvider::addFolder(Session& ctx, std::string folderPath) {
    folders[ctx.getEmailAddress() + "/" + folderPath] = Folder();
    accountFolderPaths[ctx.getEmailAddress()].push_back(folderPath);
    return folderPath;
}

void LocalEmailProvider::removeFolder(Session& ctx, std::string folderPath) {
    auto f = folders.find(ctx.getEmailAddress() + "/" + folderPath);
    if (f != folders.end()) {
        for (auto& threadId: f->second.threadIds) {
            auto t = threads.find(ctx.getEmailAddress() + "/" + threadId);
            if (t != threads.end()) {
                for (auto& emailId: t->second.emailIds) {
                    emails.erase(ctx.getEmailAddress() + "/" + emailId);
                }
                threads.erase(t);
            }
        }
        folders.erase(f);
    }
}

Session LocalEmailProvider::getSession(std::string emailAddress, std::string password) {
    std::size_t hash = accounts[emailAddress];
    if (hash == std::hash<std::string>{}(password)) {
        return Session(emailAddress, std::to_string(hash));
    } else {
        throw AuthenticationFailedException();
    }
}

void LocalEmailProvider::addAccount(std::string emailAddress, std::string password) {
    // unsecure hash!
    accounts[emailAddress] = std::hash<std::string>{}(password);
}

std::ostream& LocalEmailProvider::serialize(std::ostream& sout) const {
    return sout;
}

std::istream& LocalEmailProvider::deserialize(std::istream& sin) {
    return sin;
}