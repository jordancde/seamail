
#ifndef VIEW_H_
#define VIEW_H_

#include <memory>

#include "utility/observer.h"
#include "graphics/nwindow.h"

#include "account/account.h"

class View : public Observer, public NWindow {
protected:
    Account& account;

public:

    View(Account& account, int zindex = 0) : NWindow(zindex), account(account) {
        account.attach(this);
    }
    virtual ~View() {
        account.detach(this);
    }

};


#endif
