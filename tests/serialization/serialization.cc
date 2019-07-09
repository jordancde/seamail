#include <fstream>
#include <iostream>
#include "providers/localEmailProvider.h"
#include "utility/localState.h"

using namespace std;

int main() {
    LocalState ls = LocalState();
    ls.localProvider = std::make_shared<LocalEmailProvider>();

    auto myDummyAccount = std::make_shared<Account>(
        ls.localProvider, "mydummyaccount@example.com");

    ls.localProvider->addAccount("mydummyaccount@example.com", "abc123");

    ls.storeAccount(*myDummyAccount);
    myDummyAccount->login("mydummyaccount@example.com", "abc123");

    Email e = Email("new", "mydummyaccount@example.com",
                    vector<string>{"mydummyaccount2@example.com"}, time(NULL),
                    vector<string>{}, vector<string>{}, "test subject",
                    "test body", false, vector<string>{});

    myDummyAccount->sendEmail(e);

    ofstream outFile;
    outFile.open("serialized.txt");
    outFile << ls;
    outFile.close();

    ifstream inFile;

    inFile.open("serialized.txt");

    LocalState inputLS = LocalState();
    inFile >> inputLS;
    inFile.close();
    remove("serialized.txt");
    return !(inputLS==ls);
}
