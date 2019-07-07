#include <iostream>
#include "account/account.h"
#include "models/folder.h"
#include "providers/localEmailProvider.h"

using namespace std;

int main() {
    auto myLocalEmailProvider = std::make_shared<LocalEmailProvider>();
    auto myDummyAccount = std::make_shared<Account>(
        myLocalEmailProvider, "mydummyaccount@example.coms");
    myLocalEmailProvider->addAccount("mydummyaccount@example.com", "abc123");
    myDummyAccount->login("mydummyaccount@example.com", "abc123");

    myDummyAccount->addFolder("Test1");
    myDummyAccount->addFolder("Test2");
    myDummyAccount->addFolder("Test3");
    myDummyAccount->addFolder("Test4");

    Folder f = myDummyAccount->getFolderByPath("Test2", "none");

    string expected = "Test2";
    return f.path != expected;
}
