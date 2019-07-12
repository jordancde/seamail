#ifndef COMPOSER_H
#define COMPOSER_H

#include "../models/email.h"

#include <stdio.h>
#include <string>
#include <vector>

class Composer {
    static std::string editCmd;
    static std::string viewCmd;
    static std::string fileName;
    std::string getFilename();
    std::string commaSeperate(std::vector<std::string>);
    std::string getValFromHeader(std::string);
    std::vector<std::string> getListFromHeader(std::string);
    std::vector<std::string> splitByDelim(std::string, char);
    Email email;
    bool readOnly;

   public:
    Composer(Email e = Email(), bool readOnly = false);

    void compose();
    Email toEmail();

    // public for testing purposes
    void writeEmail();
    void readEmail();
};

#endif
