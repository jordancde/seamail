
#ifndef DIALOG_H
#define DIALOG_H

#include <vector>

#include "account/account.h"
#include "graphics/nwindow.h"

class Dialog : public NWindow {
 protected:
  std::string title;
  std::string message;
  int dialogH;
  int dialogW;

  std::function<void(void)> dialogClosedHandler;

 public:
  Dialog(std::string title, std::string message,
         std::function<void(void)> dialogClosedHandler, int height = 7,
         int width = 100, WindowColor color = WindowColor{})
      : NWindow(100, color),
        title(title),
        message(message),
        dialogH(height),
        dialogW(width),
        dialogClosedHandler(dialogClosedHandler) {}

  virtual void onDraw(bool isActive) const;
  virtual bool onInput(int key);
  void onResize();
};

#endif