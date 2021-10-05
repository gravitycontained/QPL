#include <qpl/bits.hpp>

namespace qpl {
	void qpl::bit_string_ostream::clear() const {
		this->value = 0;
		this->bit_position = qpl::bits_in_type<qpl::u64>();
		this->stream.str("");
	}
	std::string qpl::bit_string_ostream::string() const {
		if (this->bit_position != qpl::bits_in_type<qpl::u64>()) {
			this->finish();
		}
		return this->stream.str();
	}
	void qpl::bit_string_ostream::finish() const {
		if (this->bit_position != qpl::bits_in_type<qpl::u64>()) {
			this->write();
			this->value = 0;
			this->bit_position = qpl::bits_in_type<qpl::u64>();
		}
	}
	void qpl::bit_string_ostream::reset_position() {
		this->value = 0;
		this->bit_position = qpl::bits_in_type<qpl::u64>();
	}
	void qpl::bit_string_ostream::write() const {
		this->stream.write(reinterpret_cast<char*>(&this->value), qpl::bytes_in_type<qpl::u64>());
	}
	void qpl::bit_string_ostream::add(const std::string& n) {
		this->finish();
		this->stream.write(n.c_str(), n.length());
		this->value = 0;
		this->bit_position = qpl::bits_in_type<qpl::u64>();
		this->position += n.length() * qpl::bits_in_byte();
	}
	bit_string_ostream& qpl::bit_string_ostream::operator=(const bit_string_ostream& other) {
		this->clear();
		this->value = other.value;
		this->bit_position = other.bit_position;
		this->add(other.stream.str());
		return *this;
	}

	qpl::size qpl::bit_string_istream::size() const {
		return this->string.size();
	}
	void qpl::bit_string_istream::set(const std::string& string) {
		this->string = string;
		this->position = 0u;
	}
	void qpl::bit_string_istream::reset_position() {
		this->position = 0u;
	}
	std::string qpl::bit_string_istream::get_next_string(qpl::u32 size) {
		std::string str = this->string.substr(this->position / qpl::bits_in_byte(), size);
		this->position += size * qpl::bits_in_byte();
		return str;
	}
	bool qpl::bit_string_istream::is_done() const {
		return (this->position / qpl::bits_in_byte()) >= this->string.size();
	}
	void qpl::bit_string_istream::note_u64_position_offset() {
		this->position_u64_offset = (this->position / qpl::bits_in_byte()) % qpl::bytes_in_type<qpl::u64>();
	}
	void qpl::bit_string_istream::set_position_next_u64_multiple() {
		if (!this->position) {
			this->position_u64_offset = 0u;
			return;
		}
		this->position = (((this->position - 1) / qpl::bits_in_type<qpl::u64>()) + 1) * qpl::bits_in_type<qpl::u64>();
		this->position_u64_offset = 0u;
	}
	void qpl::bit_string_istream::set_position(qpl::u32 position) {
		this->position = position;
	}
}