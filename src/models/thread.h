#ifndef _THREAD_H_
#define _THREAD_H_

#include <vector>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Thread: public Serializable {

    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;
    std::string genRandomId();

public:
    std::string id;
    std::string title;

    std::vector<std::string> emailIds;
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Thread(std::string title, std::vector<std::string> emailIds = {});
    Thread(const Thread&);
    Thread();
    ~Thread();

    friend void to_json(nlohmann::json&, const Thread&);
    friend void from_json(const nlohmann::json&, Thread&);
};

#endif