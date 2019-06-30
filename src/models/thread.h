#ifndef _THREAD_H_
#define _THREAD_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Thread: public Serializable {

    vector<string> emailIds;
    string id;

public:
    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;
    
    vector<string>::Iterator begin();
    vector<string>::Iterator end();

    Thread(string id, vector<string> emailIds = {});
    ~Thread();
};

#endif