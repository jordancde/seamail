#ifndef _THREAD_H_
#define _THREAD_H_

#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Thread : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;
    
    std::string genRandomId();

public:
    std::string id;
    std::string title;
    std::vector<std::string> emailIds;

    explicit Thread(std::string title, std::vector<std::string> emailIds = {});
    Thread() = default;
    bool operator==(const Thread&) const;
};

#endif