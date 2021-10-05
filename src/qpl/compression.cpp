#include <qpl/compression.hpp>

namespace qpl {

	void qpl::huffman_compression::fast_decompression_map::reset() {
		this->bits = 0;
		this->length = 0;
	}
	void qpl::huffman_compression::fast_decompression_map::add_bit(bool b) {
		++this->length;
		this->bits <<= 1;
		this->bits |= b;
	}
	bool qpl::huffman_compression::fast_decompression_map::is_found() const {
		return this->values[this->length].find(this->bits) != this->values[this->length].cend();
	}
	char qpl::huffman_compression::fast_decompression_map::get_char() const {
		return this->values[this->length].at(this->bits);
	}
	bool qpl::huffman_compression::fast_decompression_map::is_invalid() const {
		return this->length >= this->values.size();
	}


	bool qpl::huffman_compression::character_table::character_info32::operator==(const character_info32& other) const {
		return this->bits == other.bits && this->width == other.width && this->meaning == other.meaning;
	}
	bool qpl::huffman_compression::character_table::character_info32::operator!=(const character_info32& other) const {
		return !(*this == other);
	}


	bool qpl::huffman_compression::character_table::character_info16::operator==(const character_info16& other) const {
		return this->bits == other.bits && this->width == other.width && this->meaning == other.meaning;
	}
	bool qpl::huffman_compression::character_table::character_info16::operator!=(const character_info16& other) const {
		return !(*this == other);
	}
	bool qpl::huffman_compression::character_table::character_info8::operator==(const character_info8& other) const {
		return this->bits == other.bits && this->width == other.width && this->meaning == other.meaning;
	}
	bool qpl::huffman_compression::character_table::character_info8::operator!=(const character_info8& other) const {
		return !(*this == other);
	}

