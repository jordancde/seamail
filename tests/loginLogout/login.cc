#include "providers/localEmailProvider.h"
#include "account/account.h"
#include "account/session.h"
#include "exceptions/authenticationFailedException.h"
#include <iostream>

using namespace std;

int main() {
    LocalEmailProvider myLocalEmailProvider{};
	auto myDummyAccount = std::make_shared<Account>(myLocalEmailProvider);
    myLocalEmailProvider.addAccount("mydummyaccount@example.com", "abc123");
    myDummyAccount->login("mydummyaccount@example.com", "abc123");

    try {
        myDummyAccount->login("mydummyaccount@example.com", "abc123");
    } catch (AuthenticationFailedException &e) {
        return 1;
    }

    Session s = myDummyAccount->getSession();
    string email = s.getEmailAddress();
    string key = s.getKey();

    bool emailValid = email == "mydummyaccount@example.com";
    bool keyValid = key == "abc123";

    return !(keyValid&&emailValid);
}
