

#include "graphics/compositor.h"
#include "graphics/toolbar.h"

#include <iostream>

class MyDummyWindow : public NWindow {
public:
	MyDummyWindow() : NWindow(0,0,0,0) {
		onResize();
	}

	void onDraw(bool isActive) const override {
		if(isActive)
			wattron(win, A_REVERSE);
		box(win, 0, 0);
		wattroff(win, A_REVERSE);
		mvwprintw(win, 1, 1, "%zu %zu", maxx(), maxy());
	}

	void onResize() override {
		resize(0, 1, maxx() / 2, maxy()-1);
	}

	void onInput(int key) override {
		switch(key){
			case KEY_UP:
			move(cx(), cy() - 1);
			break;
			case KEY_DOWN:
			move(cx(), cy() + 1);
			break;
			case KEY_LEFT:
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

	auto mytoolbar = std::make_shared<Toolbar>(0, std::list<std::string>{"File", "Edit", "View"}, [](std::string selected){

	});
	com.addWindow(mytoolbar);
	auto mywin = std::make_shared<MyDummyWindow>();
	com.addWindow(mywin);
	//com.setActiveWindow(mywin);
	com.setActiveWindow(mytoolbar);
	com.refresh();
	com.run();

	return 0;
}
