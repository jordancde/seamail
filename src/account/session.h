#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <string>
#include "../utility/serializable.h"

class Session: public Serializable {
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;
public:
    std::string username;
    std::string key;

    Session(std::string username, std::string key);
    ~Session();
};

#endif