#ifndef ACCOUNT_UPSERT_H
#define ACCOUNT_UPSERT_H

#include <vector>

#include "account/account.h"
#include "graphics/nwindow.h"

class AccountUpsert : public NWindow {
  typedef std::function<void(std::string username, std::string password,
                             bool newAccount)>
      auth_handler_t;
  auth_handler_t authHandler;

  int tabIndex = 0;
  std::string username;
  std::string password;
  bool newAccount = false;

  void selectNextItem();
  void selectPreviousItem();

 public:
  AccountUpsert(auth_handler_t authHandler, WindowColor color = WindowColor{})
      : NWindow(10, color), authHandler(authHandler) {}

  void onDraw(bool isActive) const;
  bool onInput(int key);
  void onResize();
};

#endif