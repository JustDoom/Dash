#ifndef DASH_WINDOW_H
#define DASH_WINDOW_H

#include <xcb/xcb_keysyms.h>

#include "util/Vector2D.h"

class Window {
public:
    Window(const bool dirty, const bool floating, const xcb_drawable_t drawable) : dirty(dirty), floating(floating), drawable(drawable) {};

    bool dirty;
    bool floating;
    Vector2D size;
    Vector2D position;
    xcb_drawable_t drawable;
};


#endif
