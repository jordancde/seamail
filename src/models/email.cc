#include "email.h"

#include <vector>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>

using namespace std;

ostream& Email::serialize(ostream& out) const {
    nlohmann::json email;
    email["id"] = id;
    email["from"] = from;
    email["to"] = to;
    email["dateTime"] = dateTime;
    email["cc"] = cc;
    email["bcc"] = bcc;
    email["subject"] = subject;
    email["body"] = body;
    email["unread"] = unread;
    email["imagePaths"] = imagePaths;
    email["threadId"] = threadId;
    return out << email;
}

istream& Email::deserialize(istream& in){
    nlohmann::json email;
    in >> email;
    id = email["id"];
    from = email["from"];
    dateTime = email["dateTime"];
    cc = email["cc"].get<vector<string>>();
    bcc = email["bcc"].get<vector<string>>();
    subject = email["subject"];
    body = email["body"];
    imagePaths = email["imagePaths"].get<vector<string>>();
    threadId = email["threadId"];
    return in;
}

string Email::genRandomId() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int len = 16;
    std::string s(len, ' ');
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}

Email::Email(
    std::string threadId,
    std::string from,
    std::vector<std::string> to,
    time_t dateTime,
    std::vector<std::string> cc, 
    std::vector<std::string> bcc,
    std::string subject,
    std::string body,
    bool unread,
    std::vector<std::string> imagePaths
    
    ): id{genRandomId()}, threadId{threadId}, from{from}, to{to}, dateTime{dateTime}, cc{cc}, bcc{bcc},
        subject{subject}, body{body}, unread{unread}, imagePaths{imagePaths}{}

Email::Email(const Email& other): 
    id{genRandomId()}, threadId{other.threadId}, from{other.from}, 
    to{other.to}, dateTime{other.dateTime}, cc{other.cc}, 
    bcc{other.bcc}, subject{other.subject}, body{other.body},
    unread{other.unread}, imagePaths{other.imagePaths} {}

Email::Email() {}
Email::~Email() {}

void to_json(nlohmann::json& j, const Email& e) {
    stringstream s;
    e.serialize(s);
    s >> j;
}

void from_json(const nlohmann::json& j, Email& e) {
    stringstream s;
    s << j;
    e.deserialize(s);
}