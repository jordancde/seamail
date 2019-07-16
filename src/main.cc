
#include <cstdio>
#include <fstream>
#include <iostream>

#include "graphics/accountSelect.h"
#include "graphics/accountUpsert.h"
#include "graphics/compositor.h"
#include "graphics/dialog.h"
#include "graphics/inputDialog.h"
#include "graphics/toolbar.h"

#include "view/accountView.h"
#include "view/composer.h"
#include "view/folderView.h"
#include "view/threadView.h"

#include "account/account.h"
#include "exceptions/accountAlreadyExistsException.h"
#include "exceptions/authenticationFailedException.h"
#include "providers/localEmailProvider.h"
#include "utility/localState.h"

using namespace std;

const string STATE_FILE = "state.json";

Compositor& com = Compositor::instance();

shared_ptr<NWindow> myAccountView;
shared_ptr<NWindow> myAccountSelectWindow;
shared_ptr<NWindow> myActiveFolderView;
shared_ptr<NWindow> myToolbar;
shared_ptr<NWindow> myAccountUpsertWindow;
shared_ptr<NWindow> myActiveDialog;
shared_ptr<NWindow> myActiveInputDialog;
shared_ptr<NWindow> myActiveThreadView;

LocalState localState;

Account activeAccount;

// Creates a dialog, automatically binding it to the Compositor
void makeDialog(string title, string message, int height = 7, int width = 100) {
  com.bindWindow(myActiveDialog,
                 myActiveDialog = make_shared<Dialog>(
                     title, message, [&] { com.destroyWindow(myActiveDialog); },
                     height, width, localState.colors["dialog"]),
                 true);
}

// Creates an input dialog, automatically binding it to the Compositor
void makeInputDialog(string title, string message,
                     std::function<void(string)> inputHandler) {
  com.bindWindow(
      myActiveInputDialog,
      make_shared<InputDialog>(title, message,
                               [&] { com.destroyWindow(myActiveInputDialog); },
                               [&, inputHandler](string input) {
                                 inputHandler(input);
                                 com.destroyWindow(myActiveInputDialog);
                               },
                               localState.colors["input_dialog"]),
      true);
}

void logoutActiveAccount(bool remove = true) {
  com.destroyWindow(myActiveFolderView);
  com.destroyWindow(myAccountView);
  com.destroyWindow(myActiveThreadView);
  if (remove) localState.removeAccount(activeAccount);
  activeAccount.logout();
}

void displayEmailHandler(Email e) {
  com.runExternalProgram([&, e] {
    Composer c = Composer(e, true);
    c.compose();
  });
}

void replyEmailHandler(Email e) {
  com.runExternalProgram([&, e] {
    Composer c = Composer(e);
    c.compose();
    activeAccount.sendEmail(c.toEmail());
  });
}

void selectedThreadChangedHandler(string threadId) {
  com.bindWindow(myActiveThreadView,
                 make_shared<ThreadView>(activeAccount, threadId,
                                         displayEmailHandler, replyEmailHandler,
                                         localState.colors["thread_view"]));
}

void selectedFolderChangedHandler(string folderPath) {
  com.destroyWindow(myActiveThreadView);
  com.bindWindow(myActiveFolderView,
                 make_shared<FolderView>(
                     activeAccount, folderPath, selectedThreadChangedHandler,
                     [&](std::string title, std::string message,
                         std::function<void(string)> inputReceivedHandler) {
                       makeInputDialog(title, message,
                                       [&, inputReceivedHandler](string input) {
                                         inputReceivedHandler(input);
                                       });
                     },
                     localState.colors["folder_view"]));
}

void changeActiveAccount(Account& acc) {
  // prevent rebinding of active account
  logoutActiveAccount(false);

  activeAccount = acc;
  activeAccount.loggedIn = true;

  com.bindWindow(myAccountView, make_shared<AccountView>(
                                    activeAccount, selectedFolderChangedHandler,
                                    localState.colors["account_view"]));
};

void registerAccount(string username, string password) {
  try {
    shared_ptr<LocalEmailProvider> provider = localState.localProvider;
    Account myNewAccount{provider, username};
    provider->addAccount(username, password);
    myNewAccount.login(username, password);
    localState.storeAccount(myNewAccount);
    changeActiveAccount(myNewAccount);
  } catch (AccountAlreadyExists& e) {
    makeDialog("Registration Failed",
               "The account with that email already exists.");
  }
}
void loginAccount(string username, string password) {
  try {
    shared_ptr<LocalEmailProvider> provider = localState.localProvider;
    Account myExistingAccount{provider, username};
    myExistingAccount.login(username, password);
    if (find(localState.getAccounts().begin(), localState.getAccounts().end(),
             myExistingAccount) != localState.getAccounts().end()) {
      makeDialog("Login Failed", "You are already logged into this account.");
      return;
    }
    localState.storeAccount(myExistingAccount);
    changeActiveAccount(myExistingAccount);
  } catch (AuthenticationFailedException& e) {
    makeDialog("Login Failed", "Check your email and password.");
  }
}

