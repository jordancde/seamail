#ifndef AUTHENTICATIONFAILED_H
#define AUTHENTICATIONFAILED_H

#include <stdexcept>

class AuthenticationFailedException : public std::logic_error {
public:
    AuthenticationFailedException() : std::logic_error("Failed to login.") { };
};

#endif