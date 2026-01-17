#include "Logger.h"

#include <cstdio>

void Log::debug(const std::string& log) {
    printf("[DEBUG] %s\n", log.c_str());
}

void Log::info(const std::string& log) {
    printf("[INFO] %s\n", log.c_str());
}

void Log::error(const std::string& log) {
    printf("[ERROR] %s\n", log.c_str());
}