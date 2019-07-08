#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

#include <iostream>

#include <nlohmann/json.hpp>

class Serializable {
protected:
    virtual void serialize(nlohmann::json&) const = 0;
    virtual void deserialize(const nlohmann::json&) = 0;

public:
    virtual ~Serializable() = default;
    friend std::ostream& operator<<(std::ostream&, const Serializable&);
    friend std::istream& operator>>(std::istream&, Serializable&);
    friend void to_json(nlohmann::json&, const Serializable&);
    friend void from_json(const nlohmann::json&, Serializable&);
};

#endif
