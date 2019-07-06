#ifndef LOCALSTATE_H
#define LOCALSTATE_H

#include <iostream>
#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

#include "../account/account.h"
#include "../providers/emailProvider.h"
#include "./serializable.h"

class LocalState : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

    std::vector<Account> accounts;
    std::vector<EmailProvider> providers;

public:
    void storeAccount(Account);
    std::vector<Account>& getAccounts();
    void removeAccount(Account&);
    std::vector<EmailProvider>& getProviders();

    LocalState();
    ~LocalState();
};

#endif