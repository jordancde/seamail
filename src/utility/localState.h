#ifndef LOCALSTATE_H
#define LOCALSTATE_H

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <nlohmann/json.hpp>

#include "../account/account.h"
#include "../providers/localEmailProvider.h"
#include "./serializable.h"
#include "graphics/nwindow.h"

class LocalState : public Serializable {
    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

    std::vector<Account> accounts;  // MAYBE public?

   public:
    std::map<std::string, WindowColor> colors;
    std::shared_ptr<LocalEmailProvider> localProvider;

    void storeAccount(Account&);
    void removeAccount(Account&);
    std::vector<Account>& getAccounts();

    LocalState() : localProvider(std::make_shared<LocalEmailProvider>()) {}
    bool operator==(const LocalState&) const;
};

#endif