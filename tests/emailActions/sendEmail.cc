#include <ctime>
#include <iostream>
#include "account/account.h"
#include "models/email.h"
#include "models/thread.h"
#include "providers/localEmailProvider.h"

using namespace std;

int main() {
    auto myLocalEmailProvider = std::make_shared<LocalEmailProvider>();

    auto myDummyAccount = std::make_shared<Account>(
        myLocalEmailProvider, "mydummyaccount@example.com");
    auto myDummyAccount2 = std::make_shared<Account>(
        myLocalEmailProvider, "mydummyaccount2@example.com");

    myLocalEmailProvider->addAccount("mydummyaccount@example.com", "abc123");
    myLocalEmailProvider->addAccount("mydummyaccount2@example.com", "abc123");

    myDummyAccount->login("mydummyaccount@example.com", "abc123");
    myDummyAccount2->login("mydummyaccount2@example.com", "abc123");

    Email e = Email("new", "mydummyaccount@example.com",
                    vector<string>{"mydummyaccount2@example.com"}, time(NULL),
                    vector<string>{}, vector<string>{}, "test subject",
                    "test body", false, vector<string>{});

    myDummyAccount->sendEmail(e);

    Folder sent = myDummyAccount->getFolderByPath("sent");
    Thread thread = myDummyAccount->getThreadById(sent.threadIds.at(0));
    string sentEmailId = thread.emailIds.at(0);
    Email sentEmail = myDummyAccount->getEmailById(sentEmailId);
    bool senderSideValid = sentEmail == e && e.id == sentEmail.id;

    Folder inbox = myDummyAccount2->getFolderByPath("inbox");
    Thread recievedThread =
        myDummyAccount2->getThreadById(inbox.threadIds.front());
    string recievedEmailId = recievedThread.emailIds.front();
    Email recievedEmail = myDummyAccount2->getEmailById(recievedEmailId);

    bool recieverSideValid = recievedEmail == e;
    return !(senderSideValid && recieverSideValid);
}