#include "session.h"

using namespace std;

void Session::serialize(nlohmann::json& session) const {
    session["emailAddress"] = emailAddress;
    session["key"] = key;
}

void Session::deserialize(const nlohmann::json& session) {
    emailAddress = session["emailAddress"];
    key = session["key"];
}

Session::Session(string emailAddress, string key) : emailAddress{emailAddress}, key{key} {}

string Session::getEmailAddress() const { 
    return emailAddress; 
}

string Session::getKey() const {
    return key; 
}
