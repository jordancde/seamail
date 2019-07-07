#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include "observer.h"
#include <vector>
#include <memory>

class Subject {
    std::vector<std::shared_ptr<Observer>> observers;

public:
    Subject() = default;
    
    void attach(std::shared_ptr<Observer>);
    void detach(std::shared_ptr<Observer>);

    void notifyAllObservers(std::shared_ptr<Event>);

    virtual ~Subject() = 0;
};

#endif
