#include <memory>

#include <xcb/xcb.h>

#include "util/Logger.h"
#include "WindowManager.h"

int main() {
    Log::info("Starting Dash");

    // Connect to the X Server and check if is succeeded
    windowManager->connection = xcb_connect(nullptr, nullptr);
    if (const auto ret = xcb_connection_has_error(windowManager->connection); ret != 0) {
        Log::error("Connection Failed! Return: " + std::to_string(ret));
        return ret;
    }

    windowManager->screen = xcb_setup_roots_iterator(xcb_get_setup(windowManager->connection)).data;

    windowManager->values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;
    xcb_change_window_attributes_checked(
        windowManager->connection,
        windowManager->screen->root,
        XCB_CW_EVENT_MASK,
        windowManager->values
    );

    xcb_ungrab_key(windowManager->connection, XCB_GRAB_ANY, windowManager->screen->root, XCB_MOD_MASK_ANY);
    xcb_flush(windowManager->connection);

    Log::info("Dash started successfully as Window Manager");

    while (WindowManager::handleEvent()) {}

    Log::info("Exiting...");
    return EXIT_SUCCESS;
}
