#include "email.h"

using namespace std;

Email::Email(string threadId, string from, vector<string> to, time_t dateTime,
             vector<string> cc, vector<string> bcc, string subject, string body,
             bool unread, vector<string> imagePaths

             )
    : id{genRandomId()},
      threadId{threadId},
      from{from},
      to{to},
      dateTime{dateTime},
      cc{cc},
      bcc{bcc},
      subject{subject},
      body{body},
      unread{unread},
      imagePaths{imagePaths} {}

Email::Email() {}

Email::~Email() {}

void Email::serialize(nlohmann::json& email) const {
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
}

void Email::deserialize(const nlohmann::json& email) {
    id = email["id"];
    from = email["from"];
    dateTime = email["dateTime"];
    cc = email["cc"].get<vector<string>>();
    bcc = email["bcc"].get<vector<string>>();
    subject = email["subject"];
    body = email["body"];
    imagePaths = email["imagePaths"].get<vector<string>>();
    threadId = email["threadId"];
}

void Email::changeId() { id = genRandomId(); }

string Email::genRandomId() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int len = 16;
    string s(len, ' ');
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}

bool Email::operator==(const Email& rhs) const {
    return from == rhs.from && to == rhs.to && dateTime == rhs.dateTime &&
           cc == rhs.cc && bcc == rhs.bcc && subject == rhs.subject &&
           body == rhs.body;
}