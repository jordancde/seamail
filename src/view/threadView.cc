#include "./threadView.h"
#include "./composer.h"
#include "events/accountEvent.h"

#include <ctime>
#include <sstream>
#include <string>

using namespace std;

string ThreadView::getDisplayString(const Email& email) const {
    string displayString = "";
    displayString += (email.read) ? "  " : "⬤ ";
    displayString += email.dateTime;
    displayString += email.from;
    return displayString;
}

void ThreadView::openEmail() {
    Composer c = Composer(emails.at(selectedEmailIndex), true);
    c.compose();
}

void ThreadView::onResize() {
    int x = maxx() / 6 + maxx() / 3;
    int y = 1;
    int w = maxx() / 2;
    int h = maxy() - 1;
    resize(w, h);
    reframe(x, y, 0, 0, w, h);
}

void ThreadView::onDraw(bool isActive) const {
    wmove(win, 1, 0);
    for (size_t eidx = 0; eidx < emails.size(); ++eidx) {
        Email e = emails.at(eidx);
        string dispName = getDisplayString(e);
        waddch(win, ' ');
        if (eidx == selectedEmailIndex) wattron(win, A_REVERSE);
        wprintw(win, "%s\n", dispName.c_str());
        wattroff(win, A_REVERSE);
    }
    wmove(win, 0, 2);
    if (isActive) wattron(win, A_REVERSE);
    box(win, 0, 0);
    wprintw(win, "Emails");
    wattroff(win, A_REVERSE);
}

void ThreadView::notify(std::shared_ptr<Event> event) {
    auto ptr = std::dynamic_pointer_cast<AccountEvent>(event);
    if (!ptr) throw std::logic_error("Pointer to wrong event type!");

    switch (ptr->type) {
        case AccountEventType::THREAD_CHANGED: {
            this->getThread();
            this->updateEmails();
            this->refresh();
            break;
        }
        case AccountEventType::ACCOUNT_FOLDERS_CHANGED: {
            // folder might get deleted
            this->getThread();
            this->updateEmails();
            this->refresh();
            break;
        }
        case AccountEventType::FOLDER_CONTENTS_CHANGED: {
            this->getThread();
            this->updateEmails();
            this->refresh();
        }
        default:
            break;
    }
}

bool ThreadView::onInput(int key) {
    size_t max = emails.size();
    bool handled = false;

    switch (key) {
        case 'k':
            if (selectedEmailIndex > 0) {
                updateSelectedEmail(selectedEmailIndex - 1);
                refresh();
            }
            handled = true;
            break;
        case 'j':
            if (selectedEmailIndex < max - 1) {
                updateSelectedEmail(selectedEmailIndex + 1);
                refresh();
            }
            handled = true;
            break;
        case '\n':
            // open email read-only
            openEmail();
            handled = true;
            break;
    }
    // catch-all failsafe
    if (selectedEmailIndex < 0 || selectedEmailIndex >= max) {
        updateSelectedEmail(0);
        refresh();
    }
    return handled;
}