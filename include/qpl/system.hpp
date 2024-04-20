#ifndef QPL_SYSTEM_HPP
#define QPL_SYSTEM_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/string.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <string_view>

namespace qpl {
    struct args_t {
        std::string string;
    };
    template<typename... Args>
    args_t args(Args&&... args) {
        std::ostringstream stream;
        bool first = true;
        for (auto& i : args) {
            if (!first) {
                stream << ' ';
            }
            stream << i;
        }
        return args_t{ stream.str()};
    }

    struct input_t {
        std::string string;
    };
    template<typename... Args>
    input_t input(Args&&... args) {
        std::ostringstream stream;
        bool first = true;
        for (auto& i : args) {
            if (!first) {
                stream << ' ';
            }
            stream << i;
        }
        return input_t{ stream.str() };
    }

#ifdef QPL_USE_BOOST
    QPLDLL int execute(const std::string_view& what);
    QPLDLL int execute(const std::string_view& what, const input_t& input);
    QPLDLL int execute(const std::string_view& path, const std::vector<std::string>& arguments);
    QPLDLL int execute(const std::string_view& path, const qpl::args_t& arguments);
    QPLDLL int execute(const std::string_view& path, const qpl::args_t& arguments, const input_t& input);
#endif

    QPLDLL void hide_input(bool enable);
    QPLDLL std::string get_input();
    QPLDLL std::string get_multiline_input();
    QPLDLL std::wstring get_multiline_input_wstring();
    QPLDLL std::wstring get_input_wstring();
    QPLDLL void wait_for_enter();

    template<typename T>
    T get_input() {
        std::string value;
        std::getline(std::cin, value);
        return qpl::string_cast<T>(value);
    }
    QPLDLL std::string get_hidden_input(const std::string_view& replace = "*");

    QPLDLL void system_pause(qpl::u32 max_keyboard_latency_hz = 50'000u);
    QPLDLL void system_pause(const std::string_view& message, qpl::u32 max_keyboard_latency_hz = 50'000u);
    QPLDLL void system_pause_clear_console(qpl::u32 max_keyboard_latency_hz = 50'000u);

    QPLDLL std::string get_environment_variable(std::string const& key);
}

#endif