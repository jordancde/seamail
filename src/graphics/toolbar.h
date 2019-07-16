#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "graphics/nwindow.h"

#include <functional>
#include <list>
#include <string>

// one line thick toolbar
class Toolbar : public NWindow {
  size_t line;
  std::list<std::string> menuItems;
  std::list<std::string>::iterator selected;

  std::function<void(std::string)> handler;

 public:
  Toolbar(size_t line, std::list<std::string> menuItems,
          std::function<void(std::string)> handler, int zindex = 0,
          WindowColor color = WindowColor{})
      : NWindow(zindex, color),
        line(line),
        menuItems(std::move(menuItems)),
        handler(handler) {
    selected = this->menuItems.begin();
  }

  void onDraw(bool isActive) const override;
  bool onInput(int key) override;
  void onResize() override;
};

#endif