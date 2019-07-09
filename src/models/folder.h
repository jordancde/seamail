#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Folder : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    std::string path;
    std::vector<std::string> threadIds;

    Folder(std::string path, std::vector<std::string> threadIds = {});
    Folder() = default;
    bool operator==(const Folder&) const;
};

#endif