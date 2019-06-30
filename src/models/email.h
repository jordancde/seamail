#ifndef _EMAIL_H_
#define _EMAIL_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Email: public Serializable {

    string id;

public:

    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;

    string from;
    vector<string> to;
    time_t dateTime;
    vector<string> cc;
    vector<string> bcc;
    string subject;
    string body;
    bool unread;
    vector<string> imagePaths;
};

#endif