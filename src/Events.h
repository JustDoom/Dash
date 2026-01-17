#ifndef DASH_EVENTS_H
#define DASH_EVENTS_H

#include <xcb/xcb.h>

namespace Events {
    void destroyWindow(xcb_generic_event_t* event);
    void enterWindow(xcb_generic_event_t* event);
    void mapWindow(xcb_generic_event_t* event);
}

#endif