#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Email: public Serializable {

    

public:

    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;

    string id;
    string from;
    vector<string> to;
    time_t dateTime;
    vector<string> cc;
    vector<string> bcc;
    string subject;
    string body;
    bool unread;
    vector<string> imagePaths;

    Email(string id, string from, vector<string> to,time_t dateTime, vector<string> cc = {}, 
        vector<string> bcc = {}, string subject = "", string body = "", bool unread = false,
        vector<string> imagePaths = {});
    
    ~Email();
};

#endif