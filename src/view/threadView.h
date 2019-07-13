
#ifndef THREAD_VIEW_H
#define THREAD_VIEW_H

#include "models/email.h"
#include "view/view.h"

class ThreadView : public View {
    std::string threadId;

    size_t selectedEmailIndex = SIZE_MAX;
    void updateSelectedEmail(size_t idx) { selectedEmailIndex = idx; }

    std::vector<Email> emails;

    void updateEmails() {
        std::vector<std::string> emailIds = getThread().emailIds;
        emails.clear();
        for (auto id : emailIds) emails.push_back(getEmail(id));
    }

    Thread getThread() { return account.getThreadById(threadId); }
    Email getEmail(std::string emailId) {
        return account.getEmailById(emailId);
    }

    void openEmail();
    void openAttachments();
    std::string getDisplayString(const Email& email) const;


    std::function<void(const Email&)> displayEmailHandler;

   public:
    ThreadView(Account& account, std::string threadId, 
    std::function<void(const Email&)> displayEmailHandler = [](Email){})
        : View(account, 3), threadId(threadId), displayEmailHandler(displayEmailHandler) {
            updateEmails();
            updateSelectedEmail(0);
        }

    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    bool onInput(int key) override;
};

#endif