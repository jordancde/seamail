#include <ctime>
#include <iostream>
#include "models/email.h"
#include "view/composer.h"

using namespace std;

int main() {
    Email e = Email("", "test@from", vector<string>{"test@to"}, time(NULL),
                    vector<string>{"test1", "test2"},
                    vector<string>{"testff1", "teffewst2"}, "test subject",
                    "test body", false, vector<string>{"test13r2", "test223r"});

    Composer composer = Composer(e);

    composer.writeEmail();
    composer.readEmail();

    Email out = composer.toEmail();

    return !(out == e);
}