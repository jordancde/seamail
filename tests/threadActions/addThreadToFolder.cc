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

    Folder sent = myDummyAccount->getFolderByPath("sent");
    Thread thread = myDummyAccount->getThreadById(sent.threadIds.at(0));

    myDummyAccount->addFolder("Test1");
    myDummyAccount->addThreadToFolder(thread.id, "Test1");

    Folder test1 = myDummyAccount->getFolderByPath("Test1");

    return !(test1.threadIds.at(0) == thread.id);
}