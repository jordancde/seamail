#include "folder.h"

#include <nlohmann/json.hpp>
#include <iostream>

using namespace std;

ostream& Folder::serialize(ostream& out) {
    nlohmann::json folder;
    folder["path"] = path;
    folder["threadIds"] = threadIds;
    return out << folder;
}

istream& Folder::deserialize(istream& in) {
    nlohmann::json folder;
    in >> folder;
    path = folder["path"];
    threadIds = folder["threadIds"].get<vector<string>>();
    return in;
}

vector<string>::iterator Folder::begin(){
    return threadIds.begin();
}

vector<string>::iterator Folder::end(){
    return threadIds.end();
}

Folder::Folder(string path, vector<string> threadIds):path{path}, threadIds{threadIds}{}
Folder::Folder(){}

Folder::~Folder(){}