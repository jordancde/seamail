

#include "graphics/compositor.h"
#include "graphics/toolbar.h"

#include <cstdio>
#include <iostream>

#include "utility/localState.h"

#include "providers/localEmailProvider.h"
#include "account/account.h"

#include "graphics/accountSelect.h"
#include "graphics/accountUpsert.h"
#include "view/accountView.h"
#include "view/folderView.h"

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
		int y = 1;
		int w = maxx() / 2;
		int h = maxy() - 1; 
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

int main()
{	
	LocalState localState;
	
	// std::vector<Account> accounts;
	// string myEmailAddress = "mydummyaccount@example.com";
	// auto myLocalEmailProvider = std::make_shared<LocalEmailProvider>();
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

	// std::for_each(emails.begin(), emails.end(), [&](Email &e){
	// 	myDummyAccount.sendEmail(e);
	// });

	Compositor& com = Compositor::instance();

	std::shared_ptr<AccountView> myAccountView;
	std::shared_ptr<NWindow> myAccountSelectWindow;
	std::shared_ptr<FolderView> myActiveFolderView;
	std::shared_ptr<Toolbar> myToolbar;
	std::shared_ptr<NWindow> myAccountUpsertWindow;

	auto logoutActiveAccount = [&]{
		if(myActiveFolderView)
			com.removeWindow(myActiveFolderView);
		if(myAccountView)
			com.removeWindow(myAccountView);
		com.refresh();
	};

	auto changeActiveAccount = [&](Account& acc){
		logoutActiveAccount();
		myAccountView = std::make_shared<AccountView>(acc, [&](std::string folderPath){
			if(myActiveFolderView)
				com.removeWindow(myActiveFolderView);
			myActiveFolderView = std::make_shared<FolderView>(acc, folderPath, 
				[&](std::string threadId){

				});
			com.addWindow(myActiveFolderView);
			myActiveFolderView->refresh();
		});
		com.addWindow(myAccountView);
		com.refresh();
	};

	auto registerAccount = [&](string username, string password){

	};

	auto loginAccount = [&](string username, string password){

	};

	myToolbar = std::make_shared<Toolbar>(0, 
		std::list<std::string>{"Accounts", "Login", "Logout", "Exit"}, [&](std::string selected){
			if(selected == "Accounts"){
				myAccountSelectWindow = std::make_shared<AccountSelect>(localState.getAccounts(), 
					[&](Account& selectedAccount){
						com.removeWindow(myAccountSelectWindow);
						changeActiveAccount(selectedAccount);
					});
				com.addWindow(myAccountSelectWindow);
				com.setActiveWindow(myAccountSelectWindow);
				com.refresh();
			} else if(selected == "Login") {
				myAccountUpsertWindow = std::make_shared<AccountUpsert>(
					[&](std::string username, std::string password, bool newAccount){
						if(username != "" && password != ""){
							
						}
						com.removeWindow(myAccountUpsertWindow);
					});
				com.addWindow(myAccountUpsertWindow);
				com.setActiveWindow(myAccountUpsertWindow);
				com.refresh();
			} else if(selected == "Logout") {
				logoutActiveAccount();
			}else if (selected == "Exit"){
				com.quit();
			}else {
				throw exception{};
			}
	});

	auto mywin = std::make_shared<MyDummyWindow>();

	com.addWindow(myToolbar);
	com.addWindow(mywin);

	com.setActiveWindow(myToolbar);

	com.refresh();
	com.run();

	return 0;
}
