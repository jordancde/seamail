
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
  Email getEmail(std::string emailId) { return account.getEmailById(emailId); }
  void replyEmail();
  void openEmail();
  void openAttachments();
  std::string getDisplayString(const Email& email) const;

  std::function<void(Email)> displayEmailHandler;
  std::function<void(Email)> replyEmailHandler;

 public:
  ThreadView(Account& account, std::string threadId,
             std::function<void(Email)> displayEmailHandler = [](Email) {},
             std::function<void(Email)> replyEmailHandler = [](Email) {},
             WindowColor color = WindowColor{})
      : View(account, 3, color),
        threadId(threadId),
        displayEmailHandler(displayEmailHandler),
        replyEmailHandler(replyEmailHandler) {
    updateEmails();
    updateSelectedEmail(0);
  }

  void onResize() override;
  void onDraw(bool isActive) const override;
  void notify(std::shared_ptr<Event> event) override;
  bool onInput(int key) override;
};

#endif