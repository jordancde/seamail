#ifndef _THREAD_H_
#define _THREAD_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Thread: public Serializable {

    std::vector<std::string> emailIds;
    std::string id;

public:
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;
    
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Thread(std::string id, std::vector<std::string> emailIds = {});
    ~Thread();
};

#endif