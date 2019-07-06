#include "localState.h"

#include <algorithm>

using namespace std;

LocalState::LocalState() {}

LocalState::~LocalState() {}

void LocalState::serialize(nlohmann::json& state) const {
    state["accounts"] = accounts;
    state["providers"] = providers;
}

void LocalState::deserialize(const nlohmann::json& state) {
    accounts = state["accounts"].get<vector<Account>>();
    // TODO
    // providers = state["providers"].get<vector<EmailProvider>>();
}

void LocalState::storeAccount(Account acc) {
    accounts.push_back(acc);
}

vector<Account>& LocalState::getAccounts() {
    return accounts;
}

vector<EmailProvider>& LocalState::getProviders() {
    return providers;
}

void LocalState::removeAccount(Account& acc) {
    accounts.erase(find(accounts.begin(), accounts.end(), acc));
}
