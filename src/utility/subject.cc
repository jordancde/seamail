#include "subject.h"

#include <algorithm>

void Subject::notifyAllObservers() {
    for(auto o : observers){
        notifyObserver(*o);
    }
}

void attach(std::shared_ptr<Observer> obs) {
    observers.push_back(obs);
}

void detach(std::shared_ptr<Observer> obs) {
    auto it = observers.find(obs, observers.begin(), observers.end());
    if(it){
        observers.erase(it);
    }
}
