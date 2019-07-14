#include "./localEmailProvider.h"

#include "../exceptions/accountAlreadyExistsException.h"
#include "../exceptions/authenticationFailedException.h"
#include "../exceptions/notImplementedException.h"

#include <algorithm>
#include <unordered_set>

using namespace std;

void LocalEmailProvider::serialize(nlohmann::json& provider) const {
    EmailProvider::serialize(provider);
    provider["accounts"] = accounts;
}

void LocalEmailProvider::deserialize(const nlohmann::json& provider) {
    EmailProvider::deserialize(provider);
    accounts = provider["accounts"].get<unordered_map<string, size_t>>();
}

void LocalEmailProvider::addAccount(string emailAddress, string password) {
    if (accounts.find(emailAddress) != accounts.end()) {
        throw AccountAlreadyExists();
    }
    accounts[emailAddress] = hash<string>{}(password);  // unsecure hash!
    Session s = getSession(emailAddress, password);
    addFolder(s, sentPath);
    addFolder(s, inboxPath);
    addFolder(s, deletedPath);
}

Session LocalEmailProvider::getSession(string emailAddress, string password) {
    size_t hash = accounts[emailAddress];
    if (hash == ::hash<string>{}(password)) {
        return Session(emailAddress, to_string(hash));
    } else {
        throw AuthenticationFailedException();
    }
}

Folder LocalEmailProvider::getFolderByPath(Session& ctx, string folderPath) {
    return folders[ctx.getEmailAddress() + "/" + folderPath];
}

vector<string> LocalEmailProvider::getAllFolderPaths(Session& ctx) {
    return folderPaths[ctx.getEmailAddress()];
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

Thread LocalEmailProvider::getThreadById(Session& ctx, string threadId) {
    return threads[ctx.getEmailAddress() + "/" + threadId];
}

void LocalEmailProvider::addThreadToFolder(Session& ctx, string threadId,
                                           string folderPath) {
    if (threads.find(ctx.getEmailAddress() + "/" + threadId) == threads.end() ||
        folders.find(ctx.getEmailAddress() + "/" + folderPath) ==
            folders.end()) {
        return;
    }

    folders[ctx.getEmailAddress() + "/" + folderPath].threadIds.emplace_back(
        threads[ctx.getEmailAddress() + "/" + threadId].id);
}

void LocalEmailProvider::removeThreadFromFolder(Session& ctx, string threadId,
                                                string folderPath) {
    if (threads.find(ctx.getEmailAddress() + "/" + threadId) == threads.end() ||
        folders.find(ctx.getEmailAddress() + "/" + folderPath) ==
            folders.end()) {
        return;
    }

    Thread& t = threads[ctx.getEmailAddress() + "/" + threadId];
    Folder& f = folders[ctx.getEmailAddress() + "/" + folderPath];

    for (auto i = f.threadIds.begin(); i != f.threadIds.end(); ++i) {
        if (*i == t.id) {
            f.threadIds.erase(i);
            break;
        }
    }
}

Email LocalEmailProvider::getEmailById(Session& ctx, string emailId) {
    return emails[ctx.getEmailAddress() + "/" + emailId];
}

void LocalEmailProvider::sendEmail(Session& ctx, Email email) {
    string user = ctx.getEmailAddress();
    // sending
    email.dateTime = time(nullptr);
    emails[user + "/" + email.id] = email;
    string threadId = email.threadId;
    if (threadId == "new") {
        Thread t = Thread(email.subject);
        threads[user + "/" + t.id] = t;
        threadId = t.id;
        folders[user + "/sent"].threadIds.emplace_back(threadId);
        emails[user + "/" + email.id].threadId = threadId;
    }
    threads[user + "/" + threadId].emailIds.emplace_back(email.id);

    // check if already in sent folder, if not add
    vector<string>& sentThreadIds = folders[user + "/sent"].threadIds;
    if (find(sentThreadIds.begin(), sentThreadIds.end(), threadId) ==
        sentThreadIds.end()) {
        sentThreadIds.emplace_back(threadId);
    }

    // receiving
    vector<string> recipients;
    recipients.reserve(email.to.size() + email.cc.size() + email.bcc.size());
    recipients.insert(recipients.end(), email.to.begin(), email.to.end());
    recipients.insert(recipients.end(), email.cc.begin(), email.cc.end());
    recipients.insert(recipients.end(), email.bcc.begin(), email.bcc.end());
    unordered_set<string> bcc(email.bcc.begin(), email.bcc.end());
    for (auto recipient : recipients) {
        if (accounts.find(recipient) == accounts.end()) {
            continue;
        }

        email.changeId();
        email.dateTime = time(nullptr);
        emails[recipient + "/" + email.id] = email;
        if (bcc.find(recipient) != bcc.end()) {
            emails[recipient + "/" + email.id].to = vector<string>();
            emails[recipient + "/" + email.id].cc = vector<string>();
            emails[recipient + "/" + email.id].bcc = vector<string>{recipient};
        }
        if (email.threadId == "new") {
            Thread t = Thread(email.subject);
            emails[recipient + "/" + email.id].threadId = t.id;
            t.emailIds.emplace_back(email.id);
            threads[recipient + "/" + t.id] = t;
            folders[recipient + "/inbox"].threadIds.emplace_back(t.id);
        } else {
            // Find the matching thread in the recipients inbox (different
            // thread Id)
            for (auto& t : threads) {
                string recip = t.first.substr(0, t.first.find("/"));
                if (t.second.title == email.subject && recip == recipient) {
                    t.second.emailIds.emplace_back(email.id);
                    threadId = t.second.id;
                    emails[recipient + "/" + email.id].threadId = threadId;
                    break;
                }
            }

            // check if already in inbox folder, if not add
            vector<string>& inboxThreadIds =
                folders[recipient + "/inbox"].threadIds;
            if (find(inboxThreadIds.begin(), inboxThreadIds.end(), threadId) ==
                inboxThreadIds.end()) {
                inboxThreadIds.emplace_back(threadId);
            }
        }
    }
}

void LocalEmailProvider::setEmailStatus(Session& ctx, string emailId,
                                        bool read) {
    emails[ctx.getEmailAddress() + "/" + emailId].read = read;
}

bool LocalEmailProvider::operator==(const LocalEmailProvider& other) const {
    return accounts == other.accounts && EmailProvider::operator==(other);
}
