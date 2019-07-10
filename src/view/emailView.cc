#include "./emailView.h"

void EmailView::onResize() {
    // I'll implement this   
}

void EmailView::onDraw(bool isActive) const {
    // I'll implement this
}

void EmailView::notify(std::shared_ptr<Event> event) {
    // call refresh() on email change event
}

bool EmailView::onInput(int key) {
    // controls for replying
    return false;
}