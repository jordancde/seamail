#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <vector>
#include <iostream>
#include "../utility/serializable.h"

class Folder: public Serializable {

    vector<string> threadIds;
    string id;
    string folderName;

    vector<string> subFolderIds;

public:
    ostream& serialize(ostream&) override;
    istream& deserialize(istream&) override;

    vector<string>::Iterator begin();
    vector<string>::Iterator end();

    Folder(string id, string folderName, vector<string> threadIds = {}, vector<string> subFolderIds = {});
    ~Folder();
};

#endif