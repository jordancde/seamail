#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <graphics/nwindow.h>

#include <memory>
#include <list>

class Compositor {

    static std::unique_ptr<Compositor> _instance;
    static void resizeHandler(int sig);

    std::list<std::shared_ptr<NWindow>> windows;
    std::shared_ptr<NWindow> activeWindow; 

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

    // force all child windows to handle resizing
    void resize();
    // force all child windows to redraw
    void refresh();
    // update virtual screen to terminal
    void update();
    // event loop
    void run();

};

#endif