	qpl::size qpl::huffman_compression::character_table::get_block_size() const {
		return this->max_width_length + this->max_bits_length + qpl::bits_in_type<char>();
	}
	void qpl::huffman_compression::character_table::add(qpl::u8 c, qpl::u32 bits, qpl::u8 width) {
		this->data32.push_back(character_info32{ bits, width, c });
	}
	void qpl::huffman_compression::character_table::add(qpl::u8 c, qpl::u16 bits, qpl::u8 width) {
		this->data16.push_back(character_info16{ bits, width, c });
	}
	void qpl::huffman_compression::character_table::add(qpl::u8 c, qpl::u8 bits, qpl::u8 width) {
		this->data8.push_back(character_info8{ bits, width, c });
	}
	void qpl::huffman_compression::character_table::set_info(qpl::size max_depth) {
		this->max_width_length = qpl::significant_bit(max_depth);
		this->max_bits_length = max_depth;

		if (this->max_bits_length <= 8) this->bits_mode = mode::b8;
		else if (this->max_bits_length <= 16) this->bits_mode = mode::b16;
		else this->bits_mode = mode::b32;
	}
	std::optional<qpl::huffman_compression::fast_decompression_map> qpl::huffman_compression::character_table::get_decompression_map() const {
		fast_decompression_map result;
		result.values.resize(this->max_bits_length + 1);

		switch (this->bits_mode) {
		case character_table::mode::b8:
			for (auto& i : this->data8) {
				if (i.width >= result.values.size()) return {};
				result.values[i.width].insert(std::make_pair(qpl::u32_cast(i.bits), i.meaning));
			}
			break;
		case character_table::mode::b16:
			for (auto& i : this->data16) {
				if (i.width >= result.values.size()) return {};
				result.values[i.width].insert(std::make_pair(qpl::u32_cast(i.bits), i.meaning));
			}
			break;
		case character_table::mode::b32:
			for (auto& i : this->data32) {
				if (i.width >= result.values.size()) return {};
				result.values[i.width].insert(std::make_pair(qpl::u32_cast(i.bits), i.meaning));
			}
			break;
		}
		return result;
	}
	void qpl::huffman_compression::character_table::make_map() {

		switch (this->bits_mode) {
		case character_table::mode::b8:
			this->map8.clear();
			for (auto& i : this->data8) {
				this->map8[qpl::i8_cast(i.meaning)] = std::make_pair(i.bits, i.width);
			}
			break;
		case character_table::mode::b16:
			this->map16.clear();
			for (auto& i : this->data16) {
				this->map16[qpl::i8_cast(i.meaning)] = std::make_pair(i.bits, i.width);
			}
			break;
		case character_table::mode::b32:
			this->map32.clear();
			for (auto& i : this->data32) {
				this->map32[qpl::i8_cast(i.meaning)] = std::make_pair(i.bits, i.width);
			}
			break;
		}
	}
	bool qpl::huffman_compression::character_table::operator==(const character_table& other) const {
		if (this->bits_mode != other.bits_mode) return false;
		if (this->max_bits_length != other.max_bits_length) return false;
		if (this->max_width_length != other.max_width_length) return false;

		if (this->bits_mode == mode::b8 && this->data8 != other.data8) return false;
		if (this->bits_mode == mode::b16 && this->data16 != other.data16) return false;
		if (this->bits_mode == mode::b32 && this->data32 != other.data32) return false;
		return true;
	}
	bool qpl::huffman_compression::character_table::operator!=(const character_table& other) const {
		return !(*this == other);
	}
	qpl::size qpl::huffman_compression::character_table::data_size() const {

		switch (this->bits_mode) {
		case character_table::mode::b8:
			return this->data8.size();
		case character_table::mode::b16:
			return this->data16.size();
		case character_table::mode::b32:
			return this->data32.size();
		}
		return 0;
	}
	std::string qpl::huffman_compression::character_table::get_header_string() const {
		qpl::bit_string_ostream stream;

		stream.add(this->max_bits_length);
		stream.add(this->max_width_length);

		stream.reset_position();
		switch (this->bits_mode) {
		case character_table::mode::b8:
			for (auto& i : this->data8) {
				stream.add_bits(i.bits, this->max_bits_length);
				stream.add_bits(i.width, this->max_width_length);
				stream.add_bits(i.meaning);
			}
			break;
		case character_table::mode::b16:
			for (auto& i : this->data16) {
				stream.add_bits(i.bits, this->max_bits_length);
				stream.add_bits(i.width, this->max_width_length);
				stream.add_bits(i.meaning);
			}
			break;
		case character_table::mode::b32:
			for (auto& i : this->data32) {
				stream.add_bits(i.bits, this->max_bits_length);
				stream.add_bits(i.width, this->max_width_length);
				stream.add_bits(i.meaning);
			}
			break;
		}
		return stream.string();
	}
	bool qpl::huffman_compression::character_table::create(qpl::bit_string_istream& stream, qpl::u16 data_size) {

		this->max_bits_length = stream.get_next<qpl::u8>();
		this->max_width_length = stream.get_next<qpl::u8>();
		stream.note_u64_position_offset();

		if (this->max_bits_length <= 8) this->bits_mode = mode::b8;
		else if (this->max_bits_length <= 16) this->bits_mode = mode::b16;
		else this->bits_mode = mode::b32;

		auto block_size = this->get_block_size();

		qpl::u64 n;
		switch (this->bits_mode) {
		case character_table::mode::b8:
			this->data8.resize(data_size);
			for (qpl::u16 i = 0u; i < data_size; ++i) {
				n = stream.get_next_bits<qpl::u64>(block_size);
				this->data8[i].bits = qpl::u8_cast(n >> (block_size - this->max_bits_length));
				this->data8[i].width = qpl::u8_cast((n >> qpl::bits_in_byte()) & ~(qpl::u64_max << this->max_width_length));
				this->data8[i].meaning = qpl::u8_cast(n);
				if (stream.is_done()) {
					break;
				}
			}
			break;
		case character_table::mode::b16:
			this->data16.resize(data_size);
			for (qpl::u16 i = 0u; i < data_size; ++i) {
				n = stream.get_next_bits<qpl::u64>(block_size);
				this->data16[i].bits = qpl::u16_cast(n >> (block_size - this->max_bits_length));
				this->data16[i].width = qpl::u8_cast((n >> qpl::bits_in_byte()) & ~(qpl::u64_max << this->max_width_length));
				this->data16[i].meaning = qpl::u8_cast(n);
				if (stream.is_done()) {
					break;
				}
			}
			break;
		case character_table::mode::b32:
			this->data32.resize(data_size);
			for (qpl::u16 i = 0u; i < data_size; ++i) {
				n = stream.get_next_bits<qpl::u64>(block_size);
				this->data32[i].bits = qpl::u32_cast(n >> (block_size - this->max_bits_length));
				this->data32[i].width = qpl::u8_cast((n >> qpl::bits_in_byte()) & ~(qpl::u64_max << this->max_width_length));
				this->data32[i].meaning = qpl::u8_cast(n);
				if (stream.is_done()) {
					break;
				}
			}
			break;
		}
		this->make_map();
		return true;
	}


