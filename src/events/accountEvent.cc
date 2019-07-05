#include "accountEvent.h"

using namespace std;

AccountEventType AccountEvent::getType() { 
    return type; 
}

std::string AccountEvent::getAffectedId() { 
    return affectedId; 
}

Account& AccountEvent::getAffectedAccount() { 
    return affectedAccount; 
}

AccountEvent::AccountEvent(AccountEventType type, Account& acc, std::string id):
    type{type}, affectedAccount{acc}, affectedId{id}{}

AccountEvent::~AccountEvent(){}
