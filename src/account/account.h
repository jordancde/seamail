#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <vector>

class Account {

public:
    vector<string> getAllFolderPaths();
    Folder getFolderByPath(string, string);
    Email getEmailById(string, string);
    
};


#endif