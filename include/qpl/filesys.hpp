#ifndef QPL_FILESYS_HPP
#define QPL_FILESYS_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/memory.hpp>
#include <qpl/encryption.hpp>
#include <string>

#include <filesystem>
#include <functional>
#include <regex>
#include <fstream>

namespace qpl {

    namespace filesys {
        class paths;

        class path {
        public:

            path() {
                this->construct();
            }
            path(const std::string& str) {
                this->construct();
                *this = str;
            }
            path(const std::string_view& str) {
                this->construct();
                *this = str;
            }
            path(const char* str) {
                this->construct();
                *this = str;
            }
            path(const qpl::filesys::path& other) {
                *this = other;
            }
            path(const std::filesystem::directory_entry& entry) {
                *this = entry;
            }

            QPLDLL path& operator=(const std::string& str);
            QPLDLL path& operator=(const std::string_view& str);
            QPLDLL path& operator=(const char* str);
            QPLDLL path& operator=(const path& other);
            QPLDLL path& operator=(const std::filesystem::directory_entry& entry);

            QPLDLL bool operator==(const path& other) const;
            QPLDLL bool file_content_equals(const path& other) const;

            QPLDLL static qpl::filesys::path current_path();
            QPLDLL void set_current_path();

            QPLDLL const char* data() const;
            QPLDLL const char* c_str() const;
            QPLDLL std::string string() const;
            QPLDLL operator std::string() const;
            QPLDLL bool empty() const;

            QPLDLL void create() const;
            QPLDLL qpl::filesys::path make_file(std::string file_name) const;
            QPLDLL std::string read() const;
            QPLDLL void write(const std::string& data);

            QPLDLL std::filesystem::file_time_type last_write_time() const;
            QPLDLL qpl::u64 file_size() const;
            QPLDLL qpl::u64 file_size_recursive() const;

            QPLDLL bool exists() const;
            QPLDLL bool exists_system() const;
            QPLDLL bool is_file() const;
            QPLDLL bool is_directory() const;

            QPLDLL bool has_extension() const;

            QPLDLL std::string get_name() const;
            QPLDLL std::string get_full_name(bool add_slash_on_directory = false) const;
            QPLDLL std::string get_file_extension() const;
            QPLDLL std::string get_full_file_name() const;
            QPLDLL std::string get_file_name() const;
            QPLDLL std::string get_directory_name() const;

            QPLDLL std::string_view get_name_view() const;
            QPLDLL std::string_view get_file_extension_view() const;
            QPLDLL std::string_view get_full_file_name_view() const;
            QPLDLL std::string_view get_file_name_view() const;
            QPLDLL std::string_view get_directory_name_view() const;

            QPLDLL bool extension_equals(const std::string_view& str) const;
            QPLDLL bool extension_equals(const char* str) const;
            QPLDLL bool extension_equals(const std::string& str) const;
            QPLDLL bool extension_contains(const std::string_view& str) const;
            QPLDLL bool extension_contains(const char* str) const;
            QPLDLL bool extension_contains(const std::string& str) const;
            QPLDLL bool extension_matches(const std::string& regex) const;
            QPLDLL bool extension_matches(const std::regex& regex) const;

            QPLDLL bool file_name_equals(const std::string_view& str) const;
            QPLDLL bool file_name_equals(const char* str) const;
            QPLDLL bool file_name_equals(const std::string& str) const;
            QPLDLL bool file_name_contains(const std::string_view& str) const;
            QPLDLL bool file_name_contains(const char* str) const;
            QPLDLL bool file_name_contains(const std::string& str) const;
            QPLDLL bool file_name_matches(const std::string& regex) const;
            QPLDLL bool file_name_matches(const std::regex& regex) const;

