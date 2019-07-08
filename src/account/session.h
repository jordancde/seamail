#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Session : public Serializable {
    std::string emailAddress;
    std::string key;

    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    Session(std::string emailAddress, std::string key);
    Session() = default;
    
    std::string getEmailAddress() const;
    std::string getKey() const;
};

#endif