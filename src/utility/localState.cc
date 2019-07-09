#include "localState.h"

#include <algorithm>

using namespace std;

void LocalState::serialize(nlohmann::json& state) const {
    state["accounts"] = accounts;
    state["localProvider"] = *localProvider;
}

void LocalState::deserialize(const nlohmann::json& state) {
    accounts = state["accounts"].get<vector<Account>>();
    localProvider = make_shared<LocalEmailProvider>(
        state["localProvider"].get<LocalEmailProvider>());
}

void LocalState::storeAccount(Account& acc) { accounts.emplace_back(acc); }

vector<Account>& LocalState::getAccounts() { return accounts; }

void LocalState::removeAccount(Account& acc) {
    accounts.erase(find(accounts.begin(), accounts.end(), acc));
}

bool LocalState::operator==(const LocalState& other) const{
    return accounts==other.accounts && *(localProvider) == *(other.localProvider);
}