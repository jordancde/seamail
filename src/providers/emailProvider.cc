#include "./emailProvider.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

using namespace std;

EmailProvider::EmailProvider() {}

EmailProvider::~EmailProvider() {}

ostream& EmailProvider::serialize(ostream& sout) const {
    nlohmann::json provider;

    provider["accountFolderPaths"] = accountFolderPaths;
    provider["folders"] = folders;
    provider["threads"] = threads;
    provider["emails"] = emails;
    
    return sout << provider;
}

istream& EmailProvider::deserialize(istream& sin) {
    nlohmann::json provider;
    sin >> provider;
    
    accountFolderPaths = provider["accountFolderPaths"]
        .get<unordered_map<string, vector<string>>>();
    folders = provider["folders"]
        .get<unordered_map<string, Folder>>();
    threads = provider["threads"]
        .get<unordered_map<string, Thread>>();
    emails = provider["emails"]
        .get<unordered_map<string, Email>>();

    return sin;
}