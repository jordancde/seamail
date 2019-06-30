#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

#include <iostream>

class Serializable {

public:
    virtual ostream& serialize(ostream&)=0;
    virtual istream& deserialize(istream&)=0;
    friend ostream& operator<<(ostream&,const Serializable&);
    friend istream& operator>>(istream&,const Serializable&);
};

#endif
