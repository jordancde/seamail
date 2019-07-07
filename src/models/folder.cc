#include "folder.h"

using namespace std;

Folder::Folder(string path, vector<string> threadIds) : path{path}, 
    threadIds{threadIds} {}

void Folder::serialize(nlohmann::json& folder) const {
    folder["path"] = path;
    folder["threadIds"] = threadIds;
}

void Folder::deserialize(const nlohmann::json& folder) {
    path = folder["path"];
    threadIds = folder["threadIds"].get<vector<string>>();
}