            QPLDLL bool full_file_name_equals(const std::string_view& str) const;
            QPLDLL bool full_file_name_equals(const char* str) const;
            QPLDLL bool full_file_name_equals(const std::string& str) const;
            QPLDLL bool full_file_name_contains(const std::string_view& str) const;
            QPLDLL bool full_file_name_contains(const char* str) const;
            QPLDLL bool full_file_name_contains(const std::string& str) const;
            QPLDLL bool full_file_name_matches(const std::string& regex) const;
            QPLDLL bool full_file_name_matches(const std::regex& regex) const;

            QPLDLL bool name_equals(const std::string_view& str) const;
            QPLDLL bool name_equals(const char* str) const;
            QPLDLL bool name_equals(const std::string& str) const;
            QPLDLL bool name_contains(const std::string_view& str) const;
            QPLDLL bool name_contains(const char* str) const;
            QPLDLL bool name_contains(const std::string& str) const;
            QPLDLL bool name_matches(const std::string& regex) const;
            QPLDLL bool name_matches(const std::regex& regex) const;

            QPLDLL void rename(const qpl::filesys::path& new_name);
            QPLDLL void set_file_name(const std::string& new_name);
            QPLDLL void set_file_extension_name(const std::string& new_extension);
            QPLDLL void set_full_file_name(const std::string& new_file);
            QPLDLL void remove() const;
            QPLDLL void encrypt(const qpl::filesys::path& destination, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256) const;
            QPLDLL void decrypt(const qpl::filesys::path& destination, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256) const;
            QPLDLL void copy(const qpl::filesys::path& path_destination) const;
            QPLDLL void copy_overwrite(const qpl::filesys::path& path_destination) const;
            QPLDLL void move(const qpl::filesys::path& path_destination) const;
            QPLDLL void move_overwrite(const qpl::filesys::path& path_destination) const;
            QPLDLL void move_and_apply_path(const qpl::filesys::path& path_destination);
            QPLDLL void move_overwrite_and_apply_path(const qpl::filesys::path& path_destination);

            QPLDLL qpl::size branch_size() const;
            QPLDLL qpl::filesys::path get_branch_at(qpl::size index) const;
            QPLDLL qpl::filesys::paths get_branches() const;
            QPLDLL qpl::filesys::path get_branch_earlier(qpl::u32 n) const;
            QPLDLL qpl::filesys::path get_parent_branch() const;
            QPLDLL qpl::filesys::path get_last_branch() const;
            QPLDLL std::vector<std::string> get_branch_names() const;

            QPLDLL void set_branch(qpl::size index, std::string branch_name);
            QPLDLL qpl::filesys::path subpath(qpl::size off, qpl::size size = qpl::size_max) const;
            QPLDLL void ensure_branches_exist();
            QPLDLL void ensure_directory_backslash();

            QPLDLL bool is_root() const;
            QPLDLL path& go_root();

            //returns false if empty path or afterwards at root, otherwise true
            QPLDLL bool go_directories_back(qpl::u32 steps);
            //returns false if empty path or afterwards at root, otherwise true
            QPLDLL bool go_directory_back();
            QPLDLL path& go_into_directory(const std::string& directory_name);
            QPLDLL path& go_into_file(const std::string& file_name);
            QPLDLL path& go_into(const std::string& entry);
            QPLDLL path& cd(const std::string& directory_name);
            QPLDLL void append(const std::string& string);
            QPLDLL void append(char c);

            QPLDLL qpl::filesys::paths list_current_directory() const;
            QPLDLL qpl::filesys::paths list_current_directory_tree() const;
            QPLDLL void print_current_directory() const;
            QPLDLL void print_tree() const;

            QPLDLL qpl::filesys::paths make_directory_range() const;
            QPLDLL qpl::filesys::paths make_directory_range_tree() const;

            QPLDLL qpl::size last_common_branch(const qpl::filesys::path& path) const;
            QPLDLL qpl::filesys::path get_common_branch(const qpl::filesys::path& other) const;

