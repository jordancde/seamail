#include "subject.h"
#include <memory>
#include <algorithm>

Subject::~Subject() {}

void Subject::notifyAllObservers(std::shared_ptr<Event> event) {
    for(auto o : observers){
        o->notify(event);
    }
}

void Subject::attach(Observer* obs) {
    observers.push_back(obs);
}

void Subject::detach(Observer* obs) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == obs) {
            observers.erase(it);
            break;
        }
    }
}
