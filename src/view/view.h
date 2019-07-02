
#ifndef VIEW_H_
#define VIEW_H_

#include <memory>

#include <ncurses.h>

#include <utility/observer.h>
#include <graphics/nwindow.h>

class View : public Observer, public NWindow {
protected:
    //std::shared_ptr<Account> account;

public:

    View();
    virtual ~View();

};


#endif