int main() {
  srand(time(0));

  ifstream stateFileIn(STATE_FILE);
  if (stateFileIn.good()) stateFileIn >> localState;

  myToolbar = make_shared<Toolbar>(
      0,
      list<string>{"Accounts", "Login", "Logout", "Exit", "Compose", "Theme",
                   "Help"},
      [&](string selected) {
        if (selected == "Accounts") {
          if (localState.getAccounts().size() < 1) {
            makeDialog("Error", "You are not logged into any accounts.");
          } else {
            com.bindWindow(myAccountSelectWindow,
                           make_shared<AccountSelect>(
                               localState.getAccounts(),
                               [&](Account& selectedAccount) {
                                 changeActiveAccount(selectedAccount);
                                 com.destroyWindow(myAccountSelectWindow);
                               },localState.colors["input_dialog"]),
                           true);
          }
        } else if (selected == "Login") {
          com.bindWindow(
              myAccountUpsertWindow,
              make_shared<AccountUpsert>(
                  [&](string username, string password, bool newAccount) {
                    if (username != "" && password != "") {
                      if (newAccount)
                        registerAccount(username, password);
                      else
                        loginAccount(username, password);
                    }
                    com.destroyWindow(myAccountUpsertWindow);
                  },localState.colors["input_dialog"]),
              true);
        } else if (selected == "Logout") {
          logoutActiveAccount();
        } else if (selected == "Exit") {
          com.quit();
        } else if (selected == "Help") {
          ifstream helpFile("help.txt");
          string message = "";
          string line;
          while (getline(helpFile, line)) message += line + "\n  ";
          makeDialog("Help", message, 23, 100);
        } else if (selected == "Theme") {
          makeInputDialog("Themes", "Select theme (plain/ocean/spicy)",
                          [&](std::string theme) {
                            auto& color = localState.colors;
                            if (theme == "ocean") {
                              color["dialog"] = {COLOR_WHITE,COLOR_CYAN};
                              color["input_dialog"] = {COLOR_WHITE,COLOR_CYAN};
                              color["toolbar"] = {COLOR_WHITE,COLOR_BLUE};
                              color["account_view"] = {COLOR_WHITE,COLOR_BLUE};
                              color["folder_view"] = {COLOR_WHITE,COLOR_BLUE};
                              color["thread_view"] = {COLOR_WHITE,COLOR_BLUE};
                            } else if (theme == "spicy") {
                              color["dialog"] = {COLOR_WHITE,COLOR_MAGENTA};
                              color["input_dialog"] = {COLOR_WHITE,COLOR_MAGENTA};
                              color["toolbar"] = {COLOR_WHITE,COLOR_RED};
                              color["account_view"] = {COLOR_WHITE,COLOR_RED};
                              color["folder_view"] = {COLOR_WHITE,COLOR_RED};
                              color["thread_view"] = {COLOR_WHITE,COLOR_RED};
                            } else {
                              color["dialog"] = WindowColor{};
                              color["input_dialog"] = WindowColor{}; 
                              color["toolbar"] = WindowColor{};
                              color["account_view"] = WindowColor{};
                              color["folder_view"] = WindowColor{};
                              color["thread_view"] = WindowColor{};
                            }
                            makeDialog("Restart",
                                       "Restart the program to apply changes.");
                          });
        } else if (selected == "Compose") {
          if (activeAccount.loggedIn) {
            activeAccount.sendEmail(com.runExternalProgram<Email>([&] {
              Email e;
              e.from = activeAccount.getEmailAddress();
              Composer c{e};
              c.compose();
              return c.toEmail();
            }));
          }
        } else {
          throw exception{};
        }
      },
      -2, localState.colors["toolbar"]);

  com.addWindow(myToolbar);

  com.setActiveWindow(myToolbar);

  com.refresh();
  com.run();

  com.destroyWindow(myAccountView);
  com.destroyWindow(myActiveFolderView);
  com.destroyWindow(myActiveThreadView);

  ofstream stateFileOut(STATE_FILE);
  stateFileOut << localState;

  return 0;
}
