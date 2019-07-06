#include "session.h"

using namespace std;


Session::Session() : emailAddress{""}, key{""} {}

Session::Session(string emailAddress, string key) : emailAddress{emailAddress}, key{key} {}

Session::~Session() {}

void Session::serialize(nlohmann::json& session) const {
    session["emailAddress"] = emailAddress;
    session["key"] = key;
}

void Session::deserialize(const nlohmann::json& session) {
    emailAddress = session["emailAddress"];
    key = session["key"];
}

string Session::getEmailAddress() { 
    return emailAddress; 
}

string Session::getKey() {
    return key; 
}
