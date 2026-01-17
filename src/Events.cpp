#include "Events.h"

#include "WindowManager.h"
#include "util/Logger.h"

void Events::destroyWindow(xcb_generic_event_t* event) {
    const xcb_destroy_notify_event_t* destroyEvent = reinterpret_cast<xcb_destroy_notify_event_t*>(event);

    xcb_kill_client(windowManager->connection, destroyEvent->event);

    const Window* window = windowManager->getWindow(destroyEvent->window);
    if (!window) {
        return;
    }

    if (Window* neighbour = WindowManager::findNeighbour(window)) {
        WindowManager::chompWindow(neighbour, window);
        neighbour->dirty = true;
        windowManager->setFocusedWindow(neighbour->drawable);
    }

    windowManager->removeWindowFromList(window);
}

void Events::enterWindow(xcb_generic_event_t* event) {
    const xcb_enter_notify_event_t* enterEvent = reinterpret_cast<xcb_enter_notify_event_t*>(event);

    windowManager->setFocusedWindow(enterEvent->event);
    windowManager->getWindow(enterEvent->event)->dirty = true;
}

// Maps a new window to the screen
void Events::mapWindow(xcb_generic_event_t* event) {
    const xcb_map_request_event_t* requestEvent = reinterpret_cast<xcb_map_request_event_t*>(event);

    // Map the window
    xcb_map_window(windowManager->connection, requestEvent->window);

    Window window(requestEvent->window, true, false);

    windowManager->setupNewWindow(&window);
    windowManager->addWindowToList(window);

    windowManager->values[0] = XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_FOCUS_CHANGE;
    xcb_change_window_attributes_checked(windowManager->connection, requestEvent->window, XCB_CW_EVENT_MASK, windowManager->values);

    windowManager->setFocusedWindow(requestEvent->window);
}