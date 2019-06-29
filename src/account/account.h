#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <vector>

class Account: public Subject, public Serializable {
    Session session;
    EmailProvider provider;

public:
    vector<string> getAllFolderPaths();
    Folder getFolderByPath(string, string);
    Email getEmailById(string);
    void sendEmail(Email);
    void addEmailToFolder(string, string);
    void removeEmailFromFolder(string,string)
    string addFolder(string,string);
    void removeFolder(string);
    bool login(string,string);
    void logout();
};


#endif