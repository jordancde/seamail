#ifndef LOCALSTATE_H
#define LOCALSTATE_H

#include <iostream>
#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

#include "../account/account.h"
#include "../providers/localEmailProvider.h"
#include "./serializable.h"

class LocalState : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

    std::vector<Account> accounts;

public:
    LocalEmailProvider localProvider;

    void storeAccount(Account&);
    void removeAccount(Account&);
    std::vector<Account>& getAccounts();

    LocalState() = default;
};

#endif