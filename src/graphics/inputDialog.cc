#include "./inputDialog.h"

#include <string>

using namespace std;

bool InputDialog::onInput(int key) {
    switch (key) {
        case '\t':
        case KEY_DOWN:
            selectNextItem();
            break;
        case KEY_UP:
            selectPreviousItem();
            break;
        default:
            switch (tabIndex) {
                case 0:
                    switch (key) {
                        case '\n':
                            selectNextItem();
                            break;
                        case 127:
                        case KEY_BACKSPACE:
                            if (input.length() > 0)
                                input = input.substr(0, input.length() - 1);
                            break;
                        default:
                            char c = (char)key;
                            if (isprint(c)) input += c;
                            break;
                    }
                    refresh();
                    break;
                case 1:
                    if (key == '\n') {
                        dialogClosedHandler();
                    }
                    break;

                case 2:
                    if (key == '\n') {
                        dialogSubmitHandler(input);
                        dialogClosedHandler();
                    }
                    break;
            }
            break;
    }
    return true;
}

void InputDialog::selectNextItem() {
    tabIndex = (tabIndex + 1) % 3;
    refresh();
}

void InputDialog::selectPreviousItem() {
    tabIndex = (tabIndex + 3 - 1) % 3;
    refresh();
}

void InputDialog::onDraw(bool isActive) const {
    werase(win);
    wmove(win, 1, 2);

    if (tabIndex == 0) wattron(win, A_REVERSE);
    if (input.empty()) {
        mvwprintw(win, cy() + 1, 2, " ");
    } else {
        mvwprintw(win, cy() + 1, 2, "%s", input.c_str());
    }
    wattroff(win, A_REVERSE);

    wmove(win, cy() + 2, 0);
    if (tabIndex == 1) wattron(win, A_REVERSE);
    mvwprintw(win, cy(), w() / 2 - 12, "%s", " [ Cancel ] ");
    wattroff(win, A_REVERSE);

    if (tabIndex == 2) wattron(win, A_REVERSE);
    wprintw(win, "%s", " [ OK ] ");
    wattroff(win, A_REVERSE);

    wmove(win, 0, 1);

    if (isActive) wattron(win, A_REVERSE);
    box(win, 0, 0);
    wmove(win, 0, 2);
    wprintw(win, title.c_str());
    wattroff(win, A_REVERSE);
}
