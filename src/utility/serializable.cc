#include "serializable.h"

using namespace std;

ostream& operator<<(ostream& sout, const Serializable& obj) {
    nlohmann::json serialized;
    serialized = obj;
    return sout << serialized;
}

istream& operator>>(istream& sin, Serializable& obj) {
    nlohmann::json deserialized;
    sin >> deserialized;
    from_json(deserialized, obj);
    return sin;
} 

void to_json(nlohmann::json& j, const Serializable& obj) {
    obj.serialize(j);
}

void from_json(const nlohmann::json& j, Serializable& obj) {
    obj.deserialize(j);
}