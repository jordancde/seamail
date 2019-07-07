#include "localState.h"

#include <algorithm>

using namespace std;

LocalState::LocalState() {}

LocalState::~LocalState() {}

void LocalState::serialize(nlohmann::json& state) const {
    state["accounts"] = accounts;
    state["localProvider"] = localProvider;
}

void LocalState::deserialize(const nlohmann::json& state) {
    accounts = state["accounts"].get<vector<Account>>();
    localProvider = state["localProvider"].get<LocalEmailProvider>();
}

void LocalState::storeAccount(Account acc) {
    accounts.push_back(acc);
}

vector<Account>& LocalState::getAccounts() {
    return accounts;
}

void LocalState::removeAccount(Account& acc) {
    accounts.erase(find(accounts.begin(), accounts.end(), acc));
}
