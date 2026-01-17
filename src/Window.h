#ifndef DASH_WINDOW_H
#define DASH_WINDOW_H

#include <xcb/xcb_keysyms.h>

#include "util/Vector2D.h"

class Window {
public:
    Window(const xcb_drawable_t drawable, const bool dirty, const bool floating) : drawable(drawable), dirty(dirty), floating(floating) {};

    xcb_drawable_t drawable;

    bool dirty;
    bool floating;
    Vector2D size;
    Vector2D position;
    double margin = 10;
};


#endif
