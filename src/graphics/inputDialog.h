
#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H

#include <string>
#include <vector>

#include "dialog.h"

class InputDialog : public Dialog {
    std::string input;
    int tabIndex;
    void selectNextItem();
    void selectPreviousItem();
    std::function<void(std::string)> dialogSubmitHandler;

   public:
    InputDialog(std::string title, std::string message,
                std::function<void(void)> dialogClosedHandler,
                std::function<void(std::string)> dialogSubmitHandler,
                WindowColor color = WindowColor{})
        : Dialog(title, message, dialogClosedHandler, 7, 100, color),
          input{""},
          tabIndex{0},
          dialogSubmitHandler{dialogSubmitHandler} {}

    void onDraw(bool isActive) const override;
    bool onInput(int key) override;
    std::string getInput() { return input; }
};

#endif