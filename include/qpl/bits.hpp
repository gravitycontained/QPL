#ifndef QPL_BITS_HPP
#define QPL_BITS_HPP
#pragma once

#include <algorithm>
#include <qpl/algorithm.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/string.hpp>
#include <qpl/vardef.hpp>
#include <array>

namespace qpl {
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 kilobyte(T n) {
		return qpl::u64_cast(1000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 kibibyte(T n) {
		return qpl::u64_cast(0x400ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 megabyte(T n) {
		return qpl::u64_cast(1000'000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 mebibyte(T n) {
		return qpl::u64_cast(0x100000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 gigabyte(T n) {
		return qpl::u64_cast(1000'000'000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 gebibyte(T n) {
		return qpl::u64_cast(0x40000000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::u64 terabyte(T n) {
		return qpl::u64_cast(1'000'000'000'000ull * n);
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
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



	template<qpl::u64 bits>
	using fbit = conditional <
		qpl::if_true<(bits == 32ull)>, qpl::f32,
		qpl::if_true<(bits == 64ull)>, qpl::f64,
		qpl::default_type, qpl::floating_point<32u, bits>>;


	template<qpl::u64 bits>
	class bitset {
	public:

		using holding_type = qpl::conditional<
			qpl::if_true<(bits <= 64u)>, qpl::ubit<bits>,
			qpl::default_type, std::array<qpl::u64, qpl::approximate_multiple_up(bits, qpl::u64{ 64 }) / 64>>;


		class bitset_proxy {
		public:
			bitset_proxy(bitset& data, qpl::u32 index) {
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
		private:
			bitset* ptr;
			qpl::u32 index;
		};

		class bitset_const_proxy {
		public:

			constexpr bitset_const_proxy(const bitset& data, qpl::u32 index) : ptr(&data), index(index) {

			}

			constexpr bool get() const {
				return this->ptr->get(this->index);
			}

			constexpr operator bool() const {
				return this->get();
			}
		private:
			const bitset* ptr;
			qpl::u32 index;
		};

		class bitset_iterator {
		public:
			bitset_iterator(bitset& data, qpl::u32 index) {
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
			qpl::u32 index;
		};

		class bitset_const_iterator {
		public:
			constexpr bitset_const_iterator(const bitset& data, qpl::u32 index) : ptr(&data), index(index) {

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
			qpl::u32 index;
		};

		constexpr static qpl::u64 memory_size() {
			return sizeof(holding_type) * qpl::bits_in_byte();
		}
		constexpr static qpl::u64 size() {
			return bits;
		}
		constexpr static bool is_array() {
			return bits > 64u;
		}

		constexpr bitset() {
			this->clear();
		}
		template<typename T>
		constexpr bitset(T value) : data() {
			this->set(value);
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

		constexpr bool get(qpl::u32 index) const {
			if constexpr (is_array()) {
				return qpl::get_bit(this->data[index / 64u], index % 64u);
			}
			else {
				return qpl::get_bit(this->data, index);
			}
		}
		constexpr void set(qpl::u32 index, bool value) {
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
				auto str = qpl::hex_string(this->data);
				return qpl::prepended_to_string_to_fit(str, '0', this->size());
			}
		}


		bitset_iterator begin() {
			return bitset_iterator(*this, 0u);
		}
		constexpr bitset_const_iterator begin() const {
			return bitset_const_iterator(*this, 0u);
		}
		constexpr bitset_const_iterator cbegin() const {
			return bitset_const_iterator(*this, 0u);
		}

		bitset_iterator end() {
			return bitset_iterator(*this, this->size());
		}
		constexpr bitset_const_iterator end() const {
			return bitset_const_iterator(*this, this->size());
		}
		constexpr bitset_const_iterator cend() const {
			return bitset_const_iterator(*this, this->size());
		}



		bitset_proxy operator[](qpl::u32 index) {
			return bitset_proxy(*this, index);
		}
		constexpr bitset_const_proxy operator[](qpl::u32 index) const {
			bitset_const_proxy result(*this, index);
			return result;
		}

		bitset_proxy front() {
			return bitset_proxy(*this, 0u);
		}
		constexpr bitset_const_proxy front() const {
			return bitset_const_proxy(*this, 0u);
		}

		bitset_proxy back() {
			return bitset_proxy(*this, this->size() - 1);
		}
		constexpr bitset_const_proxy back() const {
			return bitset_const_proxy(*this, this->size() - 1);
		}

	private:
		holding_type data;
	};

	struct double_content {
		double_content() {

		}
		double_content(double value) {
			memcpy(this, &value, sizeof(double));
		}
		constexpr static qpl::size mantissa_size() {
			return 52;
		}
		constexpr static qpl::size exponent_size() {
			return 11;
		}

		qpl::u64 mantissa : 52;
		qpl::u64 exponent : 11;
		qpl::u64 sign : 1;

		double to_double() const {
			double result;
			memcpy(&result, this, sizeof(double));
			return result;
		}
		std::string binary_string() const {
			return qpl::to_string(this->sign, ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->exponent)), '0', 11), ' ', qpl::prepended_to_string_to_fit(qpl::binary_string(qpl::u64_cast(this->mantissa)), '0', 52));
		}
	};
	struct float_content {
		float_content() {

		}
		float_content(float value) {
			memcpy(this, &value, sizeof(float));
		}
		constexpr static qpl::size mantissa_size() {
			return 23;
		}
		constexpr static qpl::size exponent_size() {
			return 8;
		}
		float to_float() const {
			float result;
			memcpy(&result, this, sizeof(float));
			return result;
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