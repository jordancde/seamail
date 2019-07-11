#ifndef ACCOUNTALREADYEXISTS_H
#define ACCOUNTALREADYEXISTS_H

#include <stdexcept>

class AccountAlreadyExists : public std::logic_error {
public:
    AccountAlreadyExists() : std::logic_error("Account already exists.") { };
};

#endif