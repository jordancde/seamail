#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include "../utility/serializable.h"

class Session: public Serializable {
    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;
public:
    string username;
    string key;
};

#endif