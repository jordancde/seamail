#include "./account.h"

#include "../events/accountEvent.h"
#include "../exceptions/authenticationFailedException.h"

using namespace std;

string Account::sentPath = "sent";
string Account::deletedPath = "deleted";
string Account::inboxPath = "inbox";

Account::Account(shared_ptr<EmailProvider> provider, 
    string emailAddress) : provider{provider}, emailAddress{emailAddress} {}

Account::Account() {}

Account::~Account() {}

void Account::serialize(nlohmann::json& account) const {
    account["session"] = session;
}

void Account::deserialize(const nlohmann::json& account) {
    session = account["session"].get<Session>();
}

vector<string> Account::getAllFolderPaths() {
    return provider->getAllFolderPaths(session);
}

Folder Account::getFolderByPath(string path, string sort) {
    return provider->getFolderByPath(session, path, sort);
}

Email Account::getEmailById(string id) {
    return provider->getEmailById(session, id);
}

void Account::sendEmail(Email email) {
    provider->sendEmail(session, email);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, sentPath));
    notifyAllObservers(e);
}

void Account::addThreadToFolder(string threadId, string folderPath) {
    provider->addThreadToFolder(session, threadId, folderPath);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

void Account::removeThreadFromFolder(string threadId, string folderPath) {
    provider->removeThreadFromFolder(session, threadId, folderPath);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

Thread Account::getThreadById(string threadId){
    return provider->getThreadById(session, threadId);
}

string Account::addFolder(string folderPath) {
    string newPath = provider->addFolder(session, folderPath);
    shared_ptr<Event> e(new AccountEvent(ACCOUNT_FOLDERS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
    return newPath;
}

void Account::removeFolder(string folderPath) {
    provider->removeFolder(session, folderPath);
    shared_ptr<Event> e(new AccountEvent(ACCOUNT_FOLDERS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

bool Account::login(string emailAddress, string password) {
    session = provider->getSession(emailAddress, password);
    return true;
}

void Account::logout() {
    session = Session();
}

bool Account::operator==(const Account& rhs) const {
    return emailAddress == rhs.emailAddress;
}
Session Account::getSession(){
    return session;
}

