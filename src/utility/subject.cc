#include "subject.h"

#include <algorithm>

void Subject::notifyAllObservers() {
    for(auto o : observers){
        notifyObserver(*o);
    }
}

void Subject::attach(std::shared_ptr<Observer> obs) {
    observers.push_back(obs);
}

void Subject::detach(std::shared_ptr<Observer> obs) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == obs) {
            observers.erase(it);
            break;
        }
    }
}
