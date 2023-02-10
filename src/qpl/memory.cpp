#include <qpl/memory.hpp>
#include <qpl/string.hpp>
#include <qpl/filesys.hpp>

namespace qpl {

	void qpl::write_string_to_stream(std::ostream& os, const std::string& value) {
		os.write(value.data(), value.length());
	}
	void qpl::print_character_bool_table(std::string_view characters) {
		std::array<bool, 256> table;
		table.fill(false);
		for (auto& i : characters) {
			table[i] = true;
		}

		qpl::println('{');
		for (qpl::u32 i = 0u; i < table.size(); ++i) {
			if (i % 16 == 0) {
				qpl::print('\t');
			}
			qpl::print(table[i], ", ");
			if (i % 16 == 15) {
				qpl::println();
			}
		}
		qpl::println('}');
	}

    void qpl::save_state::clear() {
        this->collection_string.clear();
        this->ctr = 0u;
    }
    void qpl::save_state::file_save(std::string path) {
        this->collection_string.finalize();
        auto str = this->collection_string.get_string();
        qpl::filesys::write_data_file(str, path);
    }
    void qpl::save_state::file_save(std::string path, const std::array<qpl::u64, 4>& key) {
        this->collection_string.finalize();
        auto str = this->collection_string.get_string();
        str = qpl::encrypt(str, key);
        qpl::filesys::write_data_file(str, path);
    }
    void qpl::save_state::finalize_string() {
        this->collection_string.finalize();
    }
    std::string qpl::save_state::get_string() const {
        return this->collection_string.get_string();
    }
    std::string qpl::save_state::get_finalized_string() {
        this->finalize_string();
        return this->collection_string.get_string();
    }
    std::string qpl::save_state::get_next_string() {
        return this->collection_string.get_string(this->ctr++);
    }

    void qpl::load_state::clear() {
        this->collection_string.clear();
        this->ctr = 0u;
    }
    void qpl::load_state::file_load(std::string path) {
        this->ctr = 0u;
        auto str = qpl::filesys::read_file(path);
        this->collection_string.set_string(str);
        if (!this->collection_string.read_info()) {
            throw qpl::exception("save_state: \"", path, "\" failed to load.");
        }
    }
    void qpl::load_state::file_load(std::string path, const std::array<qpl::u64, 4>& key) {
        this->ctr = 0u;
        auto str = qpl::filesys::read_file(path);
        str = qpl::decrypt(str, key);
        this->collection_string.set_string(str);
        if (!this->collection_string.read_info()) {
            throw qpl::exception("save_state: \"", path, "\" failed to load.");
        }
    }
    void qpl::load_state::set_string(const std::string& str) {
        this->collection_string.set_string(str);
        if (!this->collection_string.read_info()) {
            throw qpl::exception("save_state::set_string: failed to load.");
        }
    }
}