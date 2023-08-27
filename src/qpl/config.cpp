#include <qpl/config.hpp>
#include <qpl/system.hpp>
#include <qpl/exception.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cwctype>

namespace qpl {
    void qpl::config::load(const std::string& file_name, char seperate) {
        std::ifstream infile(file_name);

        if (!infile.good()) {
            qpl::println("couldn't load \"", file_name, "\"!");
            qpl::system_pause();
        }
        std::string line;
        while (std::getline(infile, line)) {
            this->whole_string += line + "\n";

            qpl::u32 i = 0u;
            for (; i < line.length(); ++i) {
                if (line[i] == seperate) {
                    while (i < line.length() && std::isspace(line[i])) {
                        ++i;
                    }
                    break;
                }
            }

            this->strings.emplace_back(line.substr(i + 2));
        }
    }

    void qpl::config::wload(const std::wstring& file_name, wchar_t seperate) {
        std::wifstream infile(file_name);
    
        qpl::println("loading config file \"", file_name, "\"");
        if (!infile.good()) {
            throw qpl::exception("qpl::config::wload : couldn't load \"", file_name, "\"!");
        }
    
        auto content = qpl::winsys::read_utf8_file(file_name);
        auto lines = qpl::string_split(content, L'\n');
    
        for (auto& line : lines) {
            this->whole_wstring += line + L"\n";
    
            qpl::u32 i = 0u;
            for (; i < line.length(); ++i) {
                if (line[i] == seperate) {
                    while (i < line.length() && std::iswspace(line[i])) {
                        ++i;
                    }
                    break;
                }
            }
    
            this->wstrings.emplace_back(line.substr(i + 2));
        }
    }
    std::string qpl::config::string() const {
        return this->whole_string;
    }
    std::wstring qpl::config::wstring() const {
        return this->whole_wstring;
    }
}
