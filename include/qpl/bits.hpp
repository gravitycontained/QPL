#ifndef QPL_BITS_HPP
#define QPL_BITS_HPP
#pragma once

#include <algorithm>
#include <qpl/algorithm.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/string.hpp>
#include <qpl/vardef.hpp>
#include <qpl/memory.hpp>

#include <array>

namespace qpl {
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 kilobyte(T n) {
		return qpl::u64_cast(1000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 kibibyte(T n) {
		return qpl::u64_cast(0x400ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 megabyte(T n) {
		return qpl::u64_cast(1000'000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 mebibyte(T n) {
		return qpl::u64_cast(0x100000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 gigabyte(T n) {
		return qpl::u64_cast(1000'000'000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 gebibyte(T n) {
		return qpl::u64_cast(0x40000000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 terabyte(T n) {
		return qpl::u64_cast(1'000'000'000'000ull * n);
	}
	template<typename T> requires (qpl::is_arithmetic<T>())
	constexpr qpl::u64 tebibyte(T n) {
		return qpl::u64_cast(0x10000000000ull * n);
	}

	template<qpl::u64 bits>
	using ubit = conditional<
		qpl::if_true<(bits <= 8ull)>, qpl::u8,
		qpl::if_true<(bits <= 16ull)>, qpl::u16,
		qpl::if_true<(bits <= 32ull)>, qpl::u32,
		qpl::if_true<(bits <= 64ull)>, qpl::u64,
		qpl::default_type, qpl::integer<bits, 0>>;

	template<qpl::u64 bits>
	using ibit = conditional <
		qpl::if_true<(bits <= 8ull)>, qpl::i8,
		qpl::if_true<(bits <= 16ull)>, qpl::i16,
		qpl::if_true<(bits <= 32ull)>, qpl::i32,
		qpl::if_true<(bits <= 64ull)>, qpl::i64,
		qpl::default_type, qpl::integer<bits, 1>>;

	template<qpl::u64 bits>
	using uxbit = conditional<
		qpl::if_true<(bits <= 8ull)>, qpl::u8,
		qpl::if_true<(bits <= 16ull)>, qpl::u16,
		qpl::if_true<(bits <= 32ull)>, qpl::u32,
		qpl::if_true<(bits <= 64ull)>, qpl::u64,
		qpl::default_type, qpl::x64_integer<bits, 0>>;

	template<qpl::u64 bits>
	using ixbit = conditional <
		qpl::if_true<(bits <= 8ull)>, qpl::i8,
		qpl::if_true<(bits <= 16ull)>, qpl::i16,
		qpl::if_true<(bits <= 32ull)>, qpl::i32,
		qpl::if_true<(bits <= 64ull)>, qpl::i64,
		qpl::default_type, qpl::x64_integer<bits, 1>>;


#ifndef QPL_NO_FLOATS
	template<qpl::u64 bits>
	using fbit = conditional <
		qpl::if_true<(bits == 32ull)>, qpl::f32,
		qpl::if_true<(bits == 64ull)>, qpl::f64,
		qpl::default_type, qpl::floating_point<32u, bits>>;
#else
	template<qpl::u64 bits>
	using fbit = conditional <
		qpl::if_true<(bits == 32ull)>, qpl::f32,
		qpl::if_true<(bits == 64ull)>, qpl::f64, qpl::default_error>;
#endif


	template<qpl::u64 bits, bool BOUNDARY_CHECK = detail::array_boundary_check>
	class bitset {
	public:
		using uint_type = qpl::conditional<
			qpl::if_true<(bits <= 64u)>, qpl::ubit<bits>,
			qpl::default_type, qpl::u64>;

		using holding_type = qpl::conditional<
			qpl::if_true<(bits <= 64u)>, qpl::ubit<bits>,
			qpl::default_type, qpl::array<qpl::u64, qpl::approximate_multiple_up(bits, qpl::u64{ 64 }) / 64, BOUNDARY_CHECK>>;

		class bitset_range_proxy {
		public:
			bitset_range_proxy(bitset& data, qpl::u32 begin, qpl::u32 end) {
				this->ptr = &data;
				this->begin = begin;
				this->end = end;
			}

			uint_type get() const {
				return this->ptr->get_range(this->begin, this->end);
			}

			operator bool() const {
				return this->get();
			}
			bitset_range_proxy operator=(uint_type value) const {
				this->ptr->set_range(this->begin, this->end, value);
				return *this;
			}

			bitset_range_proxy operator|=(uint_type value) const {
				this->ptr->set_range(this->begin, this->end, this->get() | value);
				return *this;
			}
			uint_type operator|(uint_type value) const {
				return this->get() | value;
			}
			bitset_range_proxy operator&=(uint_type value) const {
				this->ptr->set_range(this->begin, this->end, this->get() & value);
				return *this;
			}
			uint_type operator&(uint_type value) const {
				return this->get() & value;
			}
			bitset_range_proxy operator^=(uint_type value) const {
				this->ptr->set_range(this->begin, this->end, this->get() ^ value);
				return *this;
			}
			uint_type operator^(uint_type value) const {
				return this->get() ^ value;
			}
			bitset* ptr;
			qpl::u32 begin;
			qpl::u32 end;
		};

		class bitset_range_const_proxy {
		public:
			bitset_range_const_proxy(const bitset& data, qpl::u32 begin, qpl::u32 end) {
				this->ptr = &data;
				this->begin = begin;
				this->end = end;
			}

			uint_type get() const {
				return this->ptr->get_range(this->begin, this->end);
			}

			operator bool() const {
				return this->get();
			}

			uint_type operator|(uint_type value) const {
				return this->get() | value;
			}
			uint_type operator&(uint_type value) const {
				return this->get() & value;
			}
			uint_type operator^(uint_type value) const {
				return this->get() ^ value;
			}

			const bitset* ptr;
			qpl::u32 begin;
			qpl::u32 end;
		};

		class bitset_proxy {
		public:
			bitset_proxy(bitset& data, qpl::size index) {
				this->ptr = &data;
				this->index = index;
			}

			bool get() const {
				return this->ptr->get(this->index);
			}

			operator bool() const {
				return this->get();
			}
			bitset_proxy operator=(bool value) const {
				this->ptr->set(this->index, value);
				return *this;
			}
			
			bitset_proxy operator|=(bool value) const {
				this->ptr->set(this->index, this->ptr->get(this->index) | value);
				return *this;
			}
			bool operator|(bool value) const {
				return this->ptr->get(this->index) || value;
			}
			bitset_proxy operator&=(bool value) const {
				this->ptr->set(this->index, this->ptr->get(this->index) & value);
				return *this;
			}
			bool operator&(bool value) const {
				return this->ptr->get(this->index) && value;
			}
			bitset_proxy operator^=(bool value) const {
				this->ptr->set(this->index, this->ptr->get(this->index) ^ value);
				return *this;
			}
			bool operator^(bool value) const {
				return this->ptr->get(this->index) ^ value;
			}

			bitset* ptr;
			qpl::size index;
		};

		class bitset_const_proxy {
		public:

			constexpr bitset_const_proxy(const bitset& data, qpl::size index) : ptr(&data), index(index) {

			}

			constexpr bool get() const {
				return this->ptr->get(this->index);
			}

			constexpr operator bool() const {
				return this->get();
			}
			bool operator|(bool value) const {
				return this->ptr->get(this->index) || value;
			}
			bool operator&(bool value) const {
				return this->ptr->get(this->index) && value;
			}
			bool operator^(bool value) const {
				return this->ptr->get(this->index) ^ value;
			}

			const bitset* ptr;
			qpl::size index;
		};

		class bitset_iterator {
		public:
			bitset_iterator(bitset& data, qpl::size index) {
				this->ptr = &data;
				this->index = index;
			}

			bitset_iterator& operator++() {
				++this->index;
				return *this;
			}
			bitset_iterator operator++(int dummy) {
				auto copy = *this;
				++this->index;
				return copy;
			}

			bool operator==(const bitset_iterator& other) const {
				return (this->ptr == other.ptr && this->index == other.index);
			}

			bool operator!=(const bitset_iterator& other) const {
				return !(*this == other);
			}

			const bitset_proxy& operator*() {
				return bitset_proxy(*ptr, index);
			}
			const bitset_const_proxy& operator*() const {
				return bitset_const_proxy(*ptr, index);
			}

			bitset* ptr;
			qpl::size index;
		};

		class bitset_const_iterator {
		public:
			constexpr bitset_const_iterator(const bitset& data, qpl::size index) : ptr(&data), index(index) {

			}

			constexpr bitset_const_iterator& operator++() {
				++this->index;
			}
			constexpr bitset_const_iterator operator++(int dummy) {
				auto copy = *this;
				++this->index;
				return copy;
			}

			constexpr bool operator==(const bitset_iterator& other) const {
				return (this->ptr == other.ptr && this->index == other.index);
			}

			constexpr bool operator!=(const bitset_iterator& other) const {
				return !(*this == other);
			}

			constexpr bitset_const_proxy& operator*() const {
				return bitset_const_proxy(*ptr, index);
			}

			const bitset* const ptr;
			qpl::size index;
		};

		constexpr static qpl::u64 memory_size() {
			return sizeof(holding_type) * qpl::bits_in_byte();
		}
		constexpr static qpl::size size() {
			return bits;
		}
		constexpr static bool is_array() {
			return bits > 64u;
		}
		constexpr static qpl::size array_size() {
			if (!is_array()) {
				return 0;
			}
			return qpl::approximate_multiple_up(bits, qpl::u64{ 64 }) / 64;
		}
		constexpr qpl::size number_of_set_bits() const {
			if constexpr (is_array()) {
				qpl::size size = 0u;
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					size += qpl::number_of_set_bits(this->data[i]);
				}
				return size;
			}
			else {
				return qpl::number_of_set_bits(this->data);
			}
		}


		constexpr bitset() {
			this->clear();
		}
		template<typename T>
		constexpr bitset(T value) : data() {
			this->set(value);
		}

		constexpr bitset& operator|=(const bitset& other) {
			if constexpr (is_array()) {
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					this->data[i] |= other.data[i];
				}
			}
			else {
				this->data |= other.data;
			}
			return *this;
		}
		constexpr bitset operator|(const bitset& other) const {
			auto result = *this;
			return result |= other;
		}

		constexpr bitset& operator&=(const bitset& other) {
			if constexpr (is_array()) {
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					this->data[i] &= other.data[i];
				}
			}
			else {
				this->data &= other.data;
			}
			return *this;
		}
		constexpr bitset operator&(const bitset& other) const {
			auto result = *this;
			return result &= other;
		}

		constexpr bitset& operator^=(const bitset& other) {
			if constexpr (is_array()) {
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					this->data[i] ^= other.data[i];
				}
			}
			else {
				this->data ^= other.data;
			}
			return *this;
		}
		constexpr bitset operator^(const bitset& other) const {
			auto result = *this;
			return result ^= other;
		}

		constexpr static auto last_bits_mask() {
			return (uint_type{ 1 } << (bits % qpl::bits_in_type<uint_type>())) - 1;
		}
		constexpr void clear() {
			if constexpr (is_array()) {
				for (auto& i : this->data) {
					i = uint_type{ 0u };
				}
			}
			else {
				this->data = uint_type{ 0u };
			}
		}
		constexpr void fill(bool n) {
			if (n == false) {
				this->clear();
			}
			else {
				if constexpr (is_array()) {
					for (qpl::size i = 0u; i < this->data.size() - 1; ++i) {
						this->data[i] = qpl::type_max<uint_type>();
					}
					this->data.back() = last_bits_mask();
				}
				else {
					this->data = last_bits_mask();
				}
			}
		}

		constexpr bool operator==(const bitset& other) const {
			if constexpr (is_array()) {
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					if (this->data[i] != other.data[i]) {
						return false;
					}
				}
				return true;
			}
			else {
				return this->data == other.data;
			}
		}
		constexpr bool operator!=(const bitset& other) const {
			return !(*this == other);
		}

		constexpr auto get_masked_data() const requires (!is_array()) {
			return this->data & last_bits_mask();
		}
		constexpr qpl::size significant_bit() const {
			if constexpr (is_array()) {
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					auto index = this->data.size() - i - 1;
					if (this->data[index]) {
						return qpl::significant_bit(this->data[index]) + (i * qpl::bits_in_type<uint_type>());
					}
				}
			}
			else {
				return qpl::significant_bit(this->get_masked_data());
			}
		}
		constexpr bool get(qpl::size index) const {
			if constexpr (is_array()) {
				return qpl::get_bit(this->data[index / 64u], index % 64u);
			}
			else {
				return qpl::get_bit(this->data, index);
			}
		}
		constexpr void set(qpl::size index, bool value) {
			if constexpr (is_array()) {
				qpl::set_bit(this->data[index / 64u], index % 64u, value);
			}
			else {
				qpl::set_bit(this->data, index, value);
			}
		}
		constexpr void set(holding_type data) {
			this->data = data;
		}

		template<typename T>
		constexpr void set_range(qpl::u32 begin, qpl::u32 end, T value) {
			if constexpr (is_array()) {
				auto div = begin / 64u;

				auto extract = (value & (~T{} >> (qpl::bits_in_type<T>() - ((end - (div * 64)) - (begin - (div * 64))) - 1)));
				this->data[div] |= extract << (begin - (div * 64));
			}
			else {
				auto extract = (value & (~T{} >> (qpl::bits_in_type<T>() - (end - begin) - 1)));
				this->data |= extract << begin;
			}
		}
		constexpr uint_type get_range(qpl::u32 begin, qpl::u32 end) const {

			if constexpr (is_array()) {
				auto div = begin / 64u;

				auto shift = this->data[div] >> (begin - (div * 64));
				return (shift & (~uint_type{} >> (qpl::bits_in_type<uint_type>() - ((end - (div * 64)) - (begin - (div * 64))) - 1)));
			}
			else {
				auto shift = this->data >> begin;
				return (shift & (~uint_type{} >> (qpl::bits_in_type<uint_type>() - (end - begin) - 1)));
			}
		}

		constexpr bool empty() const {
			if constexpr (is_array()) {
				for (auto& i : this->data) {
					if (i) {
						return false;
					}
				}
				return true;
			}
			else {
				return this->data == holding_type{ 0 };
			}
		}
		constexpr bool full() const {
			if constexpr (is_array()) {
				for (qpl::size i = 0u; i < this->data.size() - 1; ++i) {
					if (this->data[i] != qpl::u64_max) {
						return false;
					}
				}
				if (this->data.back() != last_bits_mask()) {
					return false;
				}
				return true;
			}
			else {
				return this->data == last_bits_mask();
			}
		}

		std::string string() const {
			if constexpr (is_array()) {
				std::ostringstream stream;
				stream << qpl::prepended_to_string_to_fit(qpl::binary_string(this->data.back()), '0', this->size() % qpl::bits_in_type<qpl::u64>());
				for (qpl::u32 i = 0u; i < this->data.size() - 1; ++i) {
					stream << qpl::binary_string_full(this->data[this->data.size() - 2 - i]);
				}
				return stream.str();
			}
			else {
				auto str = qpl::binary_string(this->data);
				return qpl::prepended_to_string_to_fit(str, '0', this->size());
			}
		}
		std::string string_full() const {
			if constexpr (is_array()) {
				std::ostringstream stream;
				for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
					stream << qpl::binary_string_full(this->data[this->data.size() - 1 - i]);
				}
				return stream.str();
			}
			else {
				auto str = qpl::binary_string(this->data);
				return qpl::prepended_to_string_to_fit(str, '0', this->size());
			}
		}
		std::string hex_string_full(bool seperated = false) const {
			if constexpr (is_array()) {

				std::ostringstream stream;

				if (seperated) {
					for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
						if (i) stream << ", ";
						stream << qpl::hex_string_full(this->data[this->data.size() - 1 - i]);
					}
				}
				else {
					stream << "0x";
					for (qpl::u32 i = 0u; i < this->data.size(); ++i) {
						stream << qpl::hex_string_full(this->data[this->data.size() - 1 - i], "");
					}
				}
				return stream.str();
			}
			else {
				auto str = qpl::hex_string(this->data, "");
				return qpl::to_string("0x", qpl::prepended_to_string_to_fit(str, '0', this->size()));
			}
		}


		constexpr bitset_iterator begin() {
			return bitset_iterator(*this, 0u);
		}
		constexpr bitset_const_iterator begin() const {
			return bitset_const_iterator(*this, 0u);
		}
		constexpr bitset_const_iterator cbegin() const {
			return bitset_const_iterator(*this, 0u);
		}

		constexpr bitset_iterator end() {
			return bitset_iterator(*this, this->size());
		}
		constexpr bitset_const_iterator end() const {
			return bitset_const_iterator(*this, this->size());
		}
		constexpr bitset_const_iterator cend() const {
			return bitset_const_iterator(*this, this->size());
		}


		constexpr bitset_proxy operator[](qpl::size index) {
			return bitset_proxy(*this, index);
		}
		constexpr bitset_const_proxy operator[](qpl::size index) const {
			bitset_const_proxy result(*this, index);
			return result;
		}

		constexpr bitset_range_proxy operator[](std::pair<qpl::u32, qpl::u32> range) {
			return bitset_range_proxy(*this, range.first, range.second);
		}
		constexpr bitset_range_const_proxy operator[](std::pair<qpl::u32, qpl::u32> range) const {
			bitset_range_const_proxy result(*this, range.first, range.second);
			return result;
		}

		constexpr bitset_proxy front() {
			return bitset_proxy(*this, 0u);
		}
		constexpr bitset_const_proxy front() const {
			return bitset_const_proxy(*this, 0u);
		}

		constexpr bitset_proxy back() {
			return bitset_proxy(*this, this->size() - 1);
		}
		constexpr bitset_const_proxy back() const {
			return bitset_const_proxy(*this, this->size() - 1);
		}

		holding_type data;
	};

	struct double_content {
		double_content(double value = 0.0) {
			this->from_double(value);
		}
		constexpr static qpl::size mantissa_size() {
			return qpl::f64_mantissa_size();
		}
		constexpr static qpl::size exponent_size() {
			return qpl::f64_exponent_size();
		}

		qpl::u64 mantissa : 52;
		qpl::u64 exponent : 11;
		qpl::u64 sign : 1;

		double to_double() const {
			double result;
			memcpy(&result, this, sizeof(double));
			return result;
		}
		qpl::u64 to_u64() const {
			qpl::u64 value;
			memcpy(&value, this, sizeof(double));
			return value;
		}
		void from_double(double value) {
			memcpy(this, &value, sizeof(double));
		}
		std::string binary_string() const {
			return qpl::to_string(this->sign, ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->exponent)), '0', 11), ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->mantissa)), '0', 52));
		}
		std::string hex_string() const {
			return qpl::to_string(this->sign, ' ', qpl::prepended_to_string_to_fit(qpl::hex_string(qpl::u64_cast(this->exponent)), '0', 3), ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->mantissa)), '0', 13));
		}
	};
	struct float_content {
		float_content(float value = 0.f) {
			this->from_float(value);
		}

		constexpr static qpl::size mantissa_size() {
			return qpl::f32_mantissa_size();
		}
		constexpr static qpl::size exponent_size() {
			return qpl::f32_exponent_size();
		}
		float to_float() const {
			float result;
			memcpy(&result, this, sizeof(float));
			return result;
		}
		qpl::u32 to_u32() const {
			qpl::u32 value;
			memcpy(&value, this, sizeof(float));
			return value;
		}
		void from_float(float value) {
			memcpy(this, &value, sizeof(float));
		}
		std::string binary_string() const {
			return qpl::to_string(this->sign, ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->exponent)), '0', 8), ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->mantissa)), '0', 23));
		}
		qpl::u32 mantissa : 23;
		qpl::u32 exponent : 8;
		qpl::u32 sign : 1;
	};


	struct bit_string_ostream {
		mutable qpl::u64 value = 0u;
		mutable qpl::size bit_position = qpl::size_cast(qpl::bits_in_type<qpl::u64>());
		mutable qpl::size position = 0u;
		mutable std::ostringstream stream;

		bit_string_ostream() {

		}
		bit_string_ostream(const bit_string_ostream& other) {
			*this = other;
		}

		QPLDLL void clear() const;
		QPLDLL std::string string() const;
		QPLDLL void finish() const;
		QPLDLL void reset_position();
		QPLDLL void write() const;
		QPLDLL bit_string_ostream& operator=(const bit_string_ostream& other);


		template<typename T, typename U>
		void add_bits(T n, U width) {

			if (width > this->bit_position) {

				auto left_over = width - this->bit_position;
				width = static_cast<U>(this->bit_position);

				this->value |= (qpl::u64_cast(n) >> left_over);

				this->write();
				this->bit_position = qpl::bits_in_type<qpl::u64>() - left_over;
				this->value = (qpl::u64_cast(n) << this->bit_position);
			}
			else {
				this->bit_position -= qpl::size_cast(width);

				this->value |= (qpl::u64_cast(n) << this->bit_position);
			}
			if (this->bit_position == 0) {
				this->write();
				this->value = 0;
				this->bit_position = qpl::bits_in_type<qpl::u64>();
			}
			this->position += qpl::size_cast(width);
		}

		template<typename T> requires (!qpl::is_same<T, std::string>())
			void add_bits(T n) {
			this->add_bits(n, qpl::bits_in_type<T>());
		}

		template<typename T> requires (qpl::is_arithmetic<T>())
			void add(T n) {
			this->finish();
			this->stream.write(reinterpret_cast<char*>(&n), qpl::bytes_in_type<T>());
			this->position += qpl::bits_in_type<T>();
		}
		QPLDLL void add(const std::string& n);


	};


	struct bit_string_istream {
		std::string string;
		qpl::size position = 0u;
		qpl::size position_u64_offset = 0u;

		QPLDLL qpl::size size() const;
		QPLDLL void set(const std::string& string);
		QPLDLL void reset_position();
		QPLDLL std::string get_next_string(qpl::size size);
		QPLDLL bool is_done() const;
		QPLDLL void note_u64_position_offset();
		QPLDLL void set_position_next_u64_multiple();
		QPLDLL void set_position(qpl::size position);

		template<typename T, typename U>
		T get_next_bits(U width) {
			qpl::u64 left = 0;
			qpl::u64 right = 0;

			auto pos = (this->position - this->position_u64_offset * qpl::bits_in_byte()) / (qpl::bits_in_type<qpl::u64>()) * qpl::bits_in_byte() + this->position_u64_offset;
			qpl::i64 min = qpl::min(qpl::i64_cast(qpl::bytes_in_type<qpl::u64>()), qpl::i64_cast(this->string.length() - pos));
			if (min <= 0) {
				this->position = qpl::u32_cast(this->string.length() * qpl::bits_in_byte());
				return T{ 0 };
			}
			memcpy(&left, this->string.data() + pos, qpl::size_cast(min));
			min = qpl::min(qpl::i64_cast(qpl::bytes_in_type<qpl::u64>()), qpl::i64_cast(this->string.length() - pos - qpl::i64_cast(qpl::bytes_in_type<qpl::u64>())));

			auto mod = (this->position + (qpl::bytes_in_type<qpl::u64>() - this->position_u64_offset) * qpl::bits_in_byte()) % qpl::bits_in_type<qpl::u64>();
			auto lshift = qpl::bits_in_type<qpl::u64>() - mod;
			
			if (min > 0 && mod > width) {
				memcpy(&right, this->string.data() + pos + qpl::bytes_in_type<qpl::u64>(), qpl::size_cast(min));
			}
			qpl::u64 value = (left << mod) | (right >> lshift);
			value >>= qpl::bits_in_type<qpl::u64>() - width;

			this->position += qpl::size_cast(width);
			return static_cast<T>(value);
		}

		template<typename T>
		T get_next(qpl::u32 size = qpl::bytes_in_type<T>()) {

			T value;
			memcpy(&value, this->string.data() + this->position / qpl::bits_in_byte(), qpl::bytes_in_type<T>());
			auto l = qpl::bytes_in_type<T>() - size;
			if (l) {
				value >>= l * qpl::bits_in_byte();
			}

			this->position += size * qpl::bits_in_byte();
			return value;
		}
	};
}

#endif