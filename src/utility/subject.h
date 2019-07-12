#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include "observer.h"
#include <vector>
#include <memory>

class Subject {
    std::vector<Observer*> observers;

public:
    Subject() = default;
    
    void attach(Observer*);
    void detach(Observer*);

    void notifyAllObservers(std::shared_ptr<Event>);

    virtual ~Subject() = 0;
};

#endif
