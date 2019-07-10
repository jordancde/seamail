#include "./account.h"
#include "../events/accountEvent.h"
#include "../exceptions/authenticationFailedException.h"

#include <memory>

using namespace std;

void Account::serialize(nlohmann::json& account) const {
    account["session"] = session;
    account["emailAddress"] = emailAddress;
}

void Account::deserialize(const nlohmann::json& account) {
    session = account["session"].get<Session>();
    emailAddress = account["emailAddress"];
}

Account::Account(shared_ptr<EmailProvider> provider, string emailAddress)
    : provider{provider}, emailAddress{emailAddress}, loggedIn(false) {}

void Account::login(string emailAddress, string password) {
    session = provider->getSession(emailAddress, password);
    loggedIn = true;
}

void Account::logout() { 
    session = Session(); 
    loggedIn = false;
}

Folder Account::getFolderByPath(string path) {
    return provider->getFolderByPath(session, path);
}

vector<string> Account::getAllFolderPaths() {
    return provider->getAllFolderPaths(session);
}

void Account::addFolder(string folderPath) {
    provider->addFolder(session, folderPath);
    auto e = make_shared<AccountEvent>(ACCOUNT_FOLDERS_CHANGED, *this, folderPath);
    notifyAllObservers(e);
}

void Account::removeFolder(string folderPath) {
    provider->removeFolder(session, folderPath);
    auto e = make_shared<AccountEvent>(ACCOUNT_FOLDERS_CHANGED, *this, folderPath);
    notifyAllObservers(e);
}

Thread Account::getThreadById(string threadId) {
    return provider->getThreadById(session, threadId);
}

void Account::addThreadToFolder(string threadId, string folderPath) {
    provider->addThreadToFolder(session, threadId, folderPath);
    auto e = make_shared<AccountEvent>(FOLDER_CONTENTS_CHANGED, *this, folderPath);
    notifyAllObservers(e);
}

void Account::removeThreadFromFolder(string threadId, string folderPath) {
    provider->removeThreadFromFolder(session, threadId, folderPath);
    auto e = make_shared<AccountEvent>(FOLDER_CONTENTS_CHANGED, *this, folderPath);
    notifyAllObservers(e);
}

Email Account::getEmailById(string id) {
    return provider->getEmailById(session, id);
}

void Account::setEmailStatus(string emailId, bool read){
    provider->setEmailStatus(session, emailId, read);
    auto e = make_shared<AccountEvent>(FOLDER_CONTENTS_CHANGED, *this, emailId);
    notifyAllObservers(e);
}

void Account::sendEmail(Email email) {
    provider->sendEmail(session, email);
    auto e = make_shared<AccountEvent>(FOLDER_CONTENTS_CHANGED, *this, provider->sentPath);
    notifyAllObservers(e);
}

bool Account::operator==(const Account& other) const{
    return emailAddress == other.emailAddress && session == other.session;
}

void Account::setProvider(shared_ptr<EmailProvider> newProvider) {
    provider = newProvider;
}

