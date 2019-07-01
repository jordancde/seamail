#include "accountEvent.h"


AccountEventType AccountEvent::getType(){ return type; }
string AccountEvent::getAffectedId(){ return id; }
Account& AccountEvent::getAffectedAccount(){ return affectedAccount; }

AccountEvent::AccountEvent(AccountEventType type, Account& acc, std::string id):
    type{type}, affectedId{affectedId}, affectedAccount{acc}{}

AccountEvent::~AccountEvent(){}
