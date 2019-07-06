#include "./emailProvider.h"

using namespace std;

EmailProvider::EmailProvider() {}

EmailProvider::~EmailProvider() {}

void EmailProvider::serialize(nlohmann::json& provider) const {
    provider["accountFolderPaths"] = accountFolderPaths;
    provider["folders"] = folders;
    provider["threads"] = threads;
    provider["emails"] = emails;
}

void EmailProvider::deserialize(const nlohmann::json& provider) {
    accountFolderPaths = provider["accountFolderPaths"]
                                .get<unordered_map<string, vector<string>>>();
    folders = provider["folders"]
                    .get<unordered_map<string, Folder>>();
    threads = provider["threads"]
                    .get<unordered_map<string, Thread>>();
    emails = provider["emails"]
                    .get<unordered_map<string, Email>>();
}