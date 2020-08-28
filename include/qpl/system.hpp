#ifndef QPL_SYSTEM_HPP
#define QPL_SYSTEM_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/string.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>

namespace qpl {
    struct args_t {
        std::string string;
    };
    template<typename... Args>
    args_t args(Args&&... args) {
        return args_t{ qpl::to_string_space(args...) };
    }

    struct input_t {
        std::string string;
    };
    template<typename... Args>
    input_t input(Args&&... args) {
        return input_t{ qpl::to_string_space(args...) };
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
    QPLDLL std::string get_hidden_input(const std::string_view& replace = "*");

    QPLDLL void system_pause(qpl::u32 max_keyboard_latency_hz = 50'000u);
    QPLDLL void system_pause(const std::string_view& message, qpl::u32 max_keyboard_latency_hz = 50'000u);
    QPLDLL void system_pause_clear_console(qpl::u32 max_keyboard_latency_hz = 50'000u);


}

#endif