#include <iostream>
#include "account/account.h"
#include "account/session.h"
#include "exceptions/authenticationFailedException.h"
#include "providers/localEmailProvider.h"

using namespace std;

int main() {
    LocalEmailProvider myLocalEmailProvider{};
    auto myDummyAccount = std::make_shared<Account>(myLocalEmailProvider);
    myLocalEmailProvider.addAccount("mydummyaccount@example.com", "abc123");
    myDummyAccount->login("mydummyaccount@example.com", "abc123");
    myDummyAccount->logout();

    Session s = myDummyAccount->getSession();
    string email = s.getEmailAddress();
    string key = s.getKey();

    bool emailValid = email == "";
    bool keyValid = key == "";

    return !(keyValid && emailValid);
}
