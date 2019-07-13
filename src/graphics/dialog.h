
#ifndef DIALOG_H
#define DIALOG_H

#include <vector>

#include "graphics/nwindow.h"
#include "account/account.h"

class Dialog : public NWindow {
protected:
    std::string title;
    std::string message;

    std::function<void(void)> dialogClosedHandler;

public:
    Dialog(std::string title, std::string message, std::function<void(void)> dialogClosedHandler) 
        : NWindow(100), title(title), message(message), dialogClosedHandler(dialogClosedHandler) {
    } 
    
    virtual void onDraw(bool isActive) const; 
    virtual bool onInput(int key); 
    void onResize(); 
};

#endif