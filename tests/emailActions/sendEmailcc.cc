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
    auto myDummyAccount3 = std::make_shared<Account>(
        myLocalEmailProvider, "mydummyaccount3@example.com");

    myLocalEmailProvider->addAccount("mydummyaccount@example.com", "abc123");
    myLocalEmailProvider->addAccount("mydummyaccount2@example.com", "abc123");
    myLocalEmailProvider->addAccount("mydummyaccount3@example.com", "abc123");

    myDummyAccount->login("mydummyaccount@example.com", "abc123");
    myDummyAccount2->login("mydummyaccount2@example.com", "abc123");
    myDummyAccount3->login("mydummyaccount3@example.com", "abc123");

    Email e =
        Email("new", "mydummyaccount@example.com",
              vector<string>{"mydummyaccount2@example.com"}, time(NULL),
              vector<string>{"mydummyaccount3@example.com"}, vector<string>{},
              "test subject", "test body", false, vector<string>{});

    myDummyAccount->sendEmail(e);

    Folder sent = myDummyAccount->getFolderByPath("sent");
    Thread thread = myDummyAccount->getThreadById(sent.threadIds.at(0));
    string sentEmailId = thread.emailIds.at(0);
    Email sentEmail = myDummyAccount->getEmailById(sentEmailId);
    bool senderSideValid = sentEmail == e;

    Folder inbox = myDummyAccount3->getFolderByPath("inbox");
    Thread recievedThread =
        myDummyAccount3->getThreadById(inbox.threadIds.front());
    string recievedEmailId = recievedThread.emailIds.front();
    Email recievedEmail = myDummyAccount3->getEmailById(recievedEmailId);

    bool ccvalid = recievedEmail.cc.size() == 1 &&
                   recievedEmail.cc.at(0) == "mydummyaccount3@example.com";
    bool tovalid = recievedEmail.to.size() == 1 &&
                   (recievedEmail.to.at(0) == "mydummyaccount2@example.com");
    bool recieverSideValid = recievedEmail.same(e);

    return !(senderSideValid && tovalid && ccvalid && recieverSideValid);
}