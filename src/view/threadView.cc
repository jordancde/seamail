#include "./threadView.h"
#include "./composer.h"
#include "events/accountEvent.h"

#include <ctime>
#include <sstream>
#include <string>

using namespace std;

void ThreadView::openAttachments() {
    for (string path : emails.at(selectedEmailIndex).imagePaths) {
        system(("display " + path).c_str());
    }
}

string ThreadView::getDisplayString(const Email& email) const {
    string displayString = "";
    // displayString += (email.read) ? "  " : ACS_DIAMOND + " ";
    displayString += "" + email.dateTime;
    displayString += email.from;
    return displayString;
}

void ThreadView::openEmail() {
    Email& e = emails.at(selectedEmailIndex);

    displayEmailHandler(e);
    account.setEmailStatus(e.id, true);
}

void ThreadView::replyEmail() {
    Email& selected = emails.at(selectedEmailIndex);
    Email newEmail = Email();
    newEmail.threadId = selected.threadId;
    newEmail.from = account.getEmailAddress();
    newEmail.subject = selected.subject;
    newEmail.to = vector<string>{selected.from};
    newEmail.cc = selected.cc;
    replyEmailHandler(newEmail);
}

void ThreadView::onResize() {
    int x = maxx() / 6 + maxx() / 3;
    int y = 1;
    int w = maxx() / 2;
    int h = maxy() - y;
    resize(w, h);
    reframe(x, y, 0, 0, w, h);
}

void ThreadView::onDraw(bool isActive) const {
    werase(win);
    wmove(win, 1, 0);
    mvwhline(win, cy(), 0, ACS_HLINE, w());
    for (size_t eidx = 0; eidx < emails.size(); ++eidx) {
        Email e = emails.at(eidx);

        struct tm* timeinfo = localtime(&e.dateTime);
        const char* desctime = asctime(timeinfo);

        if (eidx == selectedEmailIndex) {
            wattron(win, A_REVERSE);
            mvwvline(win, cy()+1, 2, ' ', 3);
            wattroff(win, A_REVERSE);
        }else{
            wmove(win,cy()+1,cx());
        }

        mvwprintw(win, cy(), 4, "%s", e.from.c_str());
        if (!e.read) wattron(win, A_REVERSE);
        mvwprintw(win, cy() + 1, 4, "%s", e.subject.c_str());
        wattroff(win, A_REVERSE);
        mvwprintw(win, cy(), w() - strlen(desctime) - 1, "%s", desctime);

        mvwhline(win, cy(), 0, ACS_HLINE, w());
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
            this->updateEmails();
            this->refresh();
            break;
        }
        case AccountEventType::ACCOUNT_FOLDERS_CHANGED: {
            this->updateEmails();
            this->refresh();
            break;
        }
        case AccountEventType::FOLDER_CONTENTS_CHANGED: {
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
        case KEY_UP:
            if (selectedEmailIndex > 0) {
                updateSelectedEmail(selectedEmailIndex - 1);
                refresh();
            }
            handled = true;
            break;
        case 'j':
        case KEY_DOWN:
            if (selectedEmailIndex < max - 1) {
                updateSelectedEmail(selectedEmailIndex + 1);
                refresh();
            }
            handled = true;
            break;
        case 'i':
            // opens all attachments
            openAttachments();
            handled = true;
            break;
        case 'r':
            replyEmail();
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