#include "../models/email.h"

#include <stdio.h>
#include <string>
#include <vector>

class Composer {
    static std::string cmd;
    static std::string fileName;
    std::string getFilename();
    std::string commaSeperate(std::vector<std::string>);
    std::string getValFromHeader(std::string);
    std::vector<std::string> getListFromHeader(std::string);
    std::vector<std::string> splitByDelim(std::string, char);
    Email email;

   public:
    Composer(Email e = Email());

    void compose();
    Email toEmail();

    // public for testing purposes
    void writeEmail();
    void readEmail();
};