#include "email.h"

using namespace std;

ostream& Email::serialize(ostream& out) const {

}

istream& Email::deserialize(istream& in){
    
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