	void qpl::huffman_compression::tree::get_table_helper32(character_table& result, qpl::u32 bits, qpl::u8 width) const {
		if (this->nodes.empty()) {
			result.add(this->character, bits, width);
		}
		else {
			this->nodes[0].get_table_helper32(result, (bits << 1) | 0x0, width + 1);
			this->nodes[1].get_table_helper32(result, (bits << 1) | 0x1, width + 1);
		}
	}
	void qpl::huffman_compression::tree::get_table_helper16(character_table& result, qpl::u16 bits, qpl::u8 width) const {
		if (this->nodes.empty()) {
			result.add(this->character, bits, width);
		}
		else {
			this->nodes[0].get_table_helper16(result, (bits << 1) | 0x0, width + 1);
			this->nodes[1].get_table_helper16(result, (bits << 1) | 0x1, width + 1);
		}
	}
	void qpl::huffman_compression::tree::get_table_helper8(character_table& result, qpl::u8 bits, qpl::u8 width) const {
		if (this->nodes.empty()) {
			result.add(this->character, bits, width);
		}
		else {
			this->nodes[0].get_table_helper8(result, (bits << 1) | 0x0, width + 1);
			this->nodes[1].get_table_helper8(result, (bits << 1) | 0x1, width + 1);
		}
	}
	qpl::huffman_compression::character_table qpl::huffman_compression::tree::get_table() const {
		character_table result;
		result.set_info(this->max_depth());
		switch (result.bits_mode) {
		case character_table::mode::b8:
			this->get_table_helper8(result, 0, 1);
			break;
		case character_table::mode::b16:
			this->get_table_helper16(result, 0, 1);
			break;
		case character_table::mode::b32:
			this->get_table_helper32(result, 0, 1);
			break;
		}
		return result;
	}
	void qpl::huffman_compression::tree::create_from_table(const character_table& table) {
		switch (table.bits_mode) {
		case character_table::mode::b8:
			for (auto& i : table.data8) {
				this->add_character(i.bits, i.width - 1, i.meaning);
			}
			break;
		case character_table::mode::b16:
			for (auto& i : table.data16) {
				this->add_character(i.bits, i.width - 1, i.meaning);
			}
			break;
		case character_table::mode::b32:
			for (auto& i : table.data32) {
				this->add_character(i.bits, i.width - 1, i.meaning);
			}
			break;
		}
	}
	void qpl::huffman_compression::tree::max_depth_helper(qpl::size& result, qpl::size depth) const {
		result = qpl::max(result, depth);
		for (auto& i : this->nodes) {
			i.max_depth_helper(result, depth + 1);
		}
	}
	qpl::size qpl::huffman_compression::tree::max_depth() const {
		qpl::size result = 0u;
		this->max_depth_helper(result, 1u);
		return result;
	}

