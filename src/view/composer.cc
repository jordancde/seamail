#include "composer.h"

#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Composer::Composer(Email e) : email{e} {}

string Composer::fileName = "/tmp/.out." + to_string(getpid());
string Composer::cmd = "nano " + fileName;

string Composer::commaSeperate(vector<string> vec) {
    string separator;
    string out = "";
    for (auto e : vec) {
        out += separator + e;
        separator = ",";
    }
    return out;
}

vector<string> Composer::splitByDelim(string line, char delim) {
    stringstream ss(line);
    vector<string> splitted;
    string item;
    while (getline(ss, item, delim)) splitted.push_back(item);
    return splitted;
}

string Composer::getValFromHeader(string line) {
    string item;
    stringstream ss(line);
    vector<string> splitted;
    while (getline(ss, item, ':')) splitted.push_back(item);
    return (splitted.size() < 2) ? "" : splitted.at(1);
}

vector<string> Composer::getListFromHeader(string line) {
    vector<string> splitted = splitByDelim(line, ':');
    if (splitted.size() < 2) return vector<string>{};
    vector<string> vals = splitByDelim(splitted.at(1), ',');
    return vals;
}

void Composer::readEmail() {
    ifstream tmpFile(fileName);

    string line;
    getline(tmpFile, line);

    getline(tmpFile, line);  // from
    email.from = getValFromHeader(line);

    getline(tmpFile, line);  // to
    email.to = getListFromHeader(line);

    getline(tmpFile, line);  // cc
    email.cc = getListFromHeader(line);

    getline(tmpFile, line);  // bcc
    email.bcc = getListFromHeader(line);

    getline(tmpFile, line);  // subject
    email.subject = getValFromHeader(line);

    getline(tmpFile, line);  // image paths
    email.imagePaths = getListFromHeader(line);

    getline(tmpFile, line);
    email.body = "";

    string newline;  // body
    while (getline(tmpFile, line)) {
        email.body += newline + line;
        newline = '\n';
    }

    tmpFile.close();
}

void Composer::writeEmail() {
    ofstream tmpFile;
    tmpFile.open(fileName);

    tmpFile << "* Comma-seperate multiple entries" << endl;
    tmpFile << "FROM:" << email.from << endl;
    tmpFile << "TO*:" << commaSeperate(email.to) << endl;
    tmpFile << "CC*:" << commaSeperate(email.cc) << endl;
    tmpFile << "BCC*:" << commaSeperate(email.bcc) << endl;
    tmpFile << "SUBJECT:" << email.subject << endl;
    tmpFile << "IMAGE_PATHS*:" << commaSeperate(email.imagePaths) << endl;
    tmpFile << string(20, '-') << endl;
    tmpFile << email.body << endl;

    tmpFile.close();
}

void Composer::compose() {
    writeEmail();
    system(cmd.c_str());
    unlink(fileName.c_str());
    readEmail();
}

Email Composer::toEmail() { return email; }
