
/*
    Log.h
*/

#pragma once
#include <iostream>
#include <string>
#include <cassert>

#define Vi Vi
namespace Vi {

    class Log {
    public:
        Log() = delete;
        Log(const Log&) = delete;
        Log(Log&&) noexcept = delete;

        static void error(std::string message = "") {
            std::cerr << "\033[31mError:\033[0m ";
            std::cerr << message << "\n";
            assert(false);
        }

        static void warning(std::string message = "") {
            std::cerr << "\033[33mWarning:\033[0m ";
            std::cerr << message << "\n";
        }

        static void info(std::string message = "") {
            std::cerr << "\033[32mInfo:\033[0m ";
            std::cerr << message << "\n";
        }
    };
}

