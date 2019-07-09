
#ifndef EMAIL_VIEW_H
#define EMAIL_VIEW_H

#include "view/view.h"

class EmailView : public View {
    std::string emailId;

public:
    EmailView(std::shared_ptr<Account> account,
                std::string emailId)
                : View(account), emailId(emailId) {}

    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    void onInput(int key) override;
};

#endif