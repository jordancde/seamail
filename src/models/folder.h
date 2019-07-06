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
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Folder(std::string path, std::vector<std::string> threadIds = {});
    Folder();
    ~Folder();
};

#endif