#pragma once
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <set>
#include <optional>
#include <qpl/time.hpp>

namespace qpl {


	struct huffman_compression {

		struct fast_decompression_map {
			std::vector<std::unordered_map<qpl::u32, char>> values;
			qpl::u32 bits = 0;
			qpl::u32 length = 0;

			QPLDLL void reset();
			QPLDLL void add_bit(bool b);
			QPLDLL bool is_found() const;
			QPLDLL char get_char() const;
			QPLDLL bool is_invalid() const;

		};

		struct character_table {

			struct character_info32 {
				qpl::u32 bits = 0;
				qpl::u8 width = 0;
				qpl::u8 meaning = 0;

				QPLDLL bool operator==(const character_info32& other) const;
				QPLDLL bool operator!=(const character_info32& other) const;
			};
			struct character_info16 {
				qpl::u16 bits = 0;
				qpl::u8 width = 0;
				qpl::u8 meaning = 0;

				QPLDLL bool operator==(const character_info16& other) const;
				QPLDLL bool operator!=(const character_info16& other) const;
			};
			struct character_info8 {
				qpl::u8 bits = 0;
				qpl::u8 width = 0;
				qpl::u8 meaning = 0;

				QPLDLL bool operator==(const character_info8& other) const;
				QPLDLL bool operator!=(const character_info8& other) const;
			};

			std::vector<character_info32> data32;
			std::vector<character_info16> data16;
			std::vector<character_info8> data8;

			std::unordered_map<char, std::pair<qpl::u8, qpl::u8>> map8;
			std::unordered_map<char, std::pair<qpl::u16, qpl::u8>> map16;
			std::unordered_map<char, std::pair<qpl::u32, qpl::u8>> map32;

			enum class mode {
				b8, b16, b32
			};
			mode bits_mode = mode::b8;
			qpl::u8 max_width_length = 0;
			qpl::u8 max_bits_length = 0;

			QPLDLL qpl::size get_block_size() const;
			QPLDLL void add(qpl::u8 c, qpl::u32 bits, qpl::u8 width);
			QPLDLL void add(qpl::u8 c, qpl::u16 bits, qpl::u8 width);
			QPLDLL void add(qpl::u8 c, qpl::u8 bits, qpl::u8 width);
			QPLDLL void set_info(qpl::size max_depth);
			QPLDLL std::optional<fast_decompression_map> get_decompression_map() const;
			QPLDLL void make_map();
			QPLDLL bool operator==(const character_table& other) const;
			QPLDLL bool operator!=(const character_table& other) const;
			QPLDLL qpl::size data_size() const;
			QPLDLL std::string get_header_string() const;
			QPLDLL bool create(qpl::bit_string_istream& stream, qpl::u16 data_size);

			void print() {
				qpl::println("width = ", (int)this->max_width_length);
				qpl::println("bits  = ", (int)this->max_bits_length);
				qpl::println("mode  = ", 8 << (int)this->bits_mode);

				switch (this->bits_mode) {
				case mode::b8:
					for (auto& i : this->data8) {
						qpl::println("\"", i.meaning, "\" : ", qpl::prepended_to_string_to_fit(qpl::binary_string(i.bits), "0", i.width), " (", (int)i.width, ")");
					}
					break;
				case mode::b16:
					for (auto& i : this->data16) {
						qpl::println("\"", i.meaning, "\" : ", qpl::prepended_to_string_to_fit(qpl::binary_string(i.bits), "0", i.width), " (", (int)i.width, ")");
					}
					break;
				case mode::b32:
					for (auto& i : this->data32) {
						qpl::println("\"", i.meaning, "\" : ", qpl::prepended_to_string_to_fit(qpl::binary_string(i.bits), "0", i.width), " (", (int)i.width, ")");
					}
					break;
				}
			}
		};

		struct tree {
			tree() {

			}
			tree(qpl::u8 c) {
				this->character = c;
			}
			std::vector<tree> nodes;
			qpl::u8 character = 0;

			QPLDLL void get_table_helper32(character_table& result, qpl::u32 bits, qpl::u8 width) const;
			QPLDLL void get_table_helper16(character_table& result, qpl::u16 bits, qpl::u8 width) const;
			QPLDLL void get_table_helper8(character_table& result, qpl::u8 bits, qpl::u8 width) const;
			QPLDLL character_table get_table() const;
			QPLDLL void create_from_table(const character_table& table);
			QPLDLL void max_depth_helper(qpl::size& result, qpl::size depth) const;
			QPLDLL qpl::size max_depth() const;
			template<typename T>
			void add_character(T bits, qpl::u8 position, qpl::u8 character) {

				if (position == 0u) {
					this->character = qpl::i8_cast(character);
				}
				else {
					if (this->nodes.empty()) {
						this->nodes.resize(2);
					}
					auto b = qpl::get_bit(bits, position - 1);
					this->nodes[b].add_character(bits, position - 1, character);
				}
			}
		};

		QPLDLL void compress(const std::string& string);
		QPLDLL bool decompress(const std::string& string);
		QPLDLL std::string get_result() const;

		std::string result;
	};


	namespace detail {
		QPLDLL extern qpl::huffman_compression huffman_compression;
	}

	QPLDLL std::string compress(const std::string& string);
	QPLDLL std::optional<std::string> decompress_checked(const std::string& string);
	QPLDLL std::string decompress(const std::string& string);
}