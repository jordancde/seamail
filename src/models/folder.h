#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <vector>
#include <iostream>

#include <nlohmann/json.hpp>

#include "../utility/serializable.h"

class Folder: public Serializable {
    
    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;

public:
    std::string path;
    std::vector<std::string> threadIds;
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Folder(std::string path, std::vector<std::string> threadIds={});
    Folder();
    ~Folder();

    friend void to_json(nlohmann::json&, const Folder&);
    friend void from_json(const nlohmann::json&, Folder&);
};

#endif