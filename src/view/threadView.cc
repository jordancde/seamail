#include "./threadView.h"
#include "./composer.h"
#include "events/accountEvent.h"

#include <string>
using namespace std;

string ThreadView::getDisplayString(const Email& email) {
    string displayString = "";
    displayString += (email.read) ? "  " : "⬤ ";
    displayString += email.subject;
    return displayString;
}

void ThreadView::openEmail() {
    Composer c = Composer(emails.at(selectedEmailIndex), true);
    c.compose();
}

void ThreadView::onResize() {
    // I'll implement this
}

void ThreadView::onDraw(bool isActive) const {
    // I'll implement this
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
        case 'o':
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