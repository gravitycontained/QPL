#ifndef QPL_CONFIG_HPP
#define QPL_CONFIG_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <string>
#include <string>
#include <vector>


namespace qpl {

    struct config {
        std::vector<std::string> strings;
        std::vector<std::wstring> wstrings;
        std::string whole_string;
        std::wstring whole_wstring;

       QPLDLL void load(const std::string& file_name, char seperate = ':');
       QPLDLL void wload(const std::wstring& file_name, wchar_t seperate = L':');
       QPLDLL std::string string() const;
       QPLDLL std::wstring wstring() const;

       template <typename T = std::string>
       T get(qpl::size index) const {
           return qpl::string_cast<T>(this->strings[index]);
       }
       auto begin() {
           return this->strings.begin();
       }
       auto cbegin() const {
           return this->strings.cbegin();
       }
       auto begin() const {
           return this->strings.cbegin();
       }
       auto end() {
           return this->strings.end();
       }
       auto cend() const {
           return this->strings.cend();
       }
       auto end() const {
           return this->strings.cend();
       }
       template <typename T = std::wstring>
       T wget(qpl::size index) const {
           return qpl::string_cast<T>(this->wstrings[index]);
       }
    };
}

#endif