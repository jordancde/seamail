#include "exceptions/authenticationFailedException.h"

using namespace std;
int main(){
    try {
        throw AuthenticationFailedException{};
    } catch (AuthenticationFailedException &e) {
    }
    return 0;
}
