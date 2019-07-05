#include "thread.h"

#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>

using namespace std;

ostream& Thread::serialize(ostream& out) const {
    nlohmann::json thread;
    thread["id"] = id;
    thread["title"] = title;
    thread["emailIds"] = emailIds;
    return out << thread;
}

istream& Thread::deserialize(istream& in) {
    nlohmann::json thread;
    in >> thread;
    id = thread["id"];
    title = thread["title"];
    emailIds = thread["emailIds"].get<vector<string>>();
    return in;
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

vector<string>::iterator Thread::begin(){
    return emailIds.begin();
}

vector<string>::iterator Thread::end(){
    return emailIds.end();
}

Thread::Thread(string title, vector<string> emailIds):id{genRandomId()}, title{title}, emailIds{emailIds}{}
Thread::Thread(const Thread& other): id{genRandomId()}, title{other.title}, emailIds{other.emailIds}{}

Thread::Thread(){}
Thread::~Thread(){}

void to_json(nlohmann::json& j, const Thread& t) {
    stringstream s;
    t.serialize(s);
    s >> j;
}

void from_json(const nlohmann::json& j, Thread& t) {
    stringstream s;
    s << j; 
    t.deserialize(s);
}