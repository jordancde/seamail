#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <vector>
#include <iostream>
#include <string>

#include "../utility/serializable.h"

class Email: public Serializable {
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

public:
    std::string id;
    std::string from;
    std::vector<std::string> to;
    time_t dateTime;
    std::vector<std::string> cc;
    std::vector<std::string> bcc;
    std::string subject;
    std::string body;
    bool unread;
    std::vector<std::string> imagePaths;

    Email(std::string id, std::string from, std::vector<std::string> to,time_t dateTime, std::vector<std::string> cc = {}, 
        std::vector<std::string> bcc = {}, std::string subject = "", std::string body = "", bool unread = false,
        std::vector<std::string> imagePaths = {});
    ~Email();
};

#endif