#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "../events/event.h"

class Observer {
public:
    virtual void notify(Event) = 0;
    virtual ~Observer();
};

#endif