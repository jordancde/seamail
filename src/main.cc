
#include <cstdio>
#include <iostream>
#include <fstream>

#include "graphics/compositor.h"
#include "graphics/toolbar.h"
#include "graphics/dialog.h"
#include "graphics/accountSelect.h"
#include "graphics/accountUpsert.h"

#include "view/accountView.h"
#include "view/folderView.h"

#include "utility/localState.h"
#include "providers/localEmailProvider.h"
#include "account/account.h"
#include "exceptions/authenticationFailedException.h"

using namespace std;

class MyDummyWindow : public NWindow {
public:
	MyDummyWindow() {
		onResize();
	}

	void onDraw(bool isActive) const override {
		if(isActive)
			wattron(win, A_REVERSE);
		box(win, 0, 0);
		wattroff(win, A_REVERSE);
		mvwprintw(win, 1, 1, "%zu %zu", w(), maxx() * 3 / 4);
	}

	void onResize() override {
		int x = maxx() / 2;
		int y = 2;
		int w = maxx() / 2;
		int h = maxy() - y; 
		resize(w,h);
		reframe(x,y,0,0,w,h);
	}

	bool onInput(int key) override {
		switch(key){
			case KEY_UP:
			move(cx(), cy() - 1);
			break;
			case KEY_DOWN:
			move(cx(), cy() + 1);
			move(cx() - 1, cy());
			break;
			case KEY_RIGHT:
			move(cx() + 1, cy());
			break;
		}
		return false;
	}
};
const string STATE_FILE = "state.json";

int main()
{	
	LocalState localState;

	ifstream stateFileIn(STATE_FILE);
	if(stateFileIn.good())
		stateFileIn >> localState;

	
	// vector<Account> accounts;
	// string myEmailAddress = "mydummyaccount@example.com";
	// auto myLocalEmailProvider = make_shared<LocalEmailProvider>();
	// Account myDummyAccount(myLocalEmailProvider, myEmailAddress);
	// myLocalEmailProvider->addAccount(myEmailAddress, "abc123");
	// myDummyAccount.login(myEmailAddress, "abc123");
	// myDummyAccount.addFolder("inbox");
	// myDummyAccount.addFolder("sent");
	// myDummyAccount.addFolder("deleted");
	// myDummyAccount.addFolder("Test Folder 1");
	// myDummyAccount.addFolder("Test Folder 2");
	// myDummyAccount.addFolder("Test Folder 3");
	// myDummyAccount.addFolder("Test Folder 1/Nested Folder 1");
	// myDummyAccount.addFolder("Test Folder 1/Nested Folder 2");
	// myDummyAccount.addFolder("Test Folder 4");
	// myDummyAccount.addFolder("Test Folder 5");
	// myDummyAccount.addFolder("Test Folder 6");
	// myDummyAccount.addFolder("Test Folder 7");
	// accounts.push_back(myDummyAccount);
	// vector<Email> emails {
	// 	Email{
	// 		/* threadId	 	*/ string("new"),
	// 		/* from 		*/ string("test1@example.com"),
	// 		/* to			*/ vector<string>{"mydummyaccount@example.com"},
	// 		/* dateTime 	*/ (time_t) 0,
	// 		/* cc			*/ vector<string>{},
	// 		/* bcc 			*/ vector<string>{},
	// 		/* subject  	*/ string("This is supposed to be a subject."),
	// 		/* body 		*/ string("This is my first body!"),
	// 		/* unread 		*/ false,
	// 		/* imagePaths 	*/ vector<string>{}
	// 	},
	// 	Email{
	// 		/* threadId	 	*/ string("new"),
	// 		/* from 		*/ string("test1@example.com"),
	// 		/* to			*/ vector<string>{"mydummyaccount@example.com"},
	// 		/* dateTime 	*/ (time_t) 1000000,
	// 		/* cc			*/ vector<string>{},
	// 		/* bcc 			*/ vector<string>{},
	// 		/* subject  	*/ string("This is supposed to be a subject."),
	// 		/* body 		*/ string("This is my second body!"),
	// 		/* unread 		*/ false,
	// 		/* imagePaths 	*/ vector<string>{}
	// 	}

	// };

	// for_each(emails.begin(), emails.end(), [&](Email &e){
	// 	myDummyAccount.sendEmail(e);
	// });

	Compositor& com = Compositor::instance();

	shared_ptr<NWindow> myAccountView;
	shared_ptr<NWindow> myAccountSelectWindow;
	shared_ptr<NWindow> myActiveFolderView;
	shared_ptr<NWindow> myToolbar;
	shared_ptr<NWindow> myAccountUpsertWindow;
	shared_ptr<NWindow> myActiveDialog;
	shared_ptr<NWindow> myAccountToolbar;

	// DO NOT USE
	Account activeAccount;

	auto bindWindow = [&](std::shared_ptr<NWindow>& window, std::shared_ptr<NWindow> instance){
		if(window)
			com.removeWindow(window);
		window = instance;
		com.addWindow(window);
		com.setActiveWindow(window);
		com.refresh();
	};
	auto destroyWindow = [&](std::shared_ptr<NWindow> window){
		if(window){
			com.removeWindow(window);
			com.refresh();
		}
	};

	auto makeDialog = [&](string title, string message){
		bindWindow(myActiveDialog, 
			myActiveDialog = make_shared<Dialog>(title, message, [&]{
				destroyWindow(myActiveDialog);
			}));
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

		bindWindow(myAccountView, make_shared<AccountView>(acc, [&](string folderPath){
			bindWindow(myActiveFolderView, 	myActiveFolderView = make_shared<FolderView>(acc, folderPath, 
				[&](string threadId){

				}));
		}));

		bindWindow(myAccountToolbar, make_shared<Toolbar>(1, list<string>{acc.getEmailAddress(), "Compose"}, 
			[&](string selected){

			}));
	};

	auto registerAccount = [&](string username, string password){
		// TEMP
		shared_ptr<LocalEmailProvider> provider = localState.localProvider;
		Account myNewAccount{provider, username};
		provider->addAccount(username, password);
		myNewAccount.login(username, password);
		localState.storeAccount(myNewAccount);
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
						}));
				}
			} else if(selected == "Login") {
				bindWindow(myAccountUpsertWindow, make_shared<AccountUpsert>(
					[&](string username, string password, bool newAccount){
						if(username != "" && password != ""){
							if(newAccount) registerAccount(username, password);
							else loginAccount(username, password);
						}
						destroyWindow(myAccountUpsertWindow);
					}));
			} else if(selected == "Logout") {
				logoutActiveAccount();
			}else if (selected == "Exit"){
				com.quit();
			}else {
				throw exception{};
			}
	});

	auto mywin = make_shared<MyDummyWindow>();

	com.addWindow(myToolbar);
	com.addWindow(mywin);

	com.setActiveWindow(myToolbar);

	com.refresh();
	com.run();

	ofstream stateFileOut(STATE_FILE);
	stateFileOut << localState;


	return 0;
}
