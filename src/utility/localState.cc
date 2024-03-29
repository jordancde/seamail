#include "localState.h"

#include <algorithm>

using namespace std;

void LocalState::serialize(nlohmann::json& state) const {
    state["accounts"] = accounts;
    state["localProvider"] = *localProvider;
    state["colors"] = colors;
}

void LocalState::deserialize(const nlohmann::json& state) {
    accounts = state["accounts"].get<vector<Account>>();
    colors = state["colors"].get<map<string, WindowColor>>();
    localProvider = make_shared<LocalEmailProvider>(
        state["localProvider"].get<LocalEmailProvider>());

    for (auto& acc : accounts) acc.setProvider(localProvider);
}

void LocalState::storeAccount(Account& acc) { accounts.emplace_back(acc); }

vector<Account>& LocalState::getAccounts() { return accounts; }

void LocalState::removeAccount(Account& acc) {
    auto it = find(accounts.begin(), accounts.end(), acc);
    if(it != accounts.end())
        accounts.erase(it);
}

bool LocalState::operator==(const LocalState& other) const {
    return accounts == other.accounts &&
           *(localProvider) == *(other.localProvider);
}