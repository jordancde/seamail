#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Session : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

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