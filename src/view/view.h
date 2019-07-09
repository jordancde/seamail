
#ifndef VIEW_H_
#define VIEW_H_

#include <memory>

#include "utility/observer.h"
#include "graphics/nwindow.h"

#include "account/account.h"

class View : public Observer, public NWindow {
protected:
    std::shared_ptr<Account> account;

public:

    View(std::shared_ptr<Account> account) : account(account) {}
    virtual ~View() {}

};


#endif
