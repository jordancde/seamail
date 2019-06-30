#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

#include <iostream>

class Serializable {

public:
    virtual std::ostream& serialize(std::ostream&)=0;
    virtual std::istream& deserialize(std::istream&)=0;
    friend std::ostream& operator<<(std::ostream&, const Serializable&);
    friend std::istream& operator>>(std::istream&, const Serializable&);
};

#endif
