#include "thread.h"

using namespace std;

Thread::Thread(string title, vector<string> emailIds)
    : id{genRandomId()}, title{title}, emailIds{emailIds} {}

void Thread::serialize(nlohmann::json& thread) const {
    thread["id"] = id;
    thread["title"] = title;
    thread["emailIds"] = emailIds;
}

void Thread::deserialize(const nlohmann::json& thread) {
    id = thread["id"];
    title = thread["title"];
    emailIds = thread["emailIds"].get<vector<string>>();
}

string Thread::genRandomId() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int len = 16;
    std::string s(len, ' ');
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}