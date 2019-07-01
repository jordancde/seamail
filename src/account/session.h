#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <string>
#include "../utility/serializable.h"

class Session: public Serializable {
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

    std::string emailAddress;
    std::string key;

public:
    std::string getEmailAddress();
    std::string getKey();

    Session();
    Session(std::string emailAddress, std::string key);
    ~Session();
};

#endif