#include "emailProvider.h"

using namespace std;

EmailProvider::EmailProvider() {}

EmailProvider::~EmailProvider() {}

ostream& EmailProvider::serialize(ostream& sout) const {
    
    return sout;
}

istream& EmailProvider::deserialize(istream& sin) {

    return sin;
}