

#include "graphics/compositor.h"

class MyDummyWindow : public NWindow {
public:
	MyDummyWindow() 
		: NWindow() {}
	void draw(size_t nw, size_t nh) const override {
		box(win, 0, 0);
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

	auto mywin = std::make_shared<MyDummyWindow>();
	com.addWindow(mywin);
	com.setActiveWindow(mywin);
	mywin->refresh();
	com.run();

	return 0;
}
