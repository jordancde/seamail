#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Folder: public Serializable {

    std::vector<std::string> threadIds;
    std::string id;
    std::string folderName;

    std::vector<std::string> subFolderIds;

public:
    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Folder(std::string id, std::string folderName, std::vector<std::string> threadIds = {},
        std::vector<std::string> subFolderIds = {});
    ~Folder();
};

#endif