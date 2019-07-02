#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "graphics/nwindow.h"

#include <functional>
#include <string>
#include <list>

// one line thick toolbar
class Toolbar : public NWindow {

    size_t line;
    std::list<std::string> menuItems;
    std::list<std::string>::iterator selected;

    std::function<void(std::string)> handler;

public:
    Toolbar(size_t line, 
        std::list<std::string> menuItems, std::function<void(std::string)> handler);

    void onDraw(bool isActive) const override;
    void onInput(int key) override;
    void onResize() override;
};

#endif