#include <qpl/filesys.hpp>
#include <qpl/string.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/system.hpp>
#include <qpl/time.hpp>
#include <qpl/encryption.hpp>

namespace qpl {

    namespace filesys {

        qpl::filesys::path& qpl::filesys::path::operator=(const std::string_view& str) {
            this->m_string.clear();

            bool seperator = false;
            for (qpl::u32 i = 0u; i < str.size(); ++i) {
                seperator = false;
                while (i < str.size() && (str[i] == '\\' || str[i] == '/')) {
                    seperator = true;
                    ++i;
                }

                if (seperator) {
                    this->m_string.push_back('/');
                }
                if (i < str.size()) {
                    this->m_string.push_back(str[i]);
                }
            }

            this->m_update = true;
            return *this;
        }
        qpl::filesys::path& qpl::filesys::path::operator=(const std::string& str) {
            return this->operator=(std::string_view{ str.c_str() });
        }
        qpl::filesys::path& qpl::filesys::path::operator=(const char* str) {
            return this->operator=(std::string_view{ str });
        }
        qpl::filesys::path& qpl::filesys::path::operator=(const path& other) {
            this->m_string = other.m_string;
            this->m_is_file = other.m_is_file;
            this->m_is_directory = other.m_is_directory;
            this->m_update = other.m_update;
            return *this;
        }
        path& qpl::filesys::path::operator=(const std::filesystem::directory_entry& entry) {
            this->m_string = entry.path().string();
            for (qpl::u32 i = 0u; i < this->m_string.size(); ++i) {
                if (this->m_string[i] == '\\') {
                    this->m_string[i] = '/';
                }
            }

            this->m_is_file = entry.is_regular_file() || entry.is_block_file() || entry.is_character_file();
            this->m_is_directory = entry.is_directory();
            this->m_exists = entry.exists();
            this->m_update = false;
            return *this;
        }

        bool qpl::filesys::path::operator==(const path& other) const {
            std::string name1 = this->m_string;
            std::string name2 = other.m_string;

            if (this->is_directory() && name1.back() != '/') {
                name1.append("/");
            }
            if (other.is_directory() && name2.back() != '/') {
                name2.append("/");
            }
            return qpl::string_equals_ignore_case(name1, name2);
        }
        bool qpl::filesys::path::file_content_equals(const path& other) const {
            if (this->is_file() && other.is_file()) {
                auto str1 = this->read();
                auto str2 = other.read();
                return str1 == str2;
            }
            return false;
        }

        qpl::filesys::path qpl::filesys::path::current_path() {
            return std::filesystem::current_path().string();
        }
        void qpl::filesys::path::set_current_path() {
            *this = qpl::filesys::path::current_path();
        }

        const char* qpl::filesys::path::data() const {
            return this->m_string.data();
        }
        const char* qpl::filesys::path::c_str() const {
            return this->m_string.c_str();
        }
        std::string qpl::filesys::path::string() const {
            return this->m_string;
        }
        qpl::filesys::path::operator std::string() const {
            return this->m_string;
        }
        bool qpl::filesys::path::empty() const {
            return this->m_string.empty();
        }

        void qpl::filesys::path::create() const {
            if (this->exists() || this->empty()) {
                return;
            }
            if (this->is_file()) {
                std::ofstream file(this->string());
                file.close();
            }
            else if (this->is_directory()) {
                std::filesystem::create_directory(this->string());
            }
        }
        qpl::filesys::path qpl::filesys::path::make_file(std::string file_name) const {
            auto path = this->string();
            if (this->is_file()) {
                path = this->get_parent_branch();
            }
            auto path_slash = path.back() == '/';
            auto file_slash = file_name.front() == '/';
            if (path_slash && file_slash) {
                path.pop_back();
                path += file_name;
            }
            else if (!path_slash && !file_slash) {
                path += "/" + file_name;
            }
            else {
                path += file_name;
            }
            qpl::filesys::create(path);
            return path;
        }
        std::string qpl::filesys::path::read() const {
            if (this->empty()) {
                return "";
            }

            return qpl::filesys::read_file(this->string());
        }
        void qpl::filesys::path::write(const std::string& data) {
            std::ofstream file(this->string(), std::ios::binary);
            file << data;
            file.close();
        }
        std::filesystem::file_time_type qpl::filesys::path::last_write_time() const {
            return std::filesystem::last_write_time(this->string());
        }
        qpl::u64 qpl::filesys::path::file_size() const {
            return std::filesystem::file_size(this->string());
        }
        qpl::u64 qpl::filesys::path::file_size_recursive() const {
            if (this->is_file()) {
                return this->file_size();
            }

            qpl::u64 result = 0u;
            auto paths = this->list_current_directory_tree();
            for (auto& path : paths) {
                if (path.is_file()) {
                    result += path.file_size();
                }
            }
            return result;
        }

        bool qpl::filesys::path::exists() const {
            this->check_update();
            return this->m_exists;
        } 
        bool qpl::filesys::path::exists_system() const {
            this->m_exists = std::filesystem::exists(this->string());
            return this->m_exists;
        }
        bool qpl::filesys::path::is_file() const {
            this->check_update();
            return this->m_is_file;
        }

        bool qpl::filesys::path::is_directory() const {
            this->check_update();
            return this->m_is_directory;
        }

        bool qpl::filesys::path::has_extension() const {
            if (!this->is_file() || this->empty()) {
                return false;
            }
            for (auto it = this->m_string.crbegin(); it != this->m_string.crend(); ++it) {
                if (*it == '/') {
                    return false;
                }
                if (*it == '.') {
                    return true;
                }
            }
            return false;
        }
        std::string qpl::filesys::path::get_file_extension() const {
            return std::string{ this->get_file_extension_view() };
        }
        std::string qpl::filesys::path::get_name() const {
            return std::string{ this->get_name_view() };
        }
        std::string qpl::filesys::path::get_full_name(bool add_slash_on_directory) const {
            if (this->empty()) {
                return "";
            }
            if (this->is_file()) {
                return this->get_full_file_name();
            }
            else if (this->is_directory()) {
                auto str = this->get_directory_name();
                if (add_slash_on_directory && str.back() != '/') {
                    str.push_back('/');
                }
                return str;
            }
            return "";
        }
        std::string qpl::filesys::path::get_file_name() const {
            return std::string{ this->get_file_name_view() };
        }
        std::string qpl::filesys::path::get_full_file_name() const {
            return std::string{ this->get_full_file_name_view() };
        }
        std::string qpl::filesys::path::get_directory_name() const {
            return std::string{ this->get_directory_name_view() };
        }


        std::string_view qpl::filesys::path::get_file_extension_view() const {
            if (!this->is_file() || this->empty()) {
                return "";
            }

            auto it = this->m_string.cend() - 1;
            for (; it != this->m_string.cbegin(); --it) {
                if (*it == '.') {
                    ++it;
                    break;
                }
            }
            auto result = std::string_view(this->m_string.data() + std::distance(this->m_string.cbegin(), it), std::distance(it, this->m_string.cend()));
            return result;
        }
        std::string_view qpl::filesys::path::get_name_view() const {
            if (this->empty()) {
                return "";
            }
            if (this->is_file()) {
                return this->get_file_name_view();
            }
            else if (this->is_directory()) {
                return this->get_directory_name_view();
            }
            return "";
        }
        std::string_view qpl::filesys::path::get_full_file_name_view() const {
            if (!this->is_file() || this->empty()) {
                return "";
            }

            auto it = this->m_string.cend() - 1;
            for (; it != this->m_string.cbegin(); --it) {
                if (*it == '/') {
                    ++it;
                    break;
                }
            }
            return std::string_view(this->m_string.data() + std::distance(this->m_string.cbegin(), it), std::distance(it, this->m_string.cend()));
        }
        std::string_view qpl::filesys::path::get_file_name_view() const {
            if (!this->is_file() || this->empty()) {
                return "";
            }

            auto file_name = this->get_full_file_name_view();

            auto it = file_name.cend() - 1;
            for (; it != file_name.cbegin(); --it) {
                if (*it == '.') {
                    break;
                }
            }
            return std::string_view(file_name.cbegin(), it);
        }
        std::string_view qpl::filesys::path::get_directory_name_view() const {
            if (!this->is_directory() || this->empty()) {
                return "";
            }

            bool end_slash = false;
            auto it = this->m_string.cend() - 1;
            if (it != this->m_string.cbegin() && *it == '/') {
                --it;
                end_slash = true;
            }

            for (; it != this->m_string.cbegin(); --it) {
                if (*it == '/') {
                    ++it;
                    break;
                }
            }
            return std::string_view(this->m_string.data() + std::distance(this->m_string.cbegin(), it), std::distance(it, this->m_string.cend() - end_slash));
        }


