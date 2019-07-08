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
        myLocalEmailProvider, "mydummyaccount@example.coms");
    myLocalEmailProvider->addAccount("mydummyaccount@example.com", "abc123");
    myDummyAccount->login("mydummyaccount@example.com", "abc123");

    Email e = Email("", "test@from", vector<string>{"test@to"}, time(NULL),
                    vector<string>{}, vector<string>{}, "test subject",
                    "test body", false, vector<string>{});
    myDummyAccount->sendEmail(e);

    Email sentEmail = myDummyAccount->getEmailById(e.id);

    bool emailDefaultUnread = !sentEmail.read;

    myDummyAccount->setEmailStatus(e.id, true);
    sentEmail = myDummyAccount->getEmailById(e.id);

    bool setEmailRead = sentEmail.read;

    myDummyAccount->setEmailStatus(e.id, false);
    sentEmail = myDummyAccount->getEmailById(e.id);

    bool setEmailUnread = !sentEmail.read;

    return !(emailDefaultUnread && setEmailRead && setEmailUnread);
}