	void qpl::huffman_compression::compress(const std::string& string) {
		if (string.empty()) {
			this->result = "";
			return;
		}
		std::unordered_map<qpl::u8, qpl::u32> count_map;

		for (auto& i : string) {
			++count_map[i];
		}
		struct sorted_t {
			qpl::u32 frequency;
			qpl::u16 special;

			std::string string() const {
				return qpl::to_string("[", frequency, " ", special, "]");
			}

			bool operator<(const sorted_t& other) const {
				if (this->frequency < other.frequency) return true;
				if (this->frequency > other.frequency) return false;
				return this->special < other.special;
			}
			bool operator==(const sorted_t& other) const {
				return this->frequency == other.frequency && this->special == other.special;
			}
		};

		std::set<sorted_t> sorted_count;


		for (auto& i : count_map) {
			sorted_count.insert({ i.second, qpl::u16_cast(i.first) });
		}

		std::vector<tree> trees;
		trees.reserve(sorted_count.size());
		if (sorted_count.size() == 1) {
			trees.push_back({});
			trees.back().character = sorted_count.cbegin()->special;
		}
		else {

			while (sorted_count.size() > 1) {
				trees.push_back({});

				auto p = sorted_count.cbegin();
				auto a = *(p);
				auto b = *(++p);

				trees.back().nodes.resize(2);

				qpl::u32 a_i = 0u;
				qpl::u32 b_i = 1u;
				if (a.frequency >= b.frequency) {
					a_i = 1u;
					b_i = 0u;
				}


				if (a.special & 0xFF00) {
					trees.back().nodes[a_i] = trees[a.special - 0x100];
				}
				else {
					trees.back().nodes[a_i] = tree(a.special);
				}

				if (b.special & 0xFF00) {
					trees.back().nodes[b_i] = trees[b.special - 0x100];
				}
				else {
					trees.back().nodes[b_i] = tree(b.special);
				}

				auto value = sorted_t{ a.frequency + b.frequency, qpl::u16_cast((trees.size() - 1) + 0x100) };

				sorted_count.erase(sorted_count.cbegin());
				sorted_count.erase(sorted_count.cbegin());
				sorted_count.insert(value);
			}
		}
		auto table = trees.back().get_table();

		table.make_map();

		qpl::bit_string_ostream stream;
		auto header = table.get_header_string();
		stream.add(qpl::u32_cast(string.length()));
		stream.add(qpl::u16_cast(table.data_size()));
		stream.add(header);


		stream.reset_position();

		switch (table.bits_mode) {
		case character_table::mode::b8:
			for (auto& i : string) {

				auto info = table.map8[i];
				stream.add_bits(info.first, info.second);
			}
			break;
		case character_table::mode::b16:
			for (auto& i : string) {
				auto info = table.map16[i];
				stream.add_bits(info.first, info.second);
			}
			break;
		case character_table::mode::b32:
			for (auto& i : string) {
				auto info = table.map32[i];
				stream.add_bits(info.first, info.second);
			}
			break;
		}
		this->result = stream.string();
	}
	bool qpl::huffman_compression::decompress(const std::string& string) {
		if (string.empty()) {
			this->result = "";
			return true;
		}

		qpl::bit_string_istream istream;
		istream.set(string);

		auto s_len = istream.get_next<qpl::u32>();
		auto d_len = istream.get_next<qpl::u16>();

		character_table table;
		if (!table.create(istream, d_len)) {
			this->result = "";
			return false;
		}
		istream.set_position_next_u64_multiple();

		auto decompression_t = table.get_decompression_map();
		if (!decompression_t.has_value()) {
			this->result = "";
			return false;
		}
		auto decompression = decompression_t.value();

		std::ostringstream stream;

		qpl::u32 ctr = 0u;
		bool done = false;
		while (!done && !istream.is_done()) {
			auto n = istream.get_next<qpl::u64>();

			for (qpl::u32 i = 0u; i < qpl::bits_in_type<qpl::u64>(); ++i) {
				auto b = qpl::bool_cast(n & (qpl::u64{ 1 } << (qpl::bits_in_type<qpl::u64>() - 1 - i)));

				decompression.add_bit(b);
				if (decompression.is_invalid()) {
					this->result = "";
					return false;
				}
				if (decompression.is_found()) {
					auto c = decompression.get_char();
					decompression.reset();

					stream << c;
					++ctr;
					if (ctr >= s_len) {
						done = true;
						break;
					}
				}
			}
		}
		this->result = stream.str();
		return true;
	}
	std::string qpl::huffman_compression::get_result() const {
		return this->result;
	}

	qpl::huffman_compression qpl::detail::huffman_compression;


	std::string qpl::compress(const std::string& string) {
		qpl::detail::huffman_compression.compress(string);
		return qpl::detail::huffman_compression.get_result();
	}
	std::optional<std::string> qpl::decompress_checked(const std::string& string) {
		bool result = qpl::detail::huffman_compression.decompress(string);
		if (!result) return {};
		return qpl::detail::huffman_compression.get_result();
	}
	std::string qpl::decompress(const std::string& string) {
		bool result = qpl::detail::huffman_compression.decompress(string);
		return qpl::detail::huffman_compression.get_result();
	}
}
