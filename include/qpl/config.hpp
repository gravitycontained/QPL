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
        std::string whole_string;

       QPLDLL void load(const std::string& file_name, char seperate = ':');
       QPLDLL std::string string() const;

       template <typename T = std::string>
       T get(qpl::size index) const {
           return qpl::from_string<T>(this->strings[index]);
       }
    };
}

#endif