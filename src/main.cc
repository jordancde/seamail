

#include "graphics/compositor.h"
#include "graphics/toolbar.h"

#include <cstdio>
#include <iostream>

#include "providers/localEmailProvider.h"
#include "account/account.h"
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

	void onInput(int key) override {
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
	}
};

int main()
{	
	Compositor& com = Compositor::instance();

	auto mytoolbar = std::make_shared<Toolbar>(0, 
		std::list<std::string>{"File", "Edit", "View"}, [](std::string selected){
	});

	string myEmailAddress = "mydummyaccount@example.com";
	auto myLocalEmailProvider = std::make_shared<LocalEmailProvider>();
	auto myDummyAccount = std::make_shared<Account>(myLocalEmailProvider, myEmailAddress);
	myLocalEmailProvider->addAccount(myEmailAddress, "abc123");
	myDummyAccount->login(myEmailAddress, "abc123");
	myDummyAccount->addFolder("inbox");
	myDummyAccount->addFolder("sent");
	myDummyAccount->addFolder("deleted");
	myDummyAccount->addFolder("Test Folder 1");
	myDummyAccount->addFolder("Test Folder 2");
	myDummyAccount->addFolder("Test Folder 3");
	myDummyAccount->addFolder("Test Folder 1/Nested Folder 1");
	myDummyAccount->addFolder("Test Folder 1/Nested Folder 2");
	myDummyAccount->addFolder("Test Folder 4");
	myDummyAccount->addFolder("Test Folder 5");
	myDummyAccount->addFolder("Test Folder 6");
	myDummyAccount->addFolder("Test Folder 7");
	vector<Email> emails {
		Email{
			/* threadId	 	*/ string("new"),
			/* from 		*/ string("test1@example.com"),
			/* to			*/ vector<string>{"mydummyaccount@example.com"},
			/* dateTime 	*/ (time_t) 0,
			/* cc			*/ vector<string>{},
			/* bcc 			*/ vector<string>{},
			/* subject  	*/ string("This is supposed to be a subject."),
			/* body 		*/ string("This is my first body!"),
			/* unread 		*/ false,
			/* imagePaths 	*/ vector<string>{}
		},
		Email{
			/* threadId	 	*/ string("new"),
			/* from 		*/ string("test1@example.com"),
			/* to			*/ vector<string>{"mydummyaccount@example.com"},
			/* dateTime 	*/ (time_t) 1000000,
			/* cc			*/ vector<string>{},
			/* bcc 			*/ vector<string>{},
			/* subject  	*/ string("This is supposed to be a subject."),
			/* body 		*/ string("This is my second body!"),
			/* unread 		*/ false,
			/* imagePaths 	*/ vector<string>{}
		}

	};

	std::for_each(emails.begin(), emails.end(), [&](Email &e){
		myDummyAccount->sendEmail(e);
	});

	std::shared_ptr<FolderView> myActiveFolderView;

	auto myAccountView = std::make_shared<AccountView>(myDummyAccount, [&](std::string folderPath){
		if(myActiveFolderView)
			com.removeWindow(myActiveFolderView);
		myActiveFolderView = std::make_shared<FolderView>(myDummyAccount, folderPath, 
			[&](std::string threadId){

			});
		com.addWindow(myActiveFolderView);
		myActiveFolderView->refresh();


	});
	auto mywin = std::make_shared<MyDummyWindow>();

	com.addWindow(mytoolbar);
	com.addWindow(myAccountView);
	com.addWindow(mywin);
	//com.setActiveWindow(mywin);

	com.setActiveWindow(mytoolbar);

	com.refresh();
	com.run();

	return 0;
}
