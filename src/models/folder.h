#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <vector>
#include <iostream>

#include "../utility/serializable.h"

class Folder: public Serializable {
    std::string path;
    
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

public:
    std::vector<std::string> threadIds;
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Folder(std::string path, std::vector<std::string> threadIds);
    Folder();
    ~Folder();
};

#endif