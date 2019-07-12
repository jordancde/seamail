#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <graphics/nwindow.h>

#include <memory>
#include <list>
#include <set>
#include <stack>
#include <functional>

class Compositor {

    static std::unique_ptr<Compositor> _instance;
    static void resizeHandler(int sig);


    typedef std::set<std::shared_ptr<NWindow>, 
        bool(*)(const std::shared_ptr<NWindow>&,const std::shared_ptr<NWindow>&)> WindowSet;
    WindowSet windows;
    WindowSet::iterator activeWindow;

    bool shouldquit = false;

    std::stack<std::shared_ptr<NWindow>> activeWindowStack;

    Compositor();
public:
    virtual ~Compositor();

    Compositor(const Compositor&) = delete;
    Compositor(Compositor&&) = delete;
    Compositor& operator=(const Compositor&) = delete;
    Compositor& operator=(Compositor&&) = delete;

    static Compositor& instance();

    void addWindow(std::shared_ptr<NWindow> window);
    void removeWindow(std::shared_ptr<NWindow> window);
    bool setActiveWindow(std::shared_ptr<NWindow> window);
    NWindow* getActiveWindow();

    template<typename ReturnType>
    ReturnType runExternalProgram(std::function<ReturnType()> task){
        def_prog_mode();
        endwin();
        ReturnType t = task();
        reset_prog_mode();
        refresh();
        return t;
    }

    void runExternalProgram(std::function<void()> task){
        def_prog_mode();
        endwin();
        task();
        reset_prog_mode();
        refresh();
    }

    // force all child windows to handle resizing
    void resize();
    // force all child windows to redraw
    void refresh();
    // update virtual screen to terminal
    void update();
    // event loop
    void run();

    void quit();

};

#endif