            QPLDLL qpl::filesys::paths search_where_extension_equals(const std::string_view& extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_equals(const char* extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_equals(const std::string& extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_where_extension_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_extension_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_extension_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_where_name_equals(const std::string_view& name) const;
            QPLDLL qpl::filesys::paths search_where_name_equals(const char* name) const;
            QPLDLL qpl::filesys::paths search_where_name_equals(const std::string& name) const;
            QPLDLL qpl::filesys::paths search_where_name_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_where_name_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_name_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_name_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_where_file_name_equals(const std::string_view& file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_equals(const char* file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_equals(const std::string& file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_where_file_name_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_file_name_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_file_name_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_where_extension_doesnt_equal(const std::string_view& extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_equal(const char* extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_equal(const std::string& extension) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_contain(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_extension_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_where_name_doesnt_equal(const std::string_view& name) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_equal(const char* name) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_equal(const std::string& name) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_contain(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_name_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_equal(const std::string_view& file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_equal(const char* file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_equal(const std::string& file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_contain(const std::string_view& file_name) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_where_file_name_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_directories() const;
            QPLDLL qpl::filesys::paths search_files() const;

            QPLDLL qpl::filesys::paths search_recursively_where_extension_equals(const std::string_view& extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_equals(const char* extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_equals(const std::string& extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_where_name_equals(const std::string_view& name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_equals(const char* name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_equals(const std::string& name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_where_file_name_equals(const std::string_view& file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_equals(const char* file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_equals(const std::string& file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_contains(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_contains(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_contains(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_matches(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_equal(const std::string_view& extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_equal(const char* extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_equal(const std::string& extension) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_contain(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_extension_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_equal(const std::string_view& name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_equal(const char* name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_equal(const std::string& name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_contain(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_name_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_equal(const std::string_view& file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_equal(const char* file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_equal(const std::string& file_name) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_contain(const std::string_view& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_contain(const char* str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_contain(const std::string& str) const;
            QPLDLL qpl::filesys::paths search_recursively_where_file_name_doesnt_match(const std::regex& regex) const;

            QPLDLL qpl::filesys::paths search_recursively_directories() const;
            QPLDLL qpl::filesys::paths search_recursively_files() const;

        private:
            QPLDLL void construct();
            QPLDLL void check_update() const;

            std::string m_string;
            mutable bool m_is_file;
            mutable bool m_is_directory;
            mutable bool m_exists;
            mutable bool m_update;
        };

        class paths {
        public:
            paths() {

            }
            paths(const std::vector<qpl::filesys::path>& paths) {
                *this = paths;
            }
            paths(const qpl::filesys::paths& paths) {
                *this = paths;
            }

            QPLDLL paths& operator=(const std::vector<qpl::filesys::path>& paths);
            QPLDLL paths& operator=(const qpl::filesys::paths& paths);

            QPLDLL std::vector<qpl::filesys::path>::iterator begin();
            QPLDLL std::vector<qpl::filesys::path>::const_iterator begin() const;
            QPLDLL std::vector<qpl::filesys::path>::const_iterator cbegin() const;
            QPLDLL std::vector<qpl::filesys::path>::reverse_iterator rbegin();
            QPLDLL std::vector<qpl::filesys::path>::iterator end();
            QPLDLL std::vector<qpl::filesys::path>::const_iterator end() const;
            QPLDLL std::vector<qpl::filesys::path>::const_iterator cend() const;
            QPLDLL std::vector<qpl::filesys::path>::reverse_iterator rend();


            QPLDLL void copy_to(qpl::filesys::path destination);
            QPLDLL void move_to(qpl::filesys::path destination);
            QPLDLL void copy_overwrite_to(qpl::filesys::path destination);
            QPLDLL void move_overwrite_to(qpl::filesys::path destination);

            QPLDLL void copy_files_to(qpl::filesys::path destination);
            QPLDLL void move_files_to(qpl::filesys::path destination);
            QPLDLL void copy_overwrite_files_to(qpl::filesys::path destination);
            QPLDLL void move_overwrite_files_to(qpl::filesys::path destination);
            
            QPLDLL void copy_as_tree_to(qpl::filesys::path destination);
            QPLDLL void move_as_tree_to(qpl::filesys::path destination);
            QPLDLL void copy_overwrite_as_tree_to(qpl::filesys::path destination);
            QPLDLL void move_overwrite_as_tree_to(qpl::filesys::path destination);


            QPLDLL qpl::size size() const;
            QPLDLL bool empty() const;
            QPLDLL qpl::size capacity() const;
            QPLDLL void resize(qpl::size new_size);
            QPLDLL void reserve(qpl::size new_cap);
            QPLDLL void clear();

            QPLDLL qpl::filesys::path& front();
            QPLDLL const qpl::filesys::path& front() const;
            QPLDLL qpl::filesys::path& back();
            QPLDLL const qpl::filesys::path& back() const;

            QPLDLL void push_back(const qpl::filesys::path& path);
            QPLDLL void emplace_back(qpl::filesys::path& path);
            QPLDLL void pop_back();

            QPLDLL qpl::filesys::path& operator[](qpl::size index);
            QPLDLL const qpl::filesys::path& operator[](qpl::size index) const;
            QPLDLL qpl::filesys::path& at(qpl::size index);
            QPLDLL const qpl::filesys::path& at(qpl::size index) const;

            QPLDLL void print_tree() const;

            QPLDLL void list_remove_where_lambda(const std::function<bool(qpl::filesys::path)>& check);
            QPLDLL void list_keep_where_lambda(const std::function<bool(qpl::filesys::path)>& check);

            QPLDLL void list_add(const qpl::filesys::path& path);
            QPLDLL void list_add_recursively(const qpl::filesys::path& path);

            QPLDLL void list_add_directories(const qpl::filesys::path& directory);
            QPLDLL void list_recursively_add_directories(const qpl::filesys::path& directory);
            QPLDLL void list_remove_directories();
            QPLDLL void list_add_files(const qpl::filesys::path& directory);
            QPLDLL void list_recursively_add_files(const qpl::filesys::path& directory);
            QPLDLL void list_remove_files();

            QPLDLL void list_remove_where_directory_matches(const std::string& str);

            QPLDLL void list_remove_where_extension_equals(const std::string& extension);
            QPLDLL void list_add_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension);
            QPLDLL void list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension);
            QPLDLL void list_remove_where_extension_contains(const std::string& str);
            QPLDLL void list_add_where_extension_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_remove_where_extension_matches(const std::regex& regex);
            QPLDLL void list_add_where_extension_matches(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_extension_matches(const qpl::filesys::path& directory, const std::regex& regex);

            QPLDLL void list_remove_where_name_equals(const std::string& name);
            QPLDLL void list_add_where_name_equals(const qpl::filesys::path& directory, const std::string& name);
            QPLDLL void list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const std::string& name);
            QPLDLL void list_remove_where_name_contains(const std::string& name);
            QPLDLL void list_add_where_name_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_remove_where_name_matches(const std::regex& regex);
            QPLDLL void list_add_where_name_matches(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_name_matches(const qpl::filesys::path& directory, const std::regex& regex);

            QPLDLL void list_remove_where_file_name_equals(const std::string& file_name);
            QPLDLL void list_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string& file_name);
            QPLDLL void list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string& file_name);
            QPLDLL void list_remove_where_file_name_contains(const std::string& file_name);
            QPLDLL void list_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_remove_where_file_name_matches(const std::regex& regex);
            QPLDLL void list_add_where_file_name_matches(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_file_name_matches(const qpl::filesys::path& directory, const std::regex& regex);


            QPLDLL void list_keep_where_extension_equals(const std::string& extension);
            QPLDLL void list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension);
            QPLDLL void list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension);
            QPLDLL void list_keep_where_extension_contains(const std::string& str);
            QPLDLL void list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_keep_where_extension_matches(const std::regex& regex);
            QPLDLL void list_add_where_extension_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_extension_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);

            QPLDLL void list_keep_where_name_equals(const std::string& name);
            QPLDLL void list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name);
            QPLDLL void list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name);
            QPLDLL void list_keep_where_name_contains(const std::string& name);
            QPLDLL void list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_keep_where_name_matches(const std::regex& regex);
            QPLDLL void list_add_where_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);

            QPLDLL void list_keep_where_file_name_equals(const std::string& file_name);
            QPLDLL void list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& file_name);
            QPLDLL void list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& file_name);
            QPLDLL void list_keep_where_file_name_contains(const std::string& file_name);
            QPLDLL void list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& str);
            QPLDLL void list_keep_where_file_name_matches(const std::regex& regex);
            QPLDLL void list_add_where_file_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);
            QPLDLL void list_recursively_add_where_file_name_doesnt_match(const qpl::filesys::path& directory, const std::regex& regex);


            QPLDLL void list_remove_where_extension_equals(const std::string_view& extension);
            QPLDLL void list_add_where_extension_equals(const qpl::filesys::path& directory, const std::string_view& extension);
            QPLDLL void list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const std::string_view& extension);
            QPLDLL void list_remove_where_extension_contains(const std::string_view& str);
            QPLDLL void list_add_where_extension_contains(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const std::string_view& str);

            QPLDLL void list_remove_where_name_equals(const std::string_view& name);
            QPLDLL void list_add_where_name_equals(const qpl::filesys::path& directory, const std::string_view& name);
            QPLDLL void list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const std::string_view& name);
            QPLDLL void list_remove_where_name_contains(const std::string_view& name);
            QPLDLL void list_add_where_name_contains(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const std::string_view& str);

            QPLDLL void list_remove_where_file_name_equals(const std::string_view& file_name);
            QPLDLL void list_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string_view& file_name);
            QPLDLL void list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const std::string_view& file_name);
            QPLDLL void list_remove_where_file_name_contains(const std::string_view& file_name);
            QPLDLL void list_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const std::string_view& str);


            QPLDLL void list_keep_where_extension_equals(const std::string_view& extension);
            QPLDLL void list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& extension);
            QPLDLL void list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& extension);
            QPLDLL void list_keep_where_extension_contains(const std::string_view& str);
            QPLDLL void list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);

            QPLDLL void list_keep_where_name_equals(const std::string_view& name);
            QPLDLL void list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& name);
            QPLDLL void list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& name);
            QPLDLL void list_keep_where_name_contains(const std::string_view& name);
            QPLDLL void list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);

            QPLDLL void list_keep_where_file_name_equals(const std::string_view& file_name);
            QPLDLL void list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& file_name);
            QPLDLL void list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const std::string_view& file_name);
            QPLDLL void list_keep_where_file_name_contains(const std::string_view& file_name);
            QPLDLL void list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);
            QPLDLL void list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const std::string_view& str);


            QPLDLL void list_remove_where_extension_equals(const char* extension);
            QPLDLL void list_add_where_extension_equals(const qpl::filesys::path& directory, const char* extension);
            QPLDLL void list_recursively_add_where_extension_equals(const qpl::filesys::path& directory, const char* extension);
            QPLDLL void list_remove_where_extension_contains(const char* str);
            QPLDLL void list_add_where_extension_contains(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_extension_contains(const qpl::filesys::path& directory, const char* str);

            QPLDLL void list_remove_where_name_equals(const char* name);
            QPLDLL void list_add_where_name_equals(const qpl::filesys::path& directory, const char* name);
            QPLDLL void list_recursively_add_where_name_equals(const qpl::filesys::path& directory, const char* name);
            QPLDLL void list_remove_where_name_contains(const char* name);
            QPLDLL void list_add_where_name_contains(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_name_contains(const qpl::filesys::path& directory, const char* str);

            QPLDLL void list_remove_where_file_name_equals(const char* file_name);
            QPLDLL void list_add_where_file_name_equals(const qpl::filesys::path& directory, const char* file_name);
            QPLDLL void list_recursively_add_where_file_name_equals(const qpl::filesys::path& directory, const char* file_name);
            QPLDLL void list_remove_where_file_name_contains(const char* file_name);
            QPLDLL void list_add_where_file_name_contains(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_file_name_contains(const qpl::filesys::path& directory, const char* str);


            QPLDLL void list_keep_where_extension_equals(const char* extension);
            QPLDLL void list_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const char* extension);
            QPLDLL void list_recursively_add_where_extension_doesnt_equal(const qpl::filesys::path& directory, const char* extension);
            QPLDLL void list_keep_where_extension_contains(const char* str);
            QPLDLL void list_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_extension_doesnt_contain(const qpl::filesys::path& directory, const char* str);

            QPLDLL void list_keep_where_name_equals(const char* name);
            QPLDLL void list_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const char* name);
            QPLDLL void list_recursively_add_where_name_doesnt_equal(const qpl::filesys::path& directory, const char* name);
            QPLDLL void list_keep_where_name_contains(const char* name);
            QPLDLL void list_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_name_doesnt_contain(const qpl::filesys::path& directory, const char* str);

            QPLDLL void list_keep_where_file_name_equals(const char* file_name);
            QPLDLL void list_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const char* file_name);
            QPLDLL void list_recursively_add_where_file_name_doesnt_equal(const qpl::filesys::path& directory, const char* file_name);
            QPLDLL void list_keep_where_file_name_contains(const char* file_name);
            QPLDLL void list_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const char* str);
            QPLDLL void list_recursively_add_where_file_name_doesnt_contain(const qpl::filesys::path& directory, const char* str);

        private:
            std::vector<qpl::filesys::path> m_paths;
        };

        QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::filesys::path& path);

        QPLDLL bool file_content_equals(const qpl::filesys::path& path1, const qpl::filesys::path& path2);

        QPLDLL qpl::size file_lines(const qpl::filesys::path& path);

        QPLDLL qpl::size file_bytes(const qpl::filesys::path& path);

        QPLDLL std::vector<qpl::size> file_line_differences(const qpl::filesys::path& path1, const qpl::filesys::path& path2);
        QPLDLL qpl::f64 file_lines_difference_percentage(const qpl::filesys::path& path1, const qpl::filesys::path& path2);

        QPLDLL void split_file(const qpl::filesys::path& path, qpl::u32 bytes);
        QPLDLL void combine_files(const qpl::filesys::paths& paths, const qpl::filesys::path& destination);

        QPLDLL qpl::filesys::paths list_directory(const qpl::filesys::path& path);
        QPLDLL qpl::filesys::paths list_current_directory();
        QPLDLL qpl::filesys::paths list_directory_tree(const qpl::filesys::path& path);
        QPLDLL qpl::filesys::paths list_current_directory_tree();
        QPLDLL void print_current_tree();
        QPLDLL void print_tree(qpl::filesys::path path);

        QPLDLL void cd(qpl::filesys::path& path, const qpl::filesys::path& target);

        QPLDLL void create(const qpl::filesys::path& path);
        QPLDLL bool last_write_time_equal(const qpl::filesys::path& path1, const qpl::filesys::path& path2);


        QPLDLL bool is_file(const qpl::filesys::path& path);
        QPLDLL bool is_directory(const qpl::filesys::path& path);
        QPLDLL bool has_extension(const qpl::filesys::path& path);
        QPLDLL std::string get_name(const qpl::filesys::path& path);
        QPLDLL std::string get_file_extension(const qpl::filesys::path& path);
        QPLDLL std::string get_file_name(const qpl::filesys::path& path);
        QPLDLL std::string get_full_file_name(const qpl::filesys::path& path);
        QPLDLL std::string_view get_name_view(const qpl::filesys::path& path);
        QPLDLL std::string_view get_file_extension_view(const qpl::filesys::path& path);
        QPLDLL std::string_view get_file_name_view(const qpl::filesys::path& path);
        QPLDLL std::string_view get_full_file_name_view(const qpl::filesys::path& path);
        QPLDLL qpl::filesys::path get_parent_branch(const qpl::filesys::path& path);

        QPLDLL bool extension_equals(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool extension_equals(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool extension_contains(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool extension_contains(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool extension_matches(const qpl::filesys::path& path, const std::string& regex);
        QPLDLL bool extension_matches(const qpl::filesys::path& path, const std::regex& regex);
        QPLDLL bool file_name_equals(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool file_name_equals(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool file_name_contains(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool file_name_contains(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool file_name_matches(const qpl::filesys::path& path, const std::string& regex);
        QPLDLL bool file_name_matches(const qpl::filesys::path& path, const std::regex& regex);
        QPLDLL bool full_file_name_equals(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool full_file_name_equals(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool full_file_name_contains(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool full_file_name_contains(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool full_file_name_matches(const qpl::filesys::path& path, const std::string& regex);
        QPLDLL bool full_file_name_matches(const qpl::filesys::path& path, const std::regex& regex);
        QPLDLL bool name_equals(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool name_equals(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool name_contains(const qpl::filesys::path& path, const std::string_view& str);
        QPLDLL bool name_contains(const qpl::filesys::path& path, const std::string& str);
        QPLDLL bool name_matches(const qpl::filesys::path& path, const std::string& regex);
        QPLDLL bool name_matches(const qpl::filesys::path& path, const std::regex& regex);


        QPLDLL bool exists(const qpl::filesys::path& path_source);
        QPLDLL void rename(qpl::filesys::path& path_source, const qpl::filesys::path& new_name);
        QPLDLL void rename(const qpl::filesys::path& path_source, const qpl::filesys::path& new_name);
        QPLDLL void remove(const qpl::filesys::path& path_target);
        QPLDLL void copy(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination);
        QPLDLL void copy_overwrite(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination);
        QPLDLL void move(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination);
        QPLDLL void move_overwrite(const qpl::filesys::path& path_source, const qpl::filesys::path& path_destination);

        QPLDLL void partially_rename_all(qpl::filesys::paths& files, const std::string& regex, const std::string& replace);
        QPLDLL void partially_rename_all(qpl::filesys::path& path, const std::string& regex, const std::string& replace);
        QPLDLL void copy_unpack_directory(const qpl::filesys::path& path);
        QPLDLL void move_unpack_directory(const qpl::filesys::path& path);
        QPLDLL void remove_all(const qpl::filesys::paths& files);

        QPLDLL void move_all(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void copy_all(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void move_all_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void copy_all_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);

        QPLDLL void move_all_files(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void copy_all_files(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void move_all_files_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);
        QPLDLL void copy_all_files_overwrite(const qpl::filesys::paths& files, const qpl::filesys::path& path_destination);

        QPLDLL qpl::filesys::paths search_where_extension_equals(const qpl::filesys::path& directory, const std::string& extension);
        QPLDLL qpl::filesys::paths search_where_extension_contains(const qpl::filesys::path& directory, const std::string& regex);
        QPLDLL qpl::filesys::paths search_where_name_equals(const qpl::filesys::path& directory, const std::string& name);
        QPLDLL qpl::filesys::paths search_where_name_contains(const qpl::filesys::path& directory, const std::string& regex);

        QPLDLL qpl::filesys::paths search_where_extension_doesnt_equal(const qpl::filesys::path& directory, const std::string& extension);
        QPLDLL qpl::filesys::paths search_where_extension_doesnt_contain(const qpl::filesys::path& directory, const std::string& regex);
        QPLDLL qpl::filesys::paths search_where_name_doesnt_equal(const qpl::filesys::path& directory, const std::string& name);
        QPLDLL qpl::filesys::paths search_where_name_doesnt_contain(const qpl::filesys::path& directory, const std::string& regex);

        QPLDLL std::string read_file(const std::string& path);
        QPLDLL std::string read_rest_of_file(std::ifstream& file, bool close_file = true);
        QPLDLL std::filesystem::file_time_type file_last_write_time(const std::string& path);
        QPLDLL std::string file_last_write_time_str(const std::string& path);
        QPLDLL void write_to_file(const std::string& text, const std::string& path);
        QPLDLL void writeln_to_file(const std::string& text, const std::string& path);


        QPLDLL void write_data_file(const std::string& data, const std::string& path);
        QPLDLL qpl::filesys::path get_current_location();

        template<typename C>
        void container_memory_to_file(const C& data, const std::string& path) {
            if constexpr (qpl::is_vector_like<C>()) {
                qpl::filesys::write_data_file(qpl::container_memory_to_string(data), path);
            }
            else {
                std::ofstream file(path, std::ios::binary);
                file.write(reinterpret_cast<const char*>(data.data()), data.size() * qpl::bytes_in_type<qpl::container_subtype<C>>());
                file.close();
            }
        }
        template<typename C>
        void container_memory_from_file(C& data, const std::string& path) {
            if constexpr (qpl::is_vector_like<C>()) {
                qpl::string_to_container_memory(qpl::filesys::read_file(path), data);
            }
            else {
                std::ifstream file(path, std::ios::ate | std::ios::binary);

                if (!file.is_open()) {
                    throw std::runtime_error(qpl::to_string("failed to open file \"", path, "\"").c_str());
                }

                auto file_size = (size_t)file.tellg();
                data.resize((file_size - 1) / qpl::bytes_in_type<qpl::container_subtype<C>>() + 1);

                file.seekg(0);
                file.read(reinterpret_cast<char*>(data.data()), file_size);

                file.close();
            }
        }


         QPLDLL std::string file_encrypt(const std::string& path, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
         QPLDLL void file_encrypt_to(const std::string& source_path, const std::string& dest_path, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
         QPLDLL std::string file_decrypt(const std::string& path, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
         QPLDLL void file_decrypt_to(const std::string& source_path, const std::string& dest_path, const std::string& key, qpl::aes::mode mode = qpl::aes::mode::_256);
    }

    QPLDLL std::string read_file(const std::string& path);
    QPLDLL void write_to_file(const std::string& text, const std::string& path);
    QPLDLL void write_data_file(const std::string& data, const std::string& path);


    struct file_encrypter {
        qpl::filesys::paths paths;
        qpl::filesys::path common_branch;
        std::unordered_map<std::string, qpl::filesys::paths> part_paths;
        std::string keyword_string_part = "AES_PART";
        std::string keyword_string_enrypted = "ENCRYPTED";
        std::string keyword_string_derypted = "DECRYPTED";

        void clear();
        void add(std::string path);
        std::string encrypted_string(const std::string& key, qpl::aes::mode mode);
        qpl::filesys::paths encrypt(const std::string& key, std::string output_name, qpl::aes::mode mode, qpl::filesys::path destination_path = "", qpl::size split_size = qpl::size_max);
        qpl::filesys::paths decrypt(const std::string& key, qpl::aes::mode mode, qpl::filesys::path destination_path = "") const;

    private:
        qpl::size additions = 0u;
        bool adding_parts = false;

        void internal_decrypt(const std::string& string, const std::string& key, qpl::aes::mode mode, qpl::filesys::path destination_path, qpl::filesys::paths& tree) const;
    };

}

#endif