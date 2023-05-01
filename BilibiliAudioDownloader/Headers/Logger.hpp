#pragma once
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <string>
#include <utils.hpp>
#include <stringHelper.hpp>

void inline logTime() {
    fmt::print("[{:%H:%M:%S}] ", fmt::localtime(_time64(0)));
}
void inline endl() {
    fmt::print("\n");
}
#define log (std::string& msg, const Args&... args) {\
    logTime();\
    fmt::print(msg + '\n', args...);\
}
namespace logger {
    enum errorID {
        fileErr = 1,
        httpErr,
        logicErr
    };
    template<typename... Args>
    void info(const std::string& msg, const Args&... args) {
        logTime();
        fmt::print(msg, args...);
        endl();
    }
    inline void info(const std::string& msg) {
        logTime();
        fmt::print(msg);
        endl();
    }
    template<typename... Args>
    inline void info(const char msg[], const Args&... args) {
        logTime();
        fmt::print(msg, args...);
        endl();
    }
    inline void info(const char msg[]) {
        logTime();
        fmt::print(msg);
        endl();
    }
}