        bool qpl::filesys::path::extension_equals(const std::string_view& str) const {
            if (str.front() == '.' && str.size()) {
                return this->get_file_extension_view() == str.substr(1, str.size() - 1);
            }
            return this->get_file_extension_view() == str;
        }
        bool qpl::filesys::path::extension_equals(const char* str) const {
            return this->extension_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::extension_equals(const std::string& str) const {
            return this->extension_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::extension_contains(const std::string_view& str) const {
            auto extension = this->get_file_extension_view();
            for (qpl::i32 i = 0; i < qpl::signed_cast(extension.size()) - qpl::signed_cast(str.size()) + 1; ++i) {
                if (extension.substr(i, str.size()) == str) {
                    return true;
                }
            }
            return false;
        }
        bool qpl::filesys::path::extension_contains(const char* str) const {
            return this->extension_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::extension_contains(const std::string& str) const {
            return this->extension_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::extension_matches(const std::string& regex) const {
            const std::regex reg{ regex };
            return std::regex_search(this->get_file_extension(), reg);
        }
        bool qpl::filesys::path::extension_matches(const std::regex& regex) const {
            return std::regex_match(this->get_file_extension(), regex);
        }

        bool qpl::filesys::path::file_name_equals(const std::string_view& str) const {
            return this->get_file_name_view() == str;
        }
        bool qpl::filesys::path::file_name_equals(const char* str) const {
            return this->file_name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::file_name_equals(const std::string& str) const {
            return this->file_name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::file_name_contains(const std::string_view& str) const {
            auto name = this->get_file_name_view();
            for (qpl::i32 i = 0; i < qpl::signed_cast(name.size()) - qpl::signed_cast(str.size()) + 1; ++i) {
                if (name.substr(i, str.size()) == str) {
                    return true;
                }
            }
            return false;
        }
        bool qpl::filesys::path::file_name_contains(const char* str) const {
            return this->file_name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::file_name_contains(const std::string& str) const {
            return this->file_name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::file_name_matches(const std::string& regex) const {
            const std::regex reg{ regex };
            return std::regex_search(this->get_file_name(), reg);
        }
        bool qpl::filesys::path::file_name_matches(const std::regex& regex) const {
            return std::regex_match(this->get_file_name(), regex);
        }

        bool qpl::filesys::path::full_file_name_equals(const std::string_view& str) const {
            return this->get_full_file_name_view() == str;
        }
        bool qpl::filesys::path::full_file_name_equals(const char* str) const {
            return this->full_file_name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::full_file_name_equals(const std::string& str) const {
            return this->full_file_name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::full_file_name_contains(const std::string_view& str) const {
            auto name = this->get_full_file_name_view();
            for (qpl::i32 i = 0; i < qpl::signed_cast(name.size()) - qpl::signed_cast(str.size()) + 1; ++i) {
                if (name.substr(i, str.size()) == str) {
                    return true;
                }
            }
            return false;
        }
        bool qpl::filesys::path::full_file_name_contains(const char* str) const {
            return this->full_file_name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::full_file_name_contains(const std::string& str) const {
            return this->full_file_name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::full_file_name_matches(const std::string& regex) const {
            const std::regex reg{ regex };
            return std::regex_search(this->get_full_file_name(), reg);
        }
        bool qpl::filesys::path::full_file_name_matches(const std::regex& regex) const {
            return std::regex_match(this->get_full_file_name(), regex);
        }

        bool qpl::filesys::path::name_equals(const std::string_view& str) const {
            auto name = this->get_name_view();
            if (this->is_directory()) {
                auto off = qpl::size{};
                auto size = str.size();
                if (str.front() == '/') {
                    ++off;
                }
                if (str.back() == '/') {
                    --size;
                }

                if (size > off) {
                    return name == str.substr(off, size);
                }
            }
            return name == str;
        }
        bool qpl::filesys::path::name_equals(const char* str) const {
            return this->name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::name_equals(const std::string& str) const {
            return this->name_equals(std::string_view{ str });
        }
        bool qpl::filesys::path::name_contains(const std::string_view& str) const {
            auto name = this->get_name_view();
            for (qpl::i32 i = 0; i < qpl::signed_cast(name.size()) - qpl::signed_cast(str.size()) + 1; ++i) {
                if (name.substr(i, str.size()) == str) {
                    return true;
                }
            }
            return false;
        }
        bool qpl::filesys::path::name_contains(const char* str) const {
            return this->name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::name_contains(const std::string& str) const {
            return this->name_contains(std::string_view{ str });
        }
        bool qpl::filesys::path::name_matches(const std::string& regex) const {
            const std::regex reg{ regex };
            return std::regex_search(this->get_name(), reg);
        }
        bool qpl::filesys::path::name_matches(const std::regex& regex) const {
            return std::regex_match(this->get_name(), regex);
        }


        void qpl::filesys::path::rename(const qpl::filesys::path& new_name) {
            std::filesystem::rename(this->string(), new_name.string());
            this->m_string = new_name;
        }
        void qpl::filesys::path::set_file_name(const std::string& new_name) {
            auto p = this->get_parent_branch().string();
            if (p.back() != '/') {
                p.push_back('/');
            }
            auto string = p + new_name + "." + this->get_file_extension();
            std::filesystem::rename(this->string(), string);
            this->m_string = string;
        }
        void qpl::filesys::path::set_file_extension_name(const std::string& new_extension) {
            auto p = this->get_parent_branch().string();
            if (p.back() != '/') {
                p.push_back('/');
            }
            auto string = this->string() + p + this->get_file_name() + "." + new_extension;
            std::filesystem::rename(this->string(), string);
            this->m_string = string;
        }
        void qpl::filesys::path::set_full_file_name(const std::string& new_file) {
            auto p = this->get_parent_branch().string();
            if (p.back() != '/') {
                p.push_back('/');
            }
            auto string = this->string() + p + new_file;
            std::filesystem::rename(this->string(), string);
            this->m_string = string;
        }
        void qpl::filesys::path::remove() const {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::remove: ", this->string(), " doesn't exist");
                return;
            }
            std::filesystem::remove_all(this->string());
        }

        void qpl::filesys::path::encrypt(const qpl::filesys::path& destination, const std::string& key, qpl::aes::mode mode) const {
            if (this->is_file()) {
                auto content = this->read();
                qpl::encrypt_keep_size(content, key, mode);
                qpl::write_data_file(content, destination);
            }
        }
        void qpl::filesys::path::decrypt(const qpl::filesys::path& destination, const std::string& key, qpl::aes::mode mode) const {
            if (this->is_file()) {
                auto content = this->read();
                qpl::decrypt_keep_size(content, key, mode);
                qpl::write_data_file(content, destination);
            }
        }
        void qpl::filesys::path::copy(const qpl::filesys::path& path_destination) const {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::copy: ", this->string(), " doesn't exist");
                return;
            }
            if (path_destination.is_directory()) {
                if (!path_destination.exists()) {
                    path_destination.create();
                }
                std::filesystem::copy(this->string(), path_destination.string(), std::filesystem::copy_options::recursive);
            }
            else {
                std::filesystem::copy(this->string(), path_destination.string(), std::filesystem::copy_options::recursive);
            }
        }
        void qpl::filesys::path::copy_overwrite(const qpl::filesys::path& path_destination) const {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::copy_overwrite: ", this->string(), " doesn't exist");
                return;
            }
            if (path_destination.is_directory()) {
                if (!path_destination.exists()) {
                    path_destination.create();
                }
                std::filesystem::copy(this->string(), path_destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            }
            else {
                std::filesystem::copy(this->string(), path_destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            }
        }
        void qpl::filesys::path::move(const qpl::filesys::path& path_destination) const {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::move: ", this->string(), " doesn't exist");
                return;
            }
            this->copy(path_destination);
            this->remove();
        }
        void qpl::filesys::path::move_overwrite(const qpl::filesys::path& path_destination) const {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::move_overwrite: ", this->string(), " doesn't exist");
                return;
            }
            this->copy_overwrite(path_destination);
            this->remove();
        }

        void qpl::filesys::path::move_and_apply_path(const qpl::filesys::path& path_destination) {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::move_and_apply_path: ", this->string(), " doesn't exist");
                return;
            }
            this->move(path_destination);
            *this = path_destination;
        }
        void qpl::filesys::path::move_overwrite_and_apply_path(const qpl::filesys::path& path_destination) {
            if (!this->exists()) {
                qpl::println("qpl::filesys::path::move_overwrite_and_apply_path: ", this->string(), " doesn't exist");
                return;
            }
            this->move_overwrite(path_destination);
            *this = path_destination;
        }

        qpl::size qpl::filesys::path::branch_size() const {
            qpl::size ctr = 0u;
            for (auto& i : this->m_string) {
                if (i == '/') {
                    ++ctr;
                }
            }
            if (this->is_directory() && this->m_string.back() != '/') {
                ++ctr;
            }
            return ctr;
        }
        qpl::filesys::path qpl::filesys::path::get_branch_at(qpl::size index) const {
            qpl::u32 ctr = 0u;
            auto it = this->m_string.cbegin();
            for (; it != this->m_string.cend(); ++it) {
                if (*it == '/' && (ctr++ == index)) {
                    ++it;
                    break;
                }
            }
            return { std::string(this->m_string.cbegin(), it) };
        }
        qpl::filesys::paths qpl::filesys::path::get_branches() const {
            qpl::filesys::paths result;

            auto it = this->m_string.cbegin();
            for (; it != this->m_string.cend(); ++it) {
                if (*it == '/') {
                    auto string = std::string(this->m_string.cbegin(), it);
                    string.push_back('/');
                    result.push_back(string);
                }
            }
            if (this->is_directory() && this->m_string.back() != '/') {
                result.push_back(*this);
                result.back().append('/');
            }
            return result;
        }
        qpl::filesys::path qpl::filesys::path::get_branch_earlier(qpl::u32 n) const {
            if (this->empty() || n == 0u) {
                return *this;
            }

            auto it = this->m_string.cend() - 1;
            if (this->is_directory() && *it == '/') {
                --it;
            }
            for (; it != this->m_string.cbegin(); --it) {
                if (*it == '/') {
                    --n;
                    if (!n) {
                        ++it;
                        break;
                    }
                }
            }
            return { std::string(this->m_string.cbegin(), it) };
        }
        qpl::filesys::path qpl::filesys::path::get_parent_branch() const {
            if (this->empty()) {
                return *this;
            }
            auto it = this->m_string.cend() - 1;
            if (this->is_directory() && *it == '/') {
                --it;
            }
            for (; it != this->m_string.cbegin(); --it) {
                if (*it == '/') {
                    ++it;
                    break;
                }
            }
            return { std::string(this->m_string.cbegin(), it) };
        }
        qpl::filesys::path qpl::filesys::path::get_last_branch() const {
            return this->get_branch_at(this->branch_size() - 1);
        }
        std::vector<std::string> qpl::filesys::path::get_branch_names() const {
            std::vector<std::string> result;
            
            auto it = this->m_string.cbegin();
            auto it_before = it;

            for (; it != this->m_string.cend(); ++it) {
                if (*it == '/') {
                    result.push_back(std::string(it_before, it));
                    it_before = it + 1;
                }
            }
            return result;
        }

        void qpl::filesys::path::set_branch(qpl::size index, std::string branch_name) {
            qpl::size ctr = 0u;
            for (qpl::size i = 0u; i < this->m_string.length(); ++i) {
                if (this->m_string[i] == '/') {
                    ++ctr;
                    if (ctr == index || !index) {
                        if (!index) {
                            i = 0u;
                        }
                        auto end = i + 1;
                        while (true) {
                            if (end >= this->m_string.length()) {
                                return;
                            }
                            if (this->m_string[end] == '/') {
                                break;
                            }
                            ++end;
                        }
                        if (index) {
                            auto string_a = this->m_string.substr(0u, i + 1u);
                            auto string_b = this->m_string.substr(end);
                            this->m_string = string_a + branch_name + string_b;
                        }
                        else {
                            auto string_b = this->m_string.substr(end);
                            this->m_string = branch_name + string_b;
                        }
                        return;
                    }
                }
            }
        }
        qpl::filesys::path qpl::filesys::path::subpath(qpl::size off, qpl::size size) const {
            qpl::u32 ctr = 0u;
            auto begin = this->m_string.cbegin();
            for (; begin != this->m_string.cend(); ++begin) {
                if (*begin == '/' && (ctr++ == off)) {
                    ++begin;
                    break;
                }
            }
            ctr = 0u;
            auto end = begin;
            for (; end != this->m_string.cend(); ++end) {
                if (*end == '/' && (ctr++ == size)) {
                    ++end;
                    break;
                }
            }

            return { std::string(begin, end) };
        }
        void qpl::filesys::path::ensure_branches_exist() {
            if (this->exists()) {
                return;
            }
            auto branches = this->get_branches();
            for (auto& i : branches) {
                if (!i.exists()) {
                    i.create();
                }
            }
        }
        void qpl::filesys::path::ensure_directory_backslash() {
            if (this->is_directory() && this->m_string.back() != '/') {
                this->m_string.push_back('/');
            }
        }

        bool qpl::filesys::path::is_root() const {
            return this->branch_size() == qpl::size{ 1 };
        }
        path& qpl::filesys::path::go_root() {
            return  *this = this->get_branch_at(qpl::u32{ 0 });
        }
        bool qpl::filesys::path::go_directories_back(qpl::u32 steps) {
            if (this->empty()) {
                return false;
            }
            auto size = this->branch_size() - 1;
            auto level = (steps > size ? qpl::size{} : size - steps);
            if (level != size) {
                *this = this->get_branch_at(static_cast<qpl::u32>(level));
                this->m_update = true;
                return true;
            }
            return false;
        }
        bool qpl::filesys::path::go_directory_back() {
            return this->go_directories_back(qpl::size{ 1 });
        }
        path& qpl::filesys::path::go_into_directory(const std::string& directory_name) {
            if (this->m_string.back() != '/' && directory_name.front() != '/') {
                this->m_string.append("/");
            }
            else if (this->m_string.back() == '/' && directory_name.front() == '/') {
                this->m_string.pop_back();
            }
            this->m_string.append(directory_name);

            if (this->m_string.back() != '/') {
                this->m_string.push_back('/');
            }
            this->m_update = true;
            this->m_exists = false;

            this->check_update();
            return *this;
        }
        path& qpl::filesys::path::go_into_file(const std::string& file_name) {
            if (this->m_string.back() != '/' && file_name.front() != '/') {
                this->m_string.append("/");
            }
            else if (this->m_string.back() == '/' && file_name.front() == '/') {
                this->m_string.pop_back();
            }
            this->m_string.append(file_name);

            this->m_update = true;
            this->m_exists = false;

            this->check_update();
            return *this;
        }
        path& qpl::filesys::path::go_into(const std::string& entry) {

            auto dir = *this;
            dir.go_into_directory(entry);

            if (dir.exists()) {
                return *this = dir;
            }
            auto file = *this;
            file.go_into_file(entry);

            return *this = file;
        }
        path& qpl::filesys::path::cd(const std::string& directory_name) {
            this->go_into_directory(directory_name);
            return *this;
        }
        void qpl::filesys::path::append(const std::string& string) {
            this->m_string.append(string);
        }
        void qpl::filesys::path::append(char c) {
            this->m_string += c;
        }

        qpl::filesys::paths qpl::filesys::path::list_current_directory() const {
            qpl::filesys::paths list;
            auto str = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(str, std::filesystem::directory_options::skip_permission_denied)) {
                list.push_back({ i.path().string() });
            }
            return { list };
        }
        qpl::filesys::paths qpl::filesys::path::list_current_directory_tree() const {
            if (!this->exists_system()) {
                return {};
            }
            qpl::filesys::paths list;
            auto str = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(str, std::filesystem::directory_options::skip_permission_denied)) {
                list.push_back({ i.path().string() });
            }
            return { list };
        }
        void qpl::filesys::path::print_current_directory() const {
            auto list = this->list_current_directory();
            list.print_tree();
        }
        void qpl::filesys::path::print_tree() const {
            auto str = this->is_directory() ? this->string() : this->get_parent_branch().string();

            std::vector<qpl::size> widths;
            if (this->is_root()) {
                widths.push_back(0);
            }

            qpl::size depth_before = this->branch_size() + 1;

            for (qpl::size i = 0u; i < depth_before - 1; ++i) {
                for (qpl::u32 i = 0u; i < widths.size(); ++i) {
                    qpl::print_repeat(' ', widths[i] + 1);
                    if (i == widths.size() - 1) {
                        qpl::print("|");
                    }
                    else {
                        qpl::print("|");
                    }
                }
                auto str = this->get_branch_at(i);
                qpl::print("_ ");
                qpl::print(str.get_name());
                widths.push_back(str.get_name().length());
                qpl::println();

            }

            for (auto i : std::filesystem::recursive_directory_iterator(str, std::filesystem::directory_options::skip_permission_denied)) {
                auto path = qpl::filesys::path(i.path().string());
                auto depth = path.branch_size();

                while (depth_before < depth) {
                    if (depth_before) {
                        auto name = path.get_branch_at(static_cast<qpl::u32>(depth_before - 1)).get_name();
                        widths.push_back(name.size());
                    }
                    ++depth_before;
                }
                while (depth_before > depth) {
                    if (depth_before) {
                        widths.pop_back();
                    }
                    --depth_before;
                }
                for (qpl::u32 i = 0u; i < widths.size(); ++i) {
                    qpl::print_repeat(' ', widths[i] + 1);
                    if (i == widths.size() - 1) {
                        qpl::print("|");
                    }
                    else {
                        qpl::print("|");
                    }
                }

                qpl::print("_ ");
                qpl::println(path.get_name());
            }
        }


        qpl::filesys::paths qpl::filesys::path::make_directory_range() const {
            return qpl::filesys::paths(this->list_current_directory());
        }
        qpl::filesys::paths qpl::filesys::path::make_directory_range_tree() const {
            return qpl::filesys::paths(this->list_current_directory_tree());
        }

        qpl::size qpl::filesys::path::last_common_branch(const qpl::filesys::path& path) const {
            qpl::size result = 0u;
            auto stop = qpl::min(this->m_string.size(), path.m_string.size());
            for (qpl::u32 i = 0; i < stop; ++i) {
                if (this->m_string[i] != path.m_string[i]) {
                    return result;
                }
                else if (this->m_string[i] == '/') {
                    ++result;
                }
            }
            return result;
        }
        qpl::filesys::path qpl::filesys::path::get_common_branch(const qpl::filesys::path& other) const {
            auto branches_a = this->get_branches();
            auto branches_b = other.get_branches();
            auto min = qpl::min(branches_a.size(), branches_b.size());

            qpl::size i = 0u;
            for (; i < min; ++i) {
                if (branches_a[i] != branches_b[i]) {
                    if (i == 0u) return "";
                    return branches_a[i - 1];
                }
            }
            if (i == 0u) return "";
            return branches_a[i - 1];
        }

        qpl::filesys::paths qpl::filesys::path::search_where_extension_equals(const std::string_view& extension) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_equals(extension)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_equals(const char* extension) const {
            return this->search_where_extension_equals(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_equals(const std::string& extension) const {
            return this->search_where_extension_equals(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_contains(const char* str) const {
            return this->search_where_extension_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_contains(const std::string& str) const {
            return this->search_where_extension_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_where_name_equals(const std::string_view& name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_equals(name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_equals(const char* name) const {
            return this->search_where_name_equals(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_equals(const std::string& name) const {
            return this->search_where_name_equals(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_contains(const char* str) const {
            return this->search_where_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_contains(const std::string& str) const {
            return this->search_where_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_where_file_name_equals(const std::string_view& file_name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_equals(file_name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_equals(const char* file_name) const {
            return this->search_where_file_name_equals(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_equals(const std::string& file_name) const {
            return this->search_where_file_name_equals(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_contains(const char* str) const {
            return this->search_where_file_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_contains(const std::string& str) const {
            return this->search_where_file_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_equal(const std::string_view& extension) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_equals(extension)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_equal(const char* extension) const {
            return this->search_where_extension_doesnt_equal(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_equal(const std::string& extension) const {
            return this->search_where_extension_doesnt_equal(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_contain(const char* str) const {
            return this->search_where_extension_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_contain(const std::string& str) const {
            return this->search_where_extension_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_extension_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_equal(const std::string_view& name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_equals(name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_equal(const char* name) const {
            return this->search_where_name_doesnt_equal(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_equal(const std::string& name) const {
            return this->search_where_name_doesnt_equal(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_contain(const char* str) const {
            return this->search_where_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_contain(const std::string& str) const {
            return this->search_where_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_name_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_equal(const std::string_view& file_name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_equals(file_name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_equal(const char* file_name) const {
            return this->search_where_file_name_doesnt_equal(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_equal(const std::string& file_name) const {
            return this->search_where_file_name_doesnt_equal(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_contain(const char* str) const {
            return this->search_where_file_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_contain(const std::string& str) const {
            return this->search_where_file_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_where_file_name_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }



        qpl::filesys::paths qpl::filesys::path::search_directories() const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.is_directory()) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_files() const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.is_file()) {
                    result.emplace_back(path);
                }
            }
            return result;
        }


        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_equals(const std::string_view& extension) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_equals(extension)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_equals(const char* extension) const {
            return this->search_recursively_where_extension_equals(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_equals(const std::string& extension) const {
            return this->search_recursively_where_extension_equals(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_contains(const char* str) const {
            return this->search_recursively_where_extension_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_contains(const std::string& str) const {
            return this->search_recursively_where_extension_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.extension_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_equals(const std::string_view& name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_equals(name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_equals(const char* name) const {
            return this->search_recursively_where_name_equals(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_equals(const std::string& name) const {
            return this->search_recursively_where_name_equals(std::string_view{ name });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_contains(const char* str) const {
            return this->search_recursively_where_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_contains(const std::string& str) const {
            return this->search_recursively_where_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_equals(const std::string_view& file_name) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_equals(file_name)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_equals(const char* file_name) const {
            return this->search_recursively_where_file_name_equals(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_equals(const std::string& file_name) const {
            return this->search_recursively_where_file_name_equals(std::string_view{ file_name });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_contains(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_contains(const char* str) const {
            return this->search_recursively_where_file_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_contains(const std::string& str) const {
            return this->search_recursively_where_file_name_contains(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_matches(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.file_name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_equal(const std::string_view& extension) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_equals(extension)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_equal(const char* extension) const {
            return this->search_recursively_where_extension_doesnt_equal(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_equal(const std::string& extension) const {
            return this->search_recursively_where_extension_doesnt_equal(std::string_view{ extension });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_contain(const char* str) const {
            return this->search_recursively_where_extension_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_contain(const std::string& str) const {
            return this->search_recursively_where_extension_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_extension_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.extension_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_equal(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_equals(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_equal(const char* str) const {
            return this->search_recursively_where_name_doesnt_equal(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_equal(const std::string& str) const {
            return this->search_recursively_where_name_doesnt_equal(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_contain(const char* str) const {
            return this->search_recursively_where_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_contain(const std::string& str) const {
            return this->search_recursively_where_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_name_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_equal(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_equals(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_equal(const char* str) const {
            return this->search_recursively_where_file_name_doesnt_equal(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_equal(const std::string& str) const {
            return this->search_recursively_where_file_name_doesnt_equal(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_contain(const std::string_view& str) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_contains(str)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_contain(const char* str) const {
            return this->search_recursively_where_file_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_contain(const std::string& str) const {
            return this->search_recursively_where_file_name_doesnt_contain(std::string_view{ str });
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_where_file_name_doesnt_match(const std::regex& regex) const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (!path.file_name_matches(regex)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }


        qpl::filesys::paths qpl::filesys::path::search_recursively_directories() const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.is_directory()) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        qpl::filesys::paths qpl::filesys::path::search_recursively_files() const {
            qpl::filesys::paths result;
            auto directory = this->is_directory() ? this->string() : this->get_parent_branch().string();
            for (auto i : std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)) {
                qpl::filesys::path path = i;
                if (path.is_file()) {
                    result.emplace_back(path);
                }
            }
            return result;
        }

        void qpl::filesys::path::construct() {
            this->m_is_file = false;
            this->m_string = "";
            this->m_update = true;
        }
        void qpl::filesys::path::check_update() const {
            if (this->m_update) {
                this->m_exists = std::filesystem::exists(this->string());

                if (this->empty()) {
                    this->m_is_file = false;
                    this->m_is_directory = false;
                }
                else if (!this->m_exists) {
                    this->m_is_file = this->m_string.back() != '/';
                    this->m_is_directory = !this->m_is_file;
                }
                else {
                    this->m_is_directory = std::filesystem::is_directory(this->string());
                    this->m_is_file = this->m_is_directory ? false : std::filesystem::is_regular_file(this->string()) || std::filesystem::is_block_file(this->string()) || std::filesystem::is_character_file(this->string());
                }
                this->m_update = false;
            }
        }

        std::ostream& qpl::filesys::operator<<(std::ostream& os, const qpl::filesys::path& path) {
            return os << path.string();
        }




        qpl::filesys::paths& qpl::filesys::paths::operator=(const std::vector<qpl::filesys::path>& paths) {
            this->m_paths = paths;
            return *this;
        }
        qpl::filesys::paths& qpl::filesys::paths::operator=(const qpl::filesys::paths& paths) {
            this->m_paths = paths.m_paths;
            return *this;
        }

        std::vector<qpl::filesys::path>::iterator qpl::filesys::paths::begin() {
            return this->m_paths.begin();
        }
        std::vector<qpl::filesys::path>::const_iterator qpl::filesys::paths::begin() const {
            return this->m_paths.cbegin();
        }
        std::vector<qpl::filesys::path>::const_iterator qpl::filesys::paths::cbegin() const {
            return this->m_paths.cbegin();
        }
        std::vector<qpl::filesys::path>::reverse_iterator qpl::filesys::paths::rbegin() {
            return this->m_paths.rbegin();
        }
        std::vector<qpl::filesys::path>::iterator qpl::filesys::paths::end() {
            return this->m_paths.end();
        }
        std::vector<qpl::filesys::path>::const_iterator qpl::filesys::paths::end() const {
            return this->m_paths.cend();
        }
        std::vector<qpl::filesys::path>::const_iterator qpl::filesys::paths::cend() const {
            return this->m_paths.cend();
        }
        std::vector<qpl::filesys::path>::reverse_iterator qpl::filesys::paths::rend() {
            return this->m_paths.rend();
        }

        void qpl::filesys::paths::copy_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::recursive);
            }
        }
        void qpl::filesys::paths::move_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::recursive);
                i.remove();
            }
        }
        void qpl::filesys::paths::copy_overwrite_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            }
        }
        void qpl::filesys::paths::move_overwrite_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                i.remove();
            }
        }

        void qpl::filesys::paths::copy_files_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                if (i.is_file()) {
                    std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::recursive);
                }
            }
        }
        void qpl::filesys::paths::move_files_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                if (i.is_file()) {
                    std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::recursive);
                    i.remove();
                }
            }
        }
        void qpl::filesys::paths::copy_overwrite_files_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                if (i.is_file()) {
                    std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                }
            }
        }
        void qpl::filesys::paths::move_overwrite_files_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            for (auto& i : this->m_paths) {
                if (i.is_file()) {
                    std::filesystem::copy(i.string(), destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                    i.remove();
                }
            }
        }

        void qpl::filesys::paths::copy_as_tree_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            qpl::filesys::path tree_destination = destination;
            auto levels = this->m_paths.front().branch_size() - 1;
            qpl::u32 depth_ctr = qpl::u32_cast(levels);

            for (auto& i : this->m_paths) {

                auto branch_size = i.branch_size();

                if (depth_ctr != branch_size) {
                    tree_destination = destination;
                    depth_ctr = qpl::u32_cast(levels);
                    for (qpl::size l = levels; l < i.branch_size(); ++l) {
                        tree_destination.go_into(i.get_branch_at(l).get_name());
                        ++depth_ctr;
                    }
                }


                if (i.is_directory()) {
                    tree_destination.create();
                }
                else if (i.is_file()) {
                    std::filesystem::copy(i.string(), tree_destination.string(), std::filesystem::copy_options::recursive);
                }
            }
        }
        void qpl::filesys::paths::move_as_tree_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            qpl::filesys::path tree_destination = destination;
            auto levels = this->m_paths.front().branch_size() - 1;
            qpl::size depth_ctr = levels;

            for (auto& i : this->m_paths) {

                auto branch_size = i.branch_size();

                if (depth_ctr != branch_size) {
                    tree_destination = destination;
                    depth_ctr = levels;
                    for (qpl::size l = levels; l < i.branch_size(); ++l) {
                        tree_destination.go_into(i.get_branch_at(l).get_name());
                        ++depth_ctr;
                    }
                }


                if (i.is_directory()) {
                    tree_destination.create();
                }
                else if (i.is_file()) {
                    std::filesystem::copy(i.string(), tree_destination.string(), std::filesystem::copy_options::recursive);
                    i.remove();
                }
            }
        }
        void qpl::filesys::paths::copy_overwrite_as_tree_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            qpl::filesys::path tree_destination = destination;
            auto levels = this->m_paths.front().branch_size() - 1;
            qpl::u32 depth_ctr = qpl::u32_cast(levels);

            for (auto& i : this->m_paths) {

                auto branch_size = i.branch_size();

                if (depth_ctr != branch_size) {
                    tree_destination = destination;
                    depth_ctr = qpl::u32_cast(levels);
                    for (qpl::size l = levels; l < i.branch_size(); ++l) {
                        tree_destination.go_into(i.get_branch_at(l).get_name());
                        ++depth_ctr;
                    }
                }


                if (i.is_directory()) {
                    tree_destination.create();
                }
                else if (i.is_file()) {
                    std::filesystem::copy(i.string(), tree_destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                }
            }
        }
        void qpl::filesys::paths::move_overwrite_as_tree_to(qpl::filesys::path destination) {
            if (destination.is_directory()) {
                if (!destination.exists()) {
                    destination.create();
                }
            }

            qpl::filesys::path tree_destination = destination;
            auto levels = this->m_paths.front().branch_size() - 1;
            auto depth_ctr = levels;

            for (auto& i : this->m_paths) {

                auto branch_size = i.branch_size();

                if (depth_ctr != branch_size) {
                    tree_destination = destination;
                    depth_ctr = levels;
                    for (qpl::size l = levels; l < i.branch_size(); ++l) {
                        tree_destination.go_into(i.get_branch_at(l).get_name());
                        ++depth_ctr;
                    }
                }


                if (i.is_directory()) {
                    tree_destination.create();
                }
                else if (i.is_file()) {
                    std::filesystem::copy(i.string(), tree_destination.string(), std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
                    i.remove();
                }
            }
        }

        qpl::size qpl::filesys::paths::size() const {
            return this->m_paths.size();
        }
        bool qpl::filesys::paths::empty() const {
            return this->m_paths.empty();
        }
        qpl::size qpl::filesys::paths::capacity() const {
            return this->m_paths.capacity();
        }
        void qpl::filesys::paths::resize(qpl::size new_size) {
            this->m_paths.resize(new_size);
        }
        void qpl::filesys::paths::reserve(qpl::size new_cap) {
            this->m_paths.reserve(new_cap);
        }
        void qpl::filesys::paths::clear() {
            this->m_paths.clear();
        }

        qpl::filesys::path& qpl::filesys::paths::front() {
            return this->m_paths.front();
        }
        const qpl::filesys::path& qpl::filesys::paths::front() const {
            return this->m_paths.front();
        }
        qpl::filesys::path& qpl::filesys::paths::back() {
            return this->m_paths.back();
        }
        const qpl::filesys::path& qpl::filesys::paths::back() const {
            return this->m_paths.back();
        }

        void qpl::filesys::paths::push_back(const qpl::filesys::path& path) {
            this->m_paths.push_back(path);
        }
        void qpl::filesys::paths::emplace_back(qpl::filesys::path& path) {
            this->m_paths.emplace_back(path);
        }
        void qpl::filesys::paths::pop_back() {
            this->m_paths.pop_back();
        }

        qpl::filesys::path& qpl::filesys::paths::operator[](qpl::size index) {
            return this->m_paths[index];
        }
        const qpl::filesys::path& qpl::filesys::paths::operator[](qpl::size index) const {
            return this->m_paths[index];
        }
        qpl::filesys::path& qpl::filesys::paths::at(qpl::size index) {
            return this->m_paths.at(index);
        }
        const qpl::filesys::path& qpl::filesys::paths::at(qpl::size index) const {
            return this->m_paths.at(index);
        }

        void qpl::filesys::paths::print_tree() const {
            if (this->m_paths.empty()) {
                return;
            }

            std::vector<std::string> branch_names = this->m_paths.front().get_branch_names();
            std::vector<std::string> current_branch_names;

            auto print_branch_space = [&](qpl::size stop) {
                for (qpl::size i = 0u; i < stop; ++i) {
                    auto width = branch_names[i].length();
                    qpl::print_repeat(' ', width + 1);
                    if (i == branch_names.size() - 1) {
                        qpl::print("|");
                    }
                    else {
                        qpl::print("|");
                    }
                }

            };

            auto print_branch_names = [&](qpl::u32 start) {
                for (qpl::u32 i = start; i < branch_names.size(); ++i) {
                    print_branch_space(i);

                    qpl::println("_ ", branch_names[i]);
                }
            };

            print_branch_names(0u);

            for (auto& path : this->m_paths) {
                current_branch_names = path.get_branch_names();

                bool not_equal = false;
                qpl::u32 i = 0u;
                for (; i < current_branch_names.size() && i < branch_names.size(); ++i) {
                    not_equal = !qpl::string_equals_ignore_case(current_branch_names[i], branch_names[i]);
                    if (not_equal) {
                        branch_names = current_branch_names;
                        print_branch_names(i);

                        if (path.is_file()) {
                            print_branch_space(branch_names.size());
                            qpl::println("_ ", path.get_full_name(true));
                        }
                        break;
                    }
                }

                if (!not_equal) {
                    branch_names = current_branch_names;
                    print_branch_space(branch_names.size());
                    qpl::println("_ ", path.get_full_name(true));
                }
            }
        }


        void qpl::filesys::paths::list_remove_where_lambda(const std::function<bool(qpl::filesys::path)>& check) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!check(i)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_keep_where_lambda(const std::function<bool(qpl::filesys::path)>& check) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (check(i)) {
                    this->emplace_back(i);
                }
            }
        }

        void qpl::filesys::paths::list_add(const qpl::filesys::path& path) {
            this->m_paths.push_back(path);
        }
        void qpl::filesys::paths::list_add_recursively(const qpl::filesys::path& path) {
            auto list = path.list_current_directory_tree();
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_add_directories(const qpl::filesys::path& directory) {
            auto list = directory.search_directories();
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_directories(const qpl::filesys::path& directory) {
            auto list = directory.search_recursively_directories();
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_remove_directories() {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.is_directory()) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_files(const qpl::filesys::path& directory) {
            auto list = directory.search_files();
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_files(const qpl::filesys::path& directory) {
            auto list = directory.search_recursively_files();
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_remove_files() {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.is_file()) {
                    this->emplace_back(i);
                }
            }
        }

        void qpl::filesys::paths::list_remove_where_directory_matches(const std::string& str) {

            qpl::size level = 0u;
            bool remove = false;


            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.get_name() == str) {
                    remove = true;
                    level = i.branch_size() - 1;
                }
                else if (remove) {
                    if (i.get_branch_at(level).get_name() != str) {
                        this->emplace_back(i);
                        remove = false;
                    }
                }
                else {
                    this->emplace_back(i);
                }
            }

            for (auto& i : this->m_paths) {
                if (i.get_name() == str) {

                }
            }
        }

        void qpl::filesys::paths::list_remove_where_extension_equals(const std::string& extension) {
            this->list_remove_where_extension_equals(std::string_view{ extension });
        }
        void qpl::filesys::paths::list_add_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension) {
            this->list_add_where_extension_equals(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension) {
            this->list_recursively_add_where_extension_equals(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_remove_where_extension_contains(const std::string& str) {
            this->list_remove_where_extension_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_extension_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_add_where_extension_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_extension_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_remove_where_extension_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.extension_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_extension_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_extension_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_remove_where_name_equals(const std::string& name) {
            this->list_remove_where_name_equals(std::string_view{ name });
        }
        void qpl::filesys::paths::list_add_where_name_equals(const qpl::filesys::path& directory, const std::string& name) {
            this->list_add_where_name_equals(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const std::string& name) {
            this->list_recursively_add_where_name_equals(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_remove_where_name_contains(const std::string& str) {
            this->list_remove_where_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_name_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_add_where_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_remove_where_name_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.name_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_name_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_name_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_remove_where_file_name_equals(const std::string& file_name) {
            this->list_remove_where_file_name_equals(std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string& file_name) {
            this->list_add_where_file_name_equals(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string& file_name) {
            this->list_recursively_add_where_file_name_equals(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_remove_where_file_name_contains(const std::string& str) {
            this->list_remove_where_file_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_add_where_file_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_file_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_remove_where_file_name_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.file_name_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_file_name_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_matches(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_file_name_matches(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }


        void qpl::filesys::paths::list_keep_where_extension_equals(const std::string& extension) {
            this->list_keep_where_extension_equals(std::string_view{ extension });
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension) {
            this->list_add_where_extension_doesnt_equal(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension) {
            this->list_recursively_add_where_extension_doesnt_equal(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_keep_where_extension_contains(const std::string& str) {
            this->list_keep_where_extension_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_add_where_extension_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_extension_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_keep_where_extension_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.extension_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_extension_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_extension_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_keep_where_name_equals(const std::string& name) {
            this->list_keep_where_name_equals(std::string_view{ name });
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name) {
            this->list_add_where_name_doesnt_equal(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name) {
            this->list_recursively_add_where_name_doesnt_equal(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_keep_where_name_contains(const std::string& str) {
            this->list_keep_where_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_keep_where_name_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.name_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_name_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_name_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_keep_where_file_name_equals(const std::string& file_name) {
            this->list_keep_where_file_name_equals(std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& file_name) {
            this->list_add_where_file_name_doesnt_equal(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& file_name) {
            this->list_recursively_add_where_file_name_doesnt_equal(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_keep_where_file_name_contains(const std::string& str) {
            this->list_keep_where_file_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_file_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str) {
            this->list_recursively_add_where_file_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_keep_where_file_name_matches(const std::regex& regex) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.file_name_matches(regex)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_where_file_name_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex) {
            auto list = directory.search_recursively_where_file_name_doesnt_match(regex);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }


        void qpl::filesys::paths::list_remove_where_extension_equals(const std::string_view& extension) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.extension_equals(extension)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_equals(const qpl::filesys::path& directory, const std::string_view& extension) {
            auto list = directory.search_where_extension_equals(extension);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const std::string_view& extension) {
            auto list = directory.search_recursively_where_extension_equals(extension);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_remove_where_extension_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.extension_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_extension_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_extension_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_remove_where_name_equals(const std::string_view& name) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.name_equals(name)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_equals(const qpl::filesys::path& directory, const std::string_view& name) {
            auto list = directory.search_where_name_equals(name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const std::string_view& name) {
            auto list = directory.search_recursively_where_name_equals(name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_remove_where_name_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.name_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_name_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_name_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_remove_where_file_name_equals(const std::string_view& file_name) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.file_name_equals(file_name)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string_view& file_name) {
            auto list = directory.search_where_file_name_equals(file_name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string_view& file_name) {
            auto list = directory.search_recursively_where_file_name_equals(file_name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_remove_where_file_name_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (!i.file_name_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_file_name_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_file_name_contains(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }


        void qpl::filesys::paths::list_keep_where_extension_equals(const std::string_view& extension) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.extension_equals(extension)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& extension) {
            auto list = directory.search_where_extension_doesnt_equal(extension);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& extension) {
            auto list = directory.search_recursively_where_extension_doesnt_equal(extension);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_keep_where_extension_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.extension_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_extension_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_extension_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_keep_where_name_equals(const std::string_view& name) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.name_equals(name)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& name) {
            auto list = directory.search_where_name_doesnt_equal(name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& name) {
            auto list = directory.search_recursively_where_name_doesnt_equal(name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_keep_where_name_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.name_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_name_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_name_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }

        void qpl::filesys::paths::list_keep_where_file_name_equals(const std::string_view& file_name) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.file_name_equals(file_name)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& file_name) {
            auto list = directory.search_where_file_name_doesnt_equal(file_name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& file_name) {
            auto list = directory.search_recursively_where_file_name_doesnt_equal(file_name);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_keep_where_file_name_contains(const std::string_view& str) {
            auto copy = this->m_paths;
            this->clear();
            for (auto& i : copy) {
                if (i.file_name_contains(str)) {
                    this->emplace_back(i);
                }
            }
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_where_file_name_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str) {
            auto list = directory.search_recursively_where_file_name_doesnt_contain(str);
            this->m_paths.insert(this->m_paths.end(), list.m_paths.begin(), list.m_paths.end());
        }


        void qpl::filesys::paths::list_remove_where_extension_equals(const char* extension) {
            this->list_remove_where_extension_equals(std::string_view{ extension });
        }
        void qpl::filesys::paths::list_add_where_extension_equals(const qpl::filesys::path& directory, const char* extension) {
            this->list_add_where_extension_equals(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const char* extension) {
            this->list_recursively_add_where_extension_equals(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_remove_where_extension_contains(const char* str) {
            this->list_remove_where_extension_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_extension_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_add_where_extension_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_extension_contains(directory, std::string_view{ str });
        }

        void qpl::filesys::paths::list_remove_where_name_equals(const char* name) {
            this->list_remove_where_name_equals(std::string_view{ name });
        }
        void qpl::filesys::paths::list_add_where_name_equals(const qpl::filesys::path& directory, const char* name) {
            this->list_add_where_name_equals(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const char* name) {
            this->list_recursively_add_where_name_equals(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_remove_where_name_contains(const char* str) {
            this->list_remove_where_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_name_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_add_where_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_name_contains(directory, std::string_view{ str });
        }

        void qpl::filesys::paths::list_remove_where_file_name_equals(const char* file_name) {
            this->list_remove_where_file_name_equals(std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_add_where_file_name_equals(const qpl::filesys::path& directory, const char* file_name) {
            this->list_add_where_file_name_equals(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const char* file_name) {
            this->list_recursively_add_where_file_name_equals(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_remove_where_file_name_contains(const char* str) {
            this->list_remove_where_file_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_file_name_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_add_where_file_name_contains(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_file_name_contains(directory, std::string_view{ str });
        }


        void qpl::filesys::paths::list_keep_where_extension_equals(const char* extension) {
            this->list_keep_where_extension_equals(std::string_view{ extension });
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const char* extension) {
            this->list_add_where_extension_doesnt_equal(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const char* extension) {
            this->list_recursively_add_where_extension_doesnt_equal(directory, std::string_view{ extension });
        }
        void qpl::filesys::paths::list_keep_where_extension_contains(const char* str) {
            this->list_keep_where_extension_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_add_where_extension_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_extension_doesnt_contain(directory, std::string_view{ str });
        }

        void qpl::filesys::paths::list_keep_where_name_equals(const char* name) {
            this->list_keep_where_name_equals(std::string_view{ name });
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const char* name) {
            this->list_add_where_name_doesnt_equal(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const char* name) {
            this->list_recursively_add_where_name_doesnt_equal(directory, std::string_view{ name });
        }
        void qpl::filesys::paths::list_keep_where_name_contains(const char* str) {
            this->list_keep_where_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_name_doesnt_contain(directory, std::string_view{ str });
        }

        void qpl::filesys::paths::list_keep_where_file_name_equals(const char* file_name) {
            this->list_keep_where_file_name_equals(std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const char* file_name) {
            this->list_add_where_file_name_doesnt_equal(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const char* file_name) {
            this->list_recursively_add_where_file_name_doesnt_equal(directory, std::string_view{ file_name });
        }
        void qpl::filesys::paths::list_keep_where_file_name_contains(const char* str) {
            this->list_keep_where_file_name_contains(std::string_view{ str });
        }
        void qpl::filesys::paths::list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_file_name_doesnt_contain(directory, std::string_view{ str });
        }
        void qpl::filesys::paths::list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const char* str) {
            this->list_recursively_add_where_file_name_doesnt_contain(directory, std::string_view{ str });
        }


        bool qpl::filesys::file_content_equals(const qpl::filesys::path& path1, const qpl::filesys::path& path2) {
            return path1.file_content_equals(path2);
        }
        qpl::size qpl::filesys::file_lines(const qpl::filesys::path& path) {
            std::stringstream str;
            str << qpl::filesys::read_file(path).data();

            std::string line;
            qpl::size ctr = 0u;
            while (std::getline(str, line)) {
                ++ctr;
            }
            return ctr;
        }

        qpl::size qpl::filesys::file_bytes(const qpl::filesys::path& path) {
            std::stringstream str;
            str << qpl::filesys::read_file(path).data();

            return str.str().size();
        }

        std::vector<qpl::size> qpl::filesys::file_line_differences(const qpl::filesys::path& path1, const qpl::filesys::path& path2) {
            std::vector<qpl::size> result;
            std::stringstream str1;
            str1 << qpl::filesys::read_file(path1).data();
            std::stringstream str2;
            str2 << qpl::filesys::read_file(path2).data();

            std::string line1, line2;
            qpl::size ctr = 0u;
            while (std::getline(str1, line1) && std::getline(str2, line2)) {
                if (line1 != line2) {
                    result.push_back(ctr);
                }
                ++ctr;
            }
            return result;
        }
        qpl::f64 qpl::filesys::file_lines_difference_percentage(const qpl::filesys::path& path1, const qpl::filesys::path& path2) {
            std::stringstream str1;
            str1 << qpl::filesys::read_file(path1).data();
            std::stringstream str2;
            str2 << qpl::filesys::read_file(path2).data();

            std::string line1, line2;
            qpl::size ctr = 0u;
            qpl::size sum = 0u;
            while (std::getline(str1, line1) && std::getline(str2, line2)) {
                if (line1 != line2) {
                    ++ctr;
                }
                ++sum;
            }
            return static_cast<qpl::f64>(ctr) / sum;
        }

        void qpl::filesys::split_file(const qpl::filesys::path& path, qpl::u32 bytes) {
            auto content = qpl::filesys::read_file(path);
            auto splits = (content.size() - 1) / bytes + 1;

            for (qpl::u32 i = 0u; i < splits; ++i) {
                auto file_name = qpl::to_string(path.string(), ".PART", qpl::prepended_to_string_to_fit(i, '0', qpl::size_cast(std::log(splits) / std::log(10) + 1)));
                std::ofstream file(file_name.c_str(), std::ios::binary);
                if (!file.good()) {
                    qpl::println("there was a problem creating \"", file_name, "\"");
                }
                file << content.substr(i * bytes, bytes);
                file.flush();
            }
        }
        void qpl::filesys::combine_files(const qpl::filesys::paths& paths, const qpl::filesys::path& destination) {
            std::ofstream file(destination.c_str(), std::ios::binary);
            if (!file.good()) {
                qpl::println("there was a problem creating \"", destination.string(), "\"");
            }
            for (auto& i : paths) {
               file << i.read();
            }
            file.flush();
        }

        qpl::filesys::paths qpl::filesys::list_directory(const qpl::filesys::path& path) {
            return path.list_current_directory();
        }
        qpl::filesys::paths qpl::filesys::list_current_directory() {
            return qpl::filesys::list_directory(qpl::filesys::get_current_location());
        }
        qpl::filesys::paths qpl::filesys::list_directory_tree(const qpl::filesys::path& path) {
            return path.list_current_directory_tree();
        }
        qpl::filesys::paths qpl::filesys::list_current_directory_tree() {
            return qpl::filesys::list_directory_tree(qpl::filesys::get_current_location());
        }
        void qpl::filesys::print_current_tree() {
            auto p = qpl::filesys::list_current_directory_tree();
            p.print_tree();
        }
        void qpl::filesys::print_tree(qpl::filesys::path path) {
            auto p = qpl::filesys::list_directory_tree(path);
            p.print_tree();
        }

        void qpl::filesys::cd(qpl::filesys::path& path, const qpl::filesys::path& target) {
            path.go_into_directory(target);
        }
        void qpl::filesys::create(const qpl::filesys::path& path) {
            path.create();
        }
        bool qpl::filesys::last_write_time_equal(const qpl::filesys::path& path1, const qpl::filesys::path& path2) {
            if (!path1.exists() || !path2.exists()) {
                return false;
            }
            return path1.last_write_time() == path2.last_write_time();
        }


        bool qpl::filesys::is_file(const qpl::filesys::path& path) {
            return path.is_file();
        }
        bool qpl::filesys::is_directory(const qpl::filesys::path& path) {
            return path.is_directory();
        }
        bool qpl::filesys::has_extension(const qpl::filesys::path& path) {
            return path.has_extension();
        }

        std::string qpl::filesys::get_name(const qpl::filesys::path& path) {
            return path.get_name();
        }
        std::string qpl::filesys::get_file_extension(const qpl::filesys::path& path) {
            return path.get_file_extension();
        }
        std::string qpl::filesys::get_file_name(const qpl::filesys::path& path) {
            return path.get_file_name();
        }        
        std::string qpl::filesys::get_full_file_name(const qpl::filesys::path& path) {
            return path.get_full_file_name();
        }

        std::string_view get_name_view(const qpl::filesys::path& path) {
            return path.get_name_view();
        }
        std::string_view get_file_extension_view(const qpl::filesys::path& path) {
            return path.get_file_extension_view();
        }
        std::string_view get_file_name_view(const qpl::filesys::path& path) {
            return path.get_file_name_view();
        }
        std::string_view get_full_file_name_view(const qpl::filesys::path& path) {
            return path.get_full_file_name_view();
        }
        qpl::filesys::path qpl::filesys::get_parent_branch(const qpl::filesys::path& path) {
            return path.get_parent_branch();
        }

        bool qpl::filesys::extension_equals(const qpl::filesys::path& path, const std::string_view& str) {
            return path.extension_equals(str);
        }
        bool qpl::filesys::extension_equals(const qpl::filesys::path& path, const std::string& str) {
            return path.extension_equals(str);
        }
        bool qpl::filesys::extension_contains(const qpl::filesys::path& path, const std::string_view& str) {
            return path.extension_contains(str);
        }
        bool qpl::filesys::extension_contains(const qpl::filesys::path& path, const std::string& str) {
            return path.extension_contains(str);
        }
        bool qpl::filesys::extension_matches(const qpl::filesys::path& path, const std::string& regex) {
            return path.extension_matches(regex);
        }
        bool qpl::filesys::extension_matches(const qpl::filesys::path& path, const std::regex& regex) {
            return path.extension_matches(regex);
        }
        bool qpl::filesys::file_name_equals(const qpl::filesys::path& path, const std::string_view& str) {
            return path.file_name_equals(str);
        }
        bool qpl::filesys::file_name_equals(const qpl::filesys::path& path, const std::string& str) {
            return path.file_name_equals(str);
        }
        bool qpl::filesys::file_name_contains(const qpl::filesys::path& path, const std::string_view& str) {
            return path.file_name_contains(str);
        }
        bool qpl::filesys::file_name_contains(const qpl::filesys::path& path, const std::string& str) {
            return path.file_name_contains(str);
        }
        bool qpl::filesys::file_name_matches(const qpl::filesys::path& path, const std::string& regex) {
            return path.file_name_matches(regex);
        }
        bool qpl::filesys::file_name_matches(const qpl::filesys::path& path, const std::regex& regex) {
            return path.file_name_matches(regex);
        }

        bool qpl::filesys::full_file_name_equals(const qpl::filesys::path& path, const std::string_view& str) {
            return path.full_file_name_equals(str);
        }
        bool qpl::filesys::full_file_name_equals(const qpl::filesys::path& path, const std::string& str) {
            return path.full_file_name_equals(str);
        }
        bool qpl::filesys::full_file_name_contains(const qpl::filesys::path& path, const std::string_view& str) {
            return path.full_file_name_contains(str);
        }
        bool qpl::filesys::full_file_name_contains(const qpl::filesys::path& path, const std::string& str) {
            return path.full_file_name_contains(str);
        }
        bool qpl::filesys::full_file_name_matches(const qpl::filesys::path& path, const std::string& regex) {
            return path.full_file_name_matches(regex);
        }
        bool qpl::filesys::full_file_name_matches(const qpl::filesys::path& path, const std::regex& regex) {
            return path.full_file_name_matches(regex);
        }

        bool qpl::filesys::name_equals(const qpl::filesys::path& path, const std::string_view& str) {
            return path.name_equals(str);
        }
        bool qpl::filesys::name_equals(const qpl::filesys::path& path, const std::string& str) {
            return path.name_equals(str);
        }
        bool qpl::filesys::name_contains(const qpl::filesys::path& path, const std::string_view& str) {
            return path.name_contains(str);
        }
        bool qpl::filesys::name_contains(const qpl::filesys::path& path, const std::string& str) {
            return path.name_contains(str);
        }
        bool qpl::filesys::name_matches(const qpl::filesys::path& path, const std::string& regex) {
            return path.name_matches(regex);
        }
        bool qpl::filesys::name_matches(const qpl::filesys::path& path, const std::regex& regex) {
            return path.name_matches(regex);
        }

        bool qpl::filesys::exists(const qpl::filesys::path& path_source) {
            return path_source.exists();
        }
        void qpl::filesys::rename(qpl::filesys::path& path_source, const qpl::filesys::path& new_name) {
            path_source.rename(new_name);
        }
        void qpl::filesys::rename(const qpl::filesys::path& path_source, const qpl::filesys::path& new_name) {
            std::filesystem::rename(path_source.string(), new_name.string());
        }
        void qpl::filesys::remove(const qpl::filesys::path& path_target) {
            path_target.remove();
        }
        void qpl::filesys::copy(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination) {
            path_source.copy(path_destination);
        }
        void qpl::filesys::copy_overwrite(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination) {
            path_source.copy_overwrite(path_destination);
        }
        void qpl::filesys::move(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination) {
            path_source.move(path_destination);
        }
        void qpl::filesys::move_overwrite(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination) {
            path_source.move_overwrite(path_destination);
        }

        void qpl::filesys::partially_rename_all(qpl::filesys::paths& files, const std::string& regex, const std::string& replace) {
            const std::regex reg{ regex };
            auto path = files.front().get_parent_branch();
            for (auto& i : files) {
                if (std::regex_search(i.get_name(), reg)) {
                    i.rename(path.string() + std::regex_replace(i.get_name(), reg, replace));
                }
            }
        }
        void partially_rename_all(qpl::filesys::path& path, const std::string& regex, const std::string& replace) {
            auto files = qpl::filesys::list_directory(path);
            const std::regex reg{ regex };
            for (auto& i : files) {
                if (std::regex_search(i.get_name(), reg)) {
                    i.rename(path.string() + std::regex_replace(i.get_name(), reg, replace));
                }
            }
        }
        void qpl::filesys::copy_unpack_directory(const qpl::filesys::path& path) {
            if (!path.is_directory()) {
                return;
            }
            auto cd = path;
            cd.go_directory_back();
            auto l = path.list_current_directory();
            for (auto& i : l) {
                i.copy(cd);
            }
        }
        void qpl::filesys::move_unpack_directory(const qpl::filesys::path& path) {
            if (!path.is_directory()) {
                return;
            }
            auto cd = path;
            cd.go_directory_back();
            auto l = path.list_current_directory();
            for (auto& i : l) {
                i.move(cd);
            }
            path.remove();
        }

        qpl::filesys::paths qpl::filesys::search_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension) {
            return directory.search_where_extension_equals(extension);
        }
        qpl::filesys::paths qpl::filesys::search_where_extension_contains(const qpl::filesys::path& directory, const std::string& regex) {
            return directory.search_where_extension_contains(regex);
        }
        qpl::filesys::paths qpl::filesys::search_where_name_equals(const qpl::filesys::path& directory, const std::string& name) {
            return directory.search_where_name_equals(name);
        }
        qpl::filesys::paths qpl::filesys::search_where_name_contains(const qpl::filesys::path& directory, const std::string& regex) {
            return directory.search_where_name_contains(regex);
        }

        qpl::filesys::paths qpl::filesys::search_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension) {
            return directory.search_where_extension_doesnt_equal(extension);
        }
        qpl::filesys::paths qpl::filesys::search_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& regex) {
            return directory.search_where_extension_doesnt_contain(regex);
        }
        qpl::filesys::paths qpl::filesys::search_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name) {
            return directory.search_where_name_doesnt_equal(name);
        }
        qpl::filesys::paths qpl::filesys::search_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& regex) {
            return directory.search_where_name_doesnt_contain(regex);
        }


        void qpl::filesys::remove_all(const qpl::filesys::paths& files) {
            for (auto& i : files) {
                i.remove();
            }
        }

        void qpl::filesys::move_all(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                i.move(path_destination);
            }
        }
        void qpl::filesys::copy_all(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                i.copy(path_destination);
            }
        }
        void qpl::filesys::move_all_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                i.move_overwrite(path_destination);
            }
        }
        void qpl::filesys::copy_all_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                qpl::println(i);
            }
        }

        void qpl::filesys::move_all_files(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                if (i.is_file()) {
                    i.move(path_destination);
                }
            }
        }
        void qpl::filesys::copy_all_files(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                if (i.is_file()) {
                    i.copy(path_destination);
                }
            }
        }
        void qpl::filesys::move_all_files_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                if (i.is_file()) {
                    i.move_overwrite(path_destination);
                }
            }
        }
        void qpl::filesys::copy_all_files_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination) {
            for (auto& i : files) {
                if (i.is_file()) {
                    i.copy_overwrite(path_destination);
                }
            }
        }



        std::string qpl::filesys::read_file(const std::string& path) {
            std::ifstream file(path, std::ios::ate | std::ios::binary);

            if (!file.is_open()) {
                throw std::runtime_error(qpl::to_string("failed to open file \"", path, "\"").c_str());
            }

            auto file_size = (size_t)file.tellg();
            std::string buffer;
            buffer.resize(file_size);

            file.seekg(0);
            file.read(buffer.data(), file_size);

            file.close();

            return buffer;
        }
        std::string read_rest_of_file(std::ifstream& file, bool close_file) {
            if (!file.is_open()) {
                return "";
            }

            auto file_pos = (size_t)file.tellg();
            file.seekg(0, std::ios::end);
            auto file_size = ((size_t)file.tellg() - file_pos);

            std::string buffer;
            buffer.resize(file_size);

            file.seekg(file_pos);
            file.read(buffer.data(), file_size);

            if (close_file) {
                file.close();
            }

            return buffer;
        }
        std::filesystem::file_time_type qpl::filesys::file_last_write_time(const std::string& path) {
            qpl::filesys::path p = path;
            return p.last_write_time();
        }
        std::string qpl::filesys::file_last_write_time_str(const std::string& path) {
            return qpl::get_time_string(qpl::filesys::file_last_write_time(path));
        }

        void qpl::filesys::write_to_file(const std::string& text, const std::string& path) {
            std::ofstream file(path, std::ios::app);
            file << text;
            file.close();
        }
        void qpl::filesys::writeln_to_file(const std::string& text, const std::string& path) {
            std::ofstream file(path, std::ios::app);
            file << text << '\n';
            file.close();
        }
        void qpl::filesys::write_data_file(const std::string& data, const std::string& path) {
            std::ofstream file(path, std::ios::binary);
            file << data;
            file.close();
        }

        qpl::filesys::path qpl::filesys::get_current_location() {
            return qpl::filesys::path::current_path();
        }

        std::string qpl::filesys::file_encrypt(const std::string& path, const std::string& key, qpl::aes::mode mode) {
            return qpl::encrypted_keep_size(qpl::filesys::read_file(path), key, mode);
        }
        void qpl::filesys::file_encrypt_to(const std::string& source_path, const std::string& dest_path, const std::string& key, qpl::aes::mode mode) {
            auto content = qpl::filesys::file_encrypt(source_path, key, mode);
            qpl::filesys::write_data_file(content, dest_path);
        }
        std::string qpl::filesys::file_decrypt(const std::string& path, const std::string& key, qpl::aes::mode mode) {
            return qpl::decrypted_keep_size(qpl::filesys::read_file(path), key, mode);
        }
        void qpl::filesys::file_decrypt_to(const std::string& source_path, const std::string& dest_path, const std::string& key, qpl::aes::mode mode) {
            auto content = qpl::filesys::file_decrypt(source_path, key, mode);
            qpl::filesys::write_data_file(content, dest_path);
        }
    }

    std::string qpl::read_file(const std::string& path) {
        return qpl::filesys::read_file(path);
    }
    void qpl::write_to_file(const std::string& text, const std::string& path) {
        return qpl::filesys::write_to_file(text, path);
    }
    void qpl::write_data_file(const std::string& data, const std::string& path) {
        return qpl::filesys::write_data_file(data, path);
    }

    void qpl::file_encrypter::clear() {
        this->paths.clear();
        this->additions = 0u;
        this->part_paths.clear();
    }
    void qpl::file_encrypter::add(std::string path) {
        auto file_path = qpl::filesys::path(path);

        auto adding = file_path.get_file_extension().starts_with(this->keyword_string_part);

        if (adding) {
            if (!this->adding_parts) {
                ++this->additions;
            }
            auto name = file_path.get_file_name();
            this->part_paths[name].push_back(file_path);
        }
        else {
            this->paths.push_back(path);
            ++this->additions;
        }
        this->adding_parts = adding;

        if (this->common_branch.empty()) {
            this->common_branch = path;
        }
        else {
            this->common_branch = this->common_branch.get_common_branch(path);
        }

        if (file_path.is_file()) {
            return;
        }

        auto files = file_path.list_current_directory_tree();
        for (auto& i : files) {
            if (i.is_directory()) {
                if (i.string().back() != '/') {
                    i.append("/");
                }
            }
            this->paths.push_back(i);

            if (this->common_branch.empty()) {
                this->common_branch = i;
            }
            else {
                this->common_branch = this->common_branch.get_common_branch(i);
            }
        }
    }
    std::string qpl::file_encrypter::encrypted_string(const std::string& key, qpl::aes::mode mode) {
        if (this->paths.empty()) {
            return "";
        }
        auto common_size = this->common_branch.branch_size() - 1;

        qpl::save_state save_state;
        save_state.save(this->common_branch.string());
        save_state.save(this->paths.size());

        for (auto& i : this->paths) {
            save_state.save(i.string());
            save_state.save(i.is_file());
        }

        for (auto& i : this->paths) {
            if (i.is_file()) {
                save_state.save(i.read());
            }
        }

        this->clear();
        auto str = save_state.get_finalized_string();
        str = qpl::encrypted_keep_size(str, key, mode);
        return str;
    }
    qpl::filesys::paths qpl::file_encrypter::encrypt(const std::string& key, std::string output_name, qpl::aes::mode mode, qpl::filesys::path destination_path, qpl::size split_size) {
        if (!destination_path.empty() && destination_path.string().back() != '/') {
            destination_path.append("/");
        }

        auto str = this->encrypted_string(key, mode);

        qpl::filesys::path encrypted_path = qpl::to_string(destination_path, output_name, '.', this->keyword_string_enrypted);

        qpl::size ctr = 0u;
        while (encrypted_path.exists()) {
            encrypted_path = qpl::to_string(destination_path, output_name, '.', this->keyword_string_enrypted, ctr);
            ++ctr;
        }
        auto splits = qpl::split_string_every(str, split_size);
        if (splits.size() > 1) {

            auto log = std::log10(splits.size() - 1) + 1;
            for (qpl::size i = 0u; i < splits.size(); ++i) {
                auto part_string = qpl::to_string(".", this->keyword_string_part, qpl::prepended_to_string_to_fit(qpl::to_string(i), '0', qpl::size_cast(log)));

                auto path = encrypted_path;
                path.append(part_string);

                while (path.exists()) {
                    encrypted_path = qpl::to_string(destination_path, output_name, '.', this->keyword_string_enrypted, ctr);
                    path = encrypted_path;
                    path.append(part_string);
                    ++ctr;
                }

                path.write(splits[i]);
            }
        }
        else {
            encrypted_path.write(str);
        }

        return this->paths;
    }
    qpl::filesys::paths qpl::file_encrypter::decrypt(const std::string& key, qpl::aes::mode mode, qpl::filesys::path destination_path) const {
        if (!destination_path.empty() && destination_path.string().back() != '/') {
            destination_path.append("/");
        }
        qpl::filesys::paths tree;
        for (auto& path : this->paths) {
            auto string = path.read();
            this->internal_decrypt(string, key, mode, destination_path, tree);
        }
        for (auto& part : this->part_paths) {
            std::string string;

            const auto& paths = part.second;
            std::vector<std::pair<qpl::filesys::path, qpl::size>> sorted_parts(paths.size());
            for (qpl::size i = 0u; i < sorted_parts.size(); ++i) {
                auto n = qpl::size_cast(paths[i].get_file_extension().substr(this->keyword_string_part.length()));
                sorted_parts[i] = std::make_pair(paths[i], n);
            }
            qpl::sort(sorted_parts, [](const auto& a, const auto& b) {
                return a.second < b.second;
                });

            auto p = sorted_parts.size() == 1u ? "part" : "parts";
            for (auto& file : sorted_parts) {
                string += file.first.read();
            }
            this->internal_decrypt(string, key, mode, destination_path, tree);
        }

        return tree;
    }

    void qpl::file_encrypter::internal_decrypt(const std::string& string, const std::string& key, qpl::aes::mode mode, qpl::filesys::path destination_path, qpl::filesys::paths& tree) const {
        auto str = qpl::decrypted_keep_size(string, key, mode);

        qpl::save_state load_state;
        std::string s;
        load_state.set_string(str);
        load_state.load(s);
        qpl::filesys::path common = s;

        qpl::size size;
        load_state.load(size);
        qpl::filesys::paths loaded_paths;
        loaded_paths.resize(size);

        std::unordered_set<std::string> is_file_hash;
        for (auto& i : loaded_paths) {
            std::string path_str;
            load_state.load(path_str);
            bool is_file;
            load_state.load(is_file);
            if (is_file) {
                is_file_hash.insert(path_str);
            }
            i = path_str;
        }

        std::string branch_name;
        auto split = qpl::split_string(common.get_full_name(), '.');

        branch_name = common.get_full_name();
        if (split.size() && split.back() == this->keyword_string_enrypted) {
            split.pop_back();
            branch_name = qpl::to_string_format("a.b", split);
        }
        branch_name += qpl::to_string('.', this->keyword_string_derypted);
        auto original_branch_name = branch_name;

        qpl::filesys::path decrypted_path = qpl::to_string(destination_path, branch_name, "/");

        qpl::size ctr = 0u;
        while (decrypted_path.exists()) {
            branch_name = qpl::to_string(original_branch_name, ctr);
            decrypted_path = qpl::to_string(destination_path, branch_name, "/");
            ++ctr;
        }

        ctr = 0u;
        for (auto& i : loaded_paths) {
            decrypted_path = i;
            decrypted_path.set_branch(common.branch_size() - 1, branch_name);
            decrypted_path = qpl::to_string(destination_path, branch_name, "/", decrypted_path.subpath(common.branch_size() - 1));

            decrypted_path.ensure_branches_exist();

            bool is_file = is_file_hash.find(i) != is_file_hash.cend();
            if (is_file) {
                std::string data_str;
                load_state.load(data_str);
                decrypted_path.write(data_str);
            }
            tree.push_back(decrypted_path);
        }
    }
}