#include "session.h"
#include <iostream>

using namespace std;

ostream& Session::serialize(ostream& out){
    nlohmann::json session;
    session["emailAddress"] = emailAddress;
    session["key"] = key;
    return out << session;
}

istream& Session::deserialize(istream& in){
    nlohmann::json session;
    in >> session;
    emailAddress = session["emailAddress"];
    key = session["key"];
    return in;
}

string Session::getEmailAddress(){ return emailAddress; }
string Session::getKey(){ return key; }

Session::Session():emailAddress{""}, key{""}{}
Session::Session(string emailAddress, string key):emailAddress{emailAddress}, key{key}{}
Session::~Session();