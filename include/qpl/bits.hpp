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
		private:
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
		private:
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
		private:
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
		private:
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

			const bitset_proxy&& operator*() {
				return bitset_proxy(*ptr, index);
			}
			const bitset_const_proxy&& operator*() const {
				return bitset_const_proxy(*ptr, index);
			}

		private:
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

		private:
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
		constexpr qpl::size number_of_set_bits() {
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

		constexpr void clear() {
			if constexpr (is_array()) {
				for (auto& i : this->data) {
					i = 0u;
				}
			}
			else {
				this->data = 0u;
			}
		}
		constexpr void fill(bool n) {
			if (n == false) {
				this->clear();
			}
			else {
				if constexpr (is_array()) {
					for (auto& i : this->data) {
						i = 1u;
					}
				}
				else {
					this->data = qpl::type_max<qpl::ubit<bits>>();
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
				for (auto& i : this->data) {
					if (i != qpl::u64_max) {
						return false;
					}
				}
				return true;
			}
			else {
				return this->data == ~holding_type{ 0 };
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
		double_content() {

		}
		double_content(double value) {
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
		void from_double(double value) {
			memcpy(this, &value, sizeof(double));
		}
		std::string binary_string() const {
			return qpl::to_string(this->sign, ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->exponent)), '0', 11), ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->mantissa)), '0', 52));
		}
	};
	struct float_content {
		float_content() {

		}
		float_content(float value) {
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

}

#endif