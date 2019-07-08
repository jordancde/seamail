#include <ctime>
#include <iostream>
#include "account/account.h"
#include "models/thread.h"
#include "providers/localEmailProvider.h"

using namespace std;

int main() {
    auto myLocalEmailProvider = std::make_shared<LocalEmailProvider>();
    auto myDummyAccount = std::make_shared<Account>(
        myLocalEmailProvider, "mydummyaccount@example.coms");
    myLocalEmailProvider->addAccount("mydummyaccount@example.com", "abc123");
    myDummyAccount->login("mydummyaccount@example.com", "abc123");

    Email e = Email("new", "mydummyaccount@example.com",
                    vector<string>{"mydummyaccount2@example.com"}, time(NULL),
                    vector<string>{}, vector<string>{}, "test subject",
                    "test body", false, vector<string>{});
    myDummyAccount->sendEmail(e);

    Folder sent = myDummyAccount->getFolderByPath("sent", "none");
    Thread thread = myDummyAccount->getThreadById(sent.threadIds.at(0));

    myDummyAccount->removeThreadFromFolder(thread.id, "sent");

    Folder deleted = myDummyAccount->getFolderByPath("deleted", "none");
    sent = myDummyAccount->getFolderByPath("sent", "none");

    bool threadMoved = deleted.threadIds.at(0) == thread.id;
    bool threadNotCopied = sent.threadIds.size() == 0;

    return !(threadMoved && threadNotCopied);
}