#ifndef DASH_WINDOWMANAGER_H
#define DASH_WINDOWMANAGER_H

#include <memory>
#include <vector>
#include <xcb/xcb.h>

#include "Window.h"

class WindowManager {
public:
    xcb_connection_t* connection = nullptr;
    xcb_screen_t* screen = nullptr;
    uint32_t values[3]{};

    std::vector<Window> windows;
    xcb_drawable_t currentWindow = -1;

    Window* getWindow(xcb_drawable_t window);
    void setFocusedWindow(xcb_drawable_t window);
    void addWindowToList(const Window& window);
    void removeWindowFromList(const Window* window);
    void setupNewWindow(Window* window);
    void calculateNewTileSet(Window* window);

    static bool handleEvent();
    static void handleDirtyWindows();
    static void chompWindow(Window* window, const Window* food);
    static bool isNeighbour(const Window* window, const Window* possibleNeighbour);
    static Window* findNeighbour(const Window* window);
};

inline auto windowManager = std::make_unique<WindowManager>();

#endif
