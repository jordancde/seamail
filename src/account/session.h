#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <string>
#include "../utility/serializable.h"

class Session: public Serializable {
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

    std::string username;
    std::string key;

public:
    std::string getUsername();
    std::string getKey();

    Session(std::string username, std::string key);
    ~Session();
};

#endif