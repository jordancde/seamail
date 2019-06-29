#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include "observer.h"
#include <vector>
#include <memory>

class Subject {
    std::vector<shared_ptr<Observer>> observers;

public:
    Subject();
    void attach(shared_ptr<Observer>);
    void detach(shared_ptr<Observer>);
    void notifyObservers();
    virtual Subject()=0;
};

#endif