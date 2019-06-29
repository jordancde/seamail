
#ifndef VIEW_H_
#define VIEW_H_

#include <utility/observer.h>
#include <memory>

class View : Observer {
protected:
    std::shared_ptr<Account> account;

public:
    void draw(/* */);
};


#endif
