#include "folder.h"

using namespace std;

Folder::Folder(string path, vector<string> threadIds) : path{path}, 
    threadIds{threadIds} {}

Folder::Folder() {}

Folder::~Folder() {}

void Folder::serialize(nlohmann::json& folder) const {
    folder["path"] = path;
    folder["threadIds"] = threadIds;
}

void Folder::deserialize(const nlohmann::json& folder) {
    path = folder["path"];
    threadIds = folder["threadIds"].get<vector<string>>();
}

vector<string>::iterator Folder::begin() {
    return threadIds.begin();
}

vector<string>::iterator Folder::end() {
    return threadIds.end();
}
