
#include <cstdio>
#include <iostream>
#include <fstream>

#include "graphics/compositor.h"
#include "graphics/toolbar.h"
#include "graphics/dialog.h"
#include "graphics/accountSelect.h"
#include "graphics/accountUpsert.h"

#include "view/accountView.h"
#include "view/composer.h"
#include "view/folderView.h"
#include "view/threadView.h"

#include "utility/localState.h"
#include "providers/localEmailProvider.h"
#include "account/account.h"
#include "exceptions/authenticationFailedException.h"

using namespace std;

const string STATE_FILE = "state.json";

int main()
{	
	LocalState localState;

	ifstream stateFileIn(STATE_FILE);
	if(stateFileIn.good())
		stateFileIn >> localState;

	Account activeAccount;

	Compositor& com = Compositor::instance();

	shared_ptr<NWindow> myAccountView;
	shared_ptr<NWindow> myAccountSelectWindow;
	shared_ptr<NWindow> myActiveFolderView;
	shared_ptr<NWindow> myToolbar;
	shared_ptr<NWindow> myAccountUpsertWindow;
	shared_ptr<NWindow> myActiveDialog;
	shared_ptr<NWindow> myAccountToolbar;
	shared_ptr<NWindow> myActiveThreadView;

	auto bindWindow = [&](shared_ptr<NWindow>& window, shared_ptr<NWindow> instance,
							bool setActive = false){
		if(window)
			com.removeWindow(window);
		window = instance;
		com.addWindow(window);
		if(setActive)
			com.setActiveWindow(window);
		com.refresh();
	};
	auto destroyWindow = [&](shared_ptr<NWindow> window){
		if(window){
			com.removeWindow(window);
			com.refresh();
		}
	};

	auto makeDialog = [&](string title, string message){
		bindWindow(myActiveDialog, 
			myActiveDialog = make_shared<Dialog>(title, message, [&]{
				destroyWindow(myActiveDialog);
			}), true);
	};

	auto logoutActiveAccount = [&]{
		destroyWindow(myActiveFolderView);
		destroyWindow(myAccountView);
		destroyWindow(myAccountToolbar);
		localState.removeAccount(activeAccount);
		com.refresh();
	};

	auto changeActiveAccount = [&](Account& acc){
		activeAccount = acc;

		bindWindow(myAccountView, make_shared<AccountView>(activeAccount, [&](string folderPath){
			bindWindow(myActiveFolderView,
				make_shared<FolderView>(activeAccount, folderPath, 
					[&](string threadId){
						bindWindow(myActiveThreadView, 
							make_shared<ThreadView>(activeAccount, threadId,
								[&](Email e){
									com.runExternalProgram([&]{
										Composer c = Composer(e, true);
										c.compose();
									});
								}));
					}));
		}));

		bindWindow(myAccountToolbar, make_shared<Toolbar>(1, 
			list<string>{activeAccount.getEmailAddress(), "Compose"}, 
			[&](string selected){
				if(selected == "Compose"){
					activeAccount.sendEmail(
						com.runExternalProgram<Email>([&]{
							Email e;
							e.from = activeAccount.getEmailAddress();
							Composer c{e};
							c.compose();
							return c.toEmail();
						}));
				}

			}));
	};

	auto registerAccount = [&](string username, string password){
		// TEMP
		shared_ptr<LocalEmailProvider> provider = localState.localProvider;
		Account myNewAccount{provider, username};
		provider->addAccount(username, password);
		myNewAccount.login(username, password);
		localState.storeAccount(myNewAccount);
		changeActiveAccount(myNewAccount);
	};

	auto loginAccount = [&](string username, string password){
		try{
			shared_ptr<LocalEmailProvider> provider = localState.localProvider;
			Account myExistingAccount{provider, username};
			myExistingAccount.login(username, password);
			localState.storeAccount(myExistingAccount);
			changeActiveAccount(myExistingAccount);
		}catch(AuthenticationFailedException &e){
			makeDialog("Login Failed", "Check your email and password.");
		}
	};

	myToolbar = make_shared<Toolbar>(0, 
		list<string>{"Accounts", "Login", "Logout", "Exit"}, [&](string selected){
			if(selected == "Accounts"){
				if(localState.getAccounts().size() < 1){
					makeDialog("Error", "You are not logged into any accounts.");
				}else {
					bindWindow(myAccountSelectWindow, make_shared<AccountSelect>(localState.getAccounts(), 
						[&](Account& selectedAccount){
							changeActiveAccount(selectedAccount);
							destroyWindow(myAccountSelectWindow);
						}), true);
				}
			} else if(selected == "Login") {
				bindWindow(myAccountUpsertWindow, make_shared<AccountUpsert>(
					[&](string username, string password, bool newAccount){
						if(username != "" && password != ""){
							if(newAccount) registerAccount(username, password);
							else loginAccount(username, password);
						}
						destroyWindow(myAccountUpsertWindow);
					}), true);
			} else if(selected == "Logout") {
				logoutActiveAccount();
			}else if (selected == "Exit"){
				com.quit();
			}else {
				throw exception{};
			}
	});

	com.addWindow(myToolbar);

	com.setActiveWindow(myToolbar);

	com.refresh();
	com.run();

	destroyWindow(myAccountView);
	destroyWindow(myActiveFolderView);
	destroyWindow(myAccountToolbar);
	destroyWindow(myActiveThreadView);

	ofstream stateFileOut(STATE_FILE);
	stateFileOut << localState;

	return 0;
}
