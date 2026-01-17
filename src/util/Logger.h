#ifndef DASH_DEBUG_H
#define DASH_DEBUG_H

#include <string>

namespace Log {
    void debug(const std::string& log);
    void info(const std::string& log);
    void error(const std::string& log);
}

#endif