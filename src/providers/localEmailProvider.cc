#include "./localEmailProvider.h"

#include "../exceptions/authenticationFailedException.h"
#include "../exceptions/notImplementedException.h"

#include <algorithm>

using namespace std;

vector<string> LocalEmailProvider::getAllFolderPaths(Session& ctx) {
    return folderPaths[ctx.getEmailAddress()];
}

Folder LocalEmailProvider::getFolderByPath(Session& ctx, string folderPath,
                                           string sort) {
    if (sort == "none") {
        return folders[ctx.getEmailAddress() + "/" + folderPath];
    } else {
        throw NotImplementedException();
    }
}

Email LocalEmailProvider::getEmailById(Session& ctx, string emailId) {
    return emails[ctx.getEmailAddress() + "/" + emailId];
}

void LocalEmailProvider::sendEmail(Session& ctx, Email email) {
    string user = ctx.getEmailAddress();
    // sending
    emails[user + "/" + email.id] = email;
    string threadId = email.threadId;
    if (threadId == "new") {
        Thread t = Thread(email.subject);
        threads[user + "/" + t.id] = t;
        threadId = t.id;
        folders[user + "/sent"].threadIds.emplace_back(threadId);
    }
    threads[user + "/" + threadId].emailIds.emplace_back(email.id);

    // receiving
    for (auto recipient : email.to) {
        email.changeId();
        emails[recipient + "/" + email.id] = email;
        if (email.threadId == "new") {
            Thread t = Thread(email.subject);
            t.emailIds.emplace_back(email.id);
            threads[recipient + "/" + t.id] = t;
            folders[recipient + "/inbox"].threadIds.emplace_back(t.id);
        } else {
            for (auto& t : threads) {
                if (t.second.title == email.subject &&
                    t.first.substr(1, t.first.find("/", 1)) == recipient) {
                    t.second.emailIds.emplace_back(email.id);
                    break;
                }
            }
        }
    }
}

Thread LocalEmailProvider::getThreadById(Session& ctx, string threadId) {
    return threads[ctx.getEmailAddress() + "/" + threadId];
}

void LocalEmailProvider::addThreadToFolder(Session& ctx, string threadId,
                                           string folderPath) {
    folders[ctx.getEmailAddress() + "/" + folderPath].threadIds.emplace_back(
        threads[ctx.getEmailAddress() + "/" + threadId].id);
}

void LocalEmailProvider::removeThreadFromFolder(Session& ctx, string threadId,
                                                string folderPath) {
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

void LocalEmailProvider::addFolder(Session& ctx, string folderPath) {
    folders[ctx.getEmailAddress() + "/" + folderPath] = Folder(folderPath);
    folderPaths[ctx.getEmailAddress()].push_back(folderPath);
}

void LocalEmailProvider::removeFolder(Session& ctx, string folderPath) {
    auto f = folders.find(ctx.getEmailAddress() + "/" + folderPath);
    if (f != folders.end()) {
        for (auto& threadId : f->second.threadIds) {
            auto t = threads.find(ctx.getEmailAddress() + "/" + threadId);
            if (t != threads.end()) {
                for (auto& emailId : t->second.emailIds) {
                    emails.erase(ctx.getEmailAddress() + "/" + emailId);
                }
                threads.erase(t);
            }
        }
        folders.erase(f);

        folderPaths[ctx.getEmailAddress()].erase(
            remove(folderPaths[ctx.getEmailAddress()].begin(),
                   folderPaths[ctx.getEmailAddress()].end(), folderPath),
            folderPaths[ctx.getEmailAddress()].end());
    }
}

Session LocalEmailProvider::getSession(string emailAddress, string password) {
    size_t hash = accounts[emailAddress];
    if (hash == ::hash<string>{}(password)) {
        return Session(emailAddress, to_string(hash));
    } else {
        throw AuthenticationFailedException();
    }
}

void LocalEmailProvider::setEmailStatus(Session& ctx, string emailId, bool read){
    emails[ctx.getEmailAddress() + "/" + emailId].read = read;
}

void LocalEmailProvider::addAccount(string emailAddress, string password) {
    // unsecure hash!
    accounts[emailAddress] = hash<string>{}(password);
}

void LocalEmailProvider::serialize(nlohmann::json& provider) const {
    EmailProvider::serialize(provider);
    provider["accounts"] = accounts;
}

void LocalEmailProvider::deserialize(const nlohmann::json& provider) {
    EmailProvider::deserialize(provider);
    accounts = provider["accounts"].get<unordered_map<string, size_t>>();
}