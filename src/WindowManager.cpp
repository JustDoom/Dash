#include "WindowManager.h"

#include <algorithm>

#include "Events.h"
#include "util/Logger.h"

bool WindowManager::handleEvent() {
    // Check if the connection is still alive
    if (xcb_connection_has_error(windowManager->connection)) {
        return false;
    }

    if (xcb_generic_event_t* event = xcb_wait_for_event(windowManager->connection); event != nullptr) {
        switch (event->response_type & ~0x80) {
            case XCB_DESTROY_NOTIFY:
                Log::info("Event dispatched DESTROY");
                Events::destroyWindow(event);
                break;
            case XCB_ENTER_NOTIFY:
                Log::info("Event dispatched ENTER");
                Events::enterWindow(event);
                break;
            case XCB_MAP_REQUEST:
                Log::info("Event MAP_REQUEST");
                Events::mapWindow(event);
                break;
            default:
                Log::info("Unknown event: " + std::to_string(event->response_type & ~0x80));
                break;
        }

        free(event);
    }

    handleDirtyWindows();
    xcb_flush(windowManager->connection);

    return true;
}

void WindowManager::handleDirtyWindows() {
    for (auto& window : windowManager->windows) {
        if (!window.dirty) {
            continue;
        }

        windowManager->values[0] = static_cast<int>(window.size.x);
        windowManager->values[1] = static_cast<int>(window.size.y);
        xcb_configure_window(windowManager->connection, window.drawable, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, windowManager->values);

        windowManager->values[0] = static_cast<int>(window.position.x);
        windowManager->values[1] = static_cast<int>(window.position.y);
        xcb_configure_window(windowManager->connection, window.drawable, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, windowManager->values);

        window.dirty = false;
    }
}

void WindowManager::setFocusedWindow(const xcb_drawable_t window) {
    if (!window || window == this->screen->root) {
        return;
    }

    xcb_set_input_focus(this->connection, XCB_INPUT_FOCUS_POINTER_ROOT, window, XCB_CURRENT_TIME);

    this->currentWindow = window;
}

Window* WindowManager::getWindow(const xcb_drawable_t window) {
    for (Window& windowGot : this->windows) {
        if (windowGot.drawable == window) {
            return &windowGot;
        }
    }
    return nullptr;
}

void WindowManager::addWindowToList(const Window& window) {
    for (const auto& windowGot : this->windows) {
        if (windowGot.drawable == window.drawable) {
            return;
        }
    }
    this->windows.push_back(window);
}

void WindowManager::removeWindowFromList(const Window* window) {
    if (!window) {
        return;
    }

    const auto it = std::ranges::remove_if(this->windows, [window](const Window& windowGot) {
        return windowGot.drawable == window->drawable;
    }).begin();
    this->windows.erase(it, this->windows.end());
}

void WindowManager::setupNewWindow(Window* window) {
    if (!window) {
        return;
    }

    if (!window->floating) {
        calculateNewTileSet(window);
    }

    window->dirty = true;
}

void WindowManager::calculateNewTileSet(Window* window) {
    // Check if there is a window in focus. If there is, that will be selected to be tiled
    if (const auto lastWindow = getWindow(this->currentWindow)) {
        if (const auto lastWindowSize = lastWindow->size; lastWindowSize.x > lastWindowSize.y) {
            window->size = Vector2D(lastWindowSize.x / 2.f, lastWindowSize.y);
            window->position = Vector2D(lastWindow->position.x + lastWindowSize.x / 2.f, lastWindow->position.y);
            lastWindow->size = Vector2D(lastWindowSize.x / 2.f, lastWindowSize.y);
        } else {
            window->size = Vector2D(lastWindowSize.x, lastWindowSize.y / 2.f);
            window->position = Vector2D(lastWindow->position.x, lastWindow->position.y + lastWindowSize.y / 2.f);
            lastWindow->size = Vector2D(lastWindowSize.x, lastWindowSize.y / 2.f);
        }

        lastWindow->dirty = true;
    } else {
        // Open a fullscreen window
        window->size = Vector2D(this->screen->width_in_pixels, this->screen->height_in_pixels);
        window->position = Vector2D(0, 0);
    }
}

void WindowManager::chompWindow(Window* window, const Window* food) {
    const auto windowCorner = window->position + window->size;
    const auto foodCorner = food->position + food->size;

    window->position = Vector2D(std::min(window->position.x, food->position.x), std::min(window->position.y, food->position.y));
    window->size = Vector2D(std::max(windowCorner.x, foodCorner.x), std::max(windowCorner.y, foodCorner.y)) - window->position;
}

bool WindowManager::isNeighbour(const Window* window, const Window* possibleNeighbour) {
    if (!window || !possibleNeighbour) {
        return false;
    }
    const auto aLeft = window->position.x;
    const auto aRight = window->position.x + window->size.x;
    const auto aTop = window->position.y;
    const auto aBottom = window->position.y + window->size.y;

    const auto bLeft = possibleNeighbour->position.x;
    const auto bRight = possibleNeighbour->position.x + possibleNeighbour->size.x;
    const auto btop = possibleNeighbour->position.y;
    const auto bBottom = possibleNeighbour->position.y + possibleNeighbour->size.y;

    const bool verticalAdjacent = (aRight == bLeft || aLeft == bRight) &&(aBottom > btop && aTop < bBottom);
    const bool horizontalAdjacent = (aBottom == btop || aTop == bBottom) &&(aRight > bLeft && aLeft < bRight);
    const bool noOverlap = !(aRight > bLeft && aLeft < bRight && aBottom > btop && aTop < bBottom);
    return (verticalAdjacent || horizontalAdjacent) && noOverlap;
}


Window* WindowManager::findNeighbour(const Window* window) {
    for (Window& possibleNeighbour : windowManager->windows) {
        if (possibleNeighbour.drawable == window->drawable || !isNeighbour(window, &possibleNeighbour)) {
            continue;
        }

        return &possibleNeighbour;
    }

    return nullptr;
}
