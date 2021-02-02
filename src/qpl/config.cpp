#include <qpl/config.hpp>
#include <qpl/system.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace qpl {
    void qpl::config::load(const std::string& file_name, char seperate) {
        std::ifstream infile(file_name);

        qpl::println("loading config file \"", file_name, "\"");
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
    std::string qpl::config::string() const {
        return this->whole_string;
    }
}
