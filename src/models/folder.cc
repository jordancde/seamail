#include "folder.h"

#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>

using namespace std;

ostream& Folder::serialize(ostream& out) const {
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

void to_json(nlohmann::json& j, const Folder& f) {
    stringstream s;
    f.serialize(s);
    s >> j;
}

void from_json(const nlohmann::json& j, Folder& f) {
    stringstream s;
    s << j;
    f.deserialize(s);
}