#ifndef QPL_BITS_HPP
#define QPL_BITS_HPP
#pragma once

#include <algorithm>
#include <qpl/algorithm.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>
#include <array>

namespace qpl {
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
			qpl::if_true<(bits <= 64)>, qpl::ubit<bits>,
			qpl::default_type, std::array<qpl::u64, qpl::approximate_multiple_up(bits, qpl::u64{ 64 })>>;
		
		constexpr static qpl::u64 actual_bit_size() {
			return sizeof(holding_type) * qpl::bits_in_byte();
		}
		constexpr static qpl::u64 bit_size() {
			return bits;
		}
		constexpr static bool is_array() {
			return bits > 64;
		}
	private:
		holding_type m_data;
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