#include "./emailProvider.h"

using namespace std;

string EmailProvider::sentPath = "sent";
string EmailProvider::deletedPath = "deleted";
string EmailProvider::inboxPath = "inbox";

EmailProvider::EmailProvider() {}

EmailProvider::~EmailProvider() {}

void EmailProvider::serialize(nlohmann::json& provider) const {
    provider["folderPaths"] = folderPaths;
    provider["folders"] = folders;
    provider["threads"] = threads;
    provider["emails"] = emails;
}

void EmailProvider::deserialize(const nlohmann::json& provider) {
    folderPaths = provider["folderPaths"]
                                .get<unordered_map<string, vector<string>>>();
    folders = provider["folders"]
                    .get<unordered_map<string, Folder>>();
    threads = provider["threads"]
                    .get<unordered_map<string, Thread>>();
    emails = provider["emails"]
                    .get<unordered_map<string, Email>>();
}