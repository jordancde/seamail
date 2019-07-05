#include "localState.h"
#include "../account/account.h"
#include "../providers/emailProvider.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <sstream>


using namespace std;

ostream& LocalState::serialize(ostream& out) const{
    nlohmann::json localState;

    vector<Account> accs;

    for(auto acc: accounts){
        accs.push_back(*acc);
    }


    //localState["accounts"] = accs;
    //localState["providers"] = provs;

    return out << localState;
}

istream& LocalState::deserialize(istream& in){
    nlohmann::json localState;
    in >> localState;

    vector<Account> tempAccs;// = localState["accounts"];
    
    accounts.clear();

    for(auto acc : tempAccs){
        accounts.push_back(make_shared<Account>(acc));
    }

    return in;
}

void LocalState::storeAccount(shared_ptr<Account> acc){
    accounts.push_back(acc);
}

vector<shared_ptr<Account>> LocalState::getAccounts(){
    return accounts;
}

void LocalState::removeAccount(Account& acc){
    accounts.erase(remove(accounts.begin(), accounts.end(), acc), accounts.end());
}

LocalState::LocalState(vector<shared_ptr<Account>> accounts, vector<shared_ptr<EmailProvider>> providers):accounts{accounts}, providers{providers}{}

LocalState::LocalState(){}

LocalState::~LocalState(){}