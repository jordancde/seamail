#include "accountEvent.h"

using namespace std;

AccountEvent::AccountEvent(AccountEventType type, Account& acc, std::string id):
    type{type}, affectedAccount{acc}, affectedId{id}{}
