#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <graphics/nwindow.h>

#include <memory>
#include <list>
#include <set>

class Compositor {

    static std::unique_ptr<Compositor> _instance;
    static void resizeHandler(int sig);


    typedef std::set<std::shared_ptr<NWindow>, 
        bool(*)(const std::shared_ptr<NWindow>&,const std::shared_ptr<NWindow>&)> WindowSet;
    WindowSet windows;
    WindowSet::iterator activeWindow;

    bool shouldquit = false;

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
    void setActiveWindow(std::shared_ptr<NWindow> window);
    NWindow* getActiveWindow();

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