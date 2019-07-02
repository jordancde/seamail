#include "serializable.h"

using namespace std;

ostream& operator<<(ostream& sout, const Serializable& obj) {
    return obj.serialize(sout);
}

istream& operator>>(istream& sin, Serializable& obj) {
    return obj.deserialize(sin);
}