
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

int main() {
  srand(time(0));

  LocalState localState;

  ifstream stateFileIn(STATE_FILE);
  if (stateFileIn.good()) stateFileIn >> localState;


  Account activeAccount;

  Compositor& com = Compositor::instance();

  shared_ptr<NWindow> myAccountView;
  shared_ptr<NWindow> myAccountSelectWindow;
  shared_ptr<NWindow> myActiveFolderView;
  shared_ptr<NWindow> myToolbar;
  shared_ptr<NWindow> myAccountUpsertWindow;
  shared_ptr<NWindow> myActiveDialog;
  shared_ptr<NWindow> myActiveThreadView;

  auto makeDialog = [&](string title, string message, int height=7, int width=100) {
    com.bindWindow(
        myActiveDialog,
        myActiveDialog = make_shared<Dialog>(
            title, message, [&] { com.destroyWindow(myActiveDialog); }, height,width),
        true);
  };

  auto makeInputDialog = [&](string title, string message,
                             std::function<void(string)> inputHandler) {
    com.bindWindow(
        myActiveDialog,
        make_shared<InputDialog>(title, message,
                                 [&] { com.destroyWindow(myActiveDialog); },
                                 [&, inputHandler](string input) {
                                   inputHandler(input);
                                   com.destroyWindow(myActiveDialog);
                                 }),
        true);
  };

  auto logoutActiveAccount = [&](bool remove = true) {
    com.destroyWindow(myActiveFolderView);
    com.destroyWindow(myAccountView);
    com.destroyWindow(myActiveThreadView);
    if(remove)
      localState.removeAccount(activeAccount);
    activeAccount.logout();
    com.refresh();
  };

  auto changeActiveAccount = [&](Account& acc) {
    // prevent rebinding of active account
    logoutActiveAccount(false);

    activeAccount = acc;

    com.bindWindow(
        myAccountView,
        make_shared<AccountView>(activeAccount, [&](string folderPath) {
          com.bindWindow(
              myActiveFolderView,
              make_shared<FolderView>(
                  activeAccount, folderPath,
                  [&](string threadId) {
                    com.bindWindow(myActiveThreadView,
                                   make_shared<ThreadView>(
                                       activeAccount, threadId,
                                       [&](Email e) {
                                         com.runExternalProgram([&, e] {
                                           Composer c = Composer(e, true);
                                           c.compose();
                                         });
                                       },
                                       [&](Email e) {
                                         com.runExternalProgram([&, e] {
                                           Composer c = Composer(e);
                                           c.compose();
                                           activeAccount.sendEmail(c.toEmail());
                                         });
                                       }));
                  },
                  [&](std::string title, std::string message,
                      std::function<void(string)> inputReceivedHandler) {
                    makeInputDialog(title, message,
                                    [&, inputReceivedHandler](string input) {
                                      inputReceivedHandler(input);
                                    });
                  }));
        }));
  };

  auto registerAccount = [&](string username, string password) {
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
  };

  auto loginAccount = [&](string username, string password) {
    try {
      shared_ptr<LocalEmailProvider> provider = localState.localProvider;
      Account myExistingAccount{provider, username};
      myExistingAccount.login(username, password);
      if (find(localState.getAccounts().begin(), localState.getAccounts().end(),
               myExistingAccount) != localState.getAccounts().end()){
          makeDialog("Login Failed",
                     "You are already logged into this account.");
          return;
        }
      localState.storeAccount(myExistingAccount);
      changeActiveAccount(myExistingAccount);
    } catch (AuthenticationFailedException& e) {
      makeDialog("Login Failed", "Check your email and password.");
    }
  };

  myToolbar = make_shared<Toolbar>(
      0, list<string>{"Accounts", "Login", "Logout", "Exit", "Compose", "Help"},
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
                               }),
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
                  }),
              true);
        } else if (selected == "Logout") {
          logoutActiveAccount();
        } else if (selected == "Exit") {
          com.quit();
        } else if (selected == "Help") {
          ifstream helpFile ("help.txt"); 
          string message = "";
          string line; 
          while (getline(helpFile,line))
            message+=line+"\n  ";
          makeDialog("Help", message,23,100);
        } else if (selected == "Compose") {
          if(activeAccount.loggedIn){
            activeAccount.sendEmail(
                com.runExternalProgram<Email>([&] {
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
      -2);

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
