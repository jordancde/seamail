#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Event;

class Observer {
public:
    virtual void notify(Event) = 0;
    virtual ~Observer() = default;
};

#endif
