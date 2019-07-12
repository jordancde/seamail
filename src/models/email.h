#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Email : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

    std::string genRandomId();

public:
    std::string id;
    std::string threadId;
    std::string from;
    std::vector<std::string> to;
    time_t dateTime;
    std::vector<std::string> cc;
    std::vector<std::string> bcc;
    std::string subject;
    std::string body;
    bool read;
    std::vector<std::string> imagePaths;
 
    Email(std::string threadId = "new", 
            std::string from = "", 
            std::vector<std::string> to = {},
            time_t dateTime = 0, 
            std::vector<std::string> cc = {},
            std::vector<std::string> bcc = {}, 
            std::string subject = "",
            std::string body = "", 
            bool read = false,
            std::vector<std::string> imagePaths = {});

    // Checks equality without id
    bool same(const Email&) const;
    // Equality with id
    bool operator==(const Email&) const;

    void changeId();
};

#endif