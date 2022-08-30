#ifndef QPL_ALGORITHM_HPP
#define QPL_ALGORITHM_HPP
#pragma once


#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <array>
#include <iterator>
#include <iostream>
#include <functional>
#include <tuple>
#include <span>
#include <set>
#include <unordered_set>


namespace qpl {

	template<typename T>
	constexpr T min(T a, T b) {
		return (a < b ? a : b);
	}
	template<typename T, typename ...Args>
	constexpr T min(T a, T b, Args... rest) {
		return qpl::min(qpl::min(a, b), rest...);
	}

	template<typename T>
	constexpr T max(T a, T b) {
		return (a > b ? a : b);
	}
	template<typename T, typename ...Args>
	constexpr T max(T a, T b, Args... rest) {
		return qpl::max(qpl::max(a, b), rest...);
	}

	template<typename T, typename U>
	constexpr auto div_mod(T a, U b) {
		auto div = a / b;
		auto mod = a % b;
		return std::make_pair(div, mod);
	}

	template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::size number_of_digits(T value, T base = T{ 10 }) {
		return value < base ? 1 : 1 + number_of_digits(value / base, base);
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T approximate_multiple_down(T value, T multiple) {
		return (value / multiple) * multiple;
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T approximate_multiple_up(T value, T multiple) {
		return (((value - 1) / multiple) + 1) * multiple;
	}
	template<typename T, typename U, typename R = qpl::superior_arithmetic_type<T, U>>
	constexpr R pow(T a, U b) {
		return b == U{} ? R{ 1 } : static_cast<R>(a * qpl::pow(a, b - T{ 1 }));
	}
	template<typename T>
	constexpr T square(T value) {
		return value * value;
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T factorial(T a) {
		return a <= T{ 1 } ? T{ 1 } : (a == T{ 2 } ? T{ 2 } : static_cast<T>(a * qpl::factorial(a - 1)));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T log(T a, T b) {
		return b < a ? T{} : T{ 1 } + qpl::log(a, static_cast<T>(b / a));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr qpl::i64 log2(T a) {
		return a ? qpl::i64{ 1 } + qpl::log2(static_cast<T>(a >> 1)) : qpl::i64{ -1 };
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T shift(T a, T b) {
		return b == T{} ? T{ 1 } : static_cast<T>(a * qpl::pow<T>(a, b - T{ 1 }));
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr qpl::size significant_digit(T n, T base = 10) {
		return n ? qpl::size{ 1 } + qpl::significant_digit(static_cast<T>(n / base), base) : qpl::size{};
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr qpl::size significant_bit(T n) {
		return n ? qpl::size{ 1 } + qpl::significant_bit(static_cast<T>(n >> 1)) : qpl::size{};
	}
	template<>
	constexpr qpl::size significant_bit(qpl::u8 n) {
		qpl::u8 y = 0;

		qpl::size result = 0u;
		if ((y =(n & qpl::u8{ 0xf0u }))) { result |= qpl::size{ 0b100u }; n = y; }
		if ((y =(n & qpl::u8{ 0xccu }))) { result |= qpl::size{ 0b10u }; n = y; }
		if (((n & qpl::u8{ 0xaau }))) { result |= qpl::size{ 0b1u }; }
		if (n) {
			++result;
		}
		return result;
	}
	template<>
	constexpr qpl::size significant_bit(qpl::u16 n) {
		qpl::u16 y = 0;

		qpl::size result = 0u;
		if ((y = (n & qpl::u16{ 0xff00u }))) { result |= qpl::u16{ 0b1000u }; n = y; }
		if ((y = (n & qpl::u16{ 0xf0f0u }))) { result |= qpl::u16{ 0b100u }; n = y; }
		if ((y = (n & qpl::u16{ 0xccccu }))) { result |= qpl::u16{ 0b10u }; n = y; }
		if (((n & qpl::u16{ 0xaaaau }))) { result |= qpl::u16{ 0b1u }; }
		if (n) {
			++result;
		}
		return result;
	}
	template<>
	constexpr qpl::size significant_bit(qpl::u32 n) {
		qpl::u32 y = 0;

		qpl::size result = 0u;
		if ((y = (n & 0xffff0000u))) { result |= 0b10000u; n = y; }
		if ((y = (n & 0xff00ff00u))) { result |= 0b1000u; n = y; }
		if ((y = (n & 0xf0f0f0f0u))) { result |= 0b100u; n = y; }
		if ((y = (n & 0xccccccccu))) { result |= 0b10u; n = y; }
		if (((n & 0xaaaaaaaau))) { result |= 0b1u; }
		if (n) {
			++result;
		}
		return result;
	}
	template<>
	constexpr qpl::size significant_bit(qpl::u64 n) {
		qpl::u64 y = 0;

		qpl::size result = 0u;
		if ((y = (n & 0xffffffff00000000ull))) { result |= 0b100000u; n = y; }
		if ((y = (n & 0xffff0000ffff0000ull))) { result |= 0b10000u; n = y; }
		if ((y = (n & 0xff00ff00ff00ff00ull))) { result |= 0b1000u; n = y; }
		if ((y = (n & 0xf0f0f0f0f0f0f0f0ull))) { result |= 0b100u; n = y; }
		if ((y = (n & 0xccccccccccccccccull))) { result |= 0b10u; n = y; }
		if (((n & 0xaaaaaaaaaaaaaaaaull))) { result |= 0b1u; }
		if (n) {
			++result;
		}
		return result;
	}

	constexpr qpl::size significant_bit_ignore_last_bit(qpl::u32 n) {
		uint32_t y = 0;

		qpl::size result = 0u;
		if ((y = (n & 0x7fff0000U))) { result |= 0b10000u; n = y; }
		if ((y = (n & 0x7f00ff00U))) { result |= 0b1000u; n = y; }
		if ((y = (n & 0x70f0f0f0U))) { result |= 0b100u; n = y; }
		if ((y = (n & 0x4cccccccU))) { result |= 0b10u; n = y; }
		if (((n & 0x2aaaaaaaU))) { result |= 0b1u; }
		if (n) {
			++result;
		}
		return result;
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr qpl::size number_of_set_bits(T n) {
		return qpl::number_of_set_bits(qpl::u64_cast(n));
	}
	template<>
	constexpr qpl::size number_of_set_bits(qpl::u64 n) {
		n = n - ((n >> 1) & 0x5555'5555'5555'5555ull);
		n = (n & 0x3333'3333'3333'3333ull) + ((n >> 2) & 0x3333'3333'3333'3333ull);
		return (((n + (n >> 4)) & 0x0F0F'0F0F'0F0F'0F0Full) * 0x0101'0101'0101'0101ull) >> 56;
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr bool get_bit(T value, qpl::size position) {
		return qpl::bool_cast(value & (T{ 1 } << position));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr void set_bit(T& value, qpl::size position, bool flag) {
		value = flag ? (value | (T{ 1 } << position)) : (value & ~(T{ 1 } << position));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T set_bit_copied(T value, qpl::size position, bool flag) {
		return flag ? (value | (T{ 1 } << position)) : (value & ~(T{ 1 } << position));
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr void toggle_bit(T& value, qpl::size position) {
		value = (value & (T{ 1 } << position)) ? (value & ~(T{ 1 } << position)) : (value | (T{ 1 } << position));
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_bits(T& value) {
		value = ~value;
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_bits(T value) {
		return ~value;
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_bits_until(T& value, qpl::size position) {
		value = (value & (~T{} << position)) | (~value & ~(~T{} << position));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_bits_until(T value, qpl::size position) {
		return (value & (~T{} << position)) | (~value & ~(~T{} << position));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_bits_until_msb(T& value) {
		qpl::flip_bits_until(value, qpl::significant_bit(value));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_bits_until_msb(T value) {
		return qpl::flipped_bits_until(value, qpl::significant_bit(value));
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr void reverse_bits(T& value) {

		if constexpr (qpl::is_same<T, qpl::u32>()) {
			value = ((value >> 1) & 0x55555555u) | ((value & 0x55555555u) << 1);
			value = ((value >> 2) & 0x33333333u) | ((value & 0x33333333u) << 2);
			value = ((value >> 4) & 0x0f0f0f0fu) | ((value & 0x0f0f0f0fu) << 4);
			value = ((value >> 8) & 0x00ff00ffu) | ((value & 0x00ff00ffu) << 8);
			value = ((value >> 16) & 0xffffu) | ((value & 0xffffu) << 16);
			return;
		}

		qpl::size count = qpl::bits_in_type<T>() - 1;
		T reverse_num = value;

		reverse_num >>= 1;
		while (reverse_num) {
			value <<= 1;
			value |= reverse_num & 1;
			reverse_num >>= 1;
			--count;
		}
		value <<= count;
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr T reverse_bits_copied(T value) {
		T copy;
		qpl::reverse_bits(copy);
		return copy;
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr T get_digit(T value, qpl::size position, T base = 10) {
		return static_cast<T>((value / qpl::pow<qpl::size>(base, position)) % base);
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr void set_digit(T& value, qpl::size position, T digit, T base = 10) {
		auto pow = qpl::pow<qpl::size>(base, position);
		auto original_digit = static_cast<T>((value / pow) % base);
		if (original_digit >= digit) {
			value -= (original_digit - digit) * pow;
		}
		else {
			value += (digit - original_digit) * pow;
		}
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T set_digit_copied(T value, qpl::size position, T digit, T base = 10) {
		auto copy = value;
		qpl::set_digit(copy, position, digit, base);
		return copy;
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr T log_approximation(T a, T b, T c = T{ 1 }) {
		return c > static_cast<T>(b / a) ? c : qpl::log_approximation(a, b, static_cast<T>(c * a));
	}



	template<typename T> requires (qpl::is_integer<T>())
		constexpr bool is_power_of_two(T n) {
		return n == T{} ? false : (n == (T{ 1 } << (qpl::significant_bit(n) - 1)));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr bool base_full_bit_usage(T base) {
		return is_power_of_two(base) ? qpl::bits_in_type<T>() % (qpl::significant_bit(base) - 1) == 0 : false;
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr T base_max_log(T base) {
		return qpl::base_full_bit_usage(base) ? static_cast<T>(qpl::bits_in_type<T>() / (qpl::significant_bit(base) - 1)) : qpl::log<T>(base, T{ 1 } << (qpl::bits_in_type<T>() - 1));
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr T base_max(T base) {
		return qpl::base_full_bit_usage(base) ? T{} : qpl::pow(base, qpl::base_max_log(base));
	}

	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_digits(T& value, qpl::u32 base = 10) {
		value = qpl::base_max<T>(static_cast<T>(base)) - 1 - value;
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_digits(T value, qpl::u32 base = 10) {
		return qpl::base_max<T>(static_cast<T>(base)) - 1 - value;
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_digits_until(T& value, qpl::size position, qpl::u32 base = 10) {
		auto pow = qpl::pow<qpl::size>(base, position);
		value = (value + (pow - 1 - (value % pow) * 2));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_digits_until(T value, qpl::size position, qpl::u32 base = 10) {
		auto pow = qpl::pow<qpl::size>(base, position);
		return (value + (pow - 1 - (value % pow) * 2));
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr void flip_digits_until_msd(T& value, qpl::u32 base = 10) {
		qpl::flip_digits_until(value, qpl::significant_digit(value, base), base);
	}
	template<typename T> requires (qpl::is_integer<T>())
		constexpr T flipped_digits_until_msd(T value, qpl::u32 base = 10) {
		return qpl::flipped_digits_until(value, qpl::significant_digit(value, base), base);
	}


	template<typename T, typename R = qpl::unsigned_type<T>> requires (qpl::is_integer<T>())
	R constexpr cantor_pairing(T x, T y) {
		auto rx = static_cast<R>(x);
		auto ry = static_cast<R>(y);
		return (rx + ry) * (rx + ry + 1) / 2 + ry;
	}
	template<typename T, typename R = qpl::signed_type<T>> requires (qpl::is_integer<T>())
	std::pair<R, R> cantor_pairing_inverse(T z) {
		std::pair<R, R> result;
		auto w = static_cast<R>((std::sqrt(R{ 8 } *z + R{ 1 }) - R{ 1 }) / R{ 2 });
		result.second = z - (w + R{ 1 }) * w / R{ 2 };
		result.first = w - result.second;
		return result;
	}

	template<typename T> requires (qpl::is_integer<T>())
		T constexpr rosenberg_pairing(T x, T y) {
		return ((x > y ? x : y) * (x > y ? x : y)) + (x > y ? x : y) + x - y;
	}
	template<typename T> requires (qpl::is_integer<T>())
		std::pair<T, T> rosenberg_pairing_inverse(T z) {
		auto m = static_cast<T>(std::sqrt(z));
		if (z - m * m < m) {
			return std::make_pair(z - m * m, m);
		}
		else {
			return std::make_pair(m, m * m + T{ 2 } *m - z);
		}
	}

	template<typename T, typename R = qpl::conditional<qpl::if_true<qpl::is_floating_point<T>()>, T, qpl::f64>> requires(qpl::is_arithmetic<T>())
	constexpr R radians(T value) {
		return static_cast<R>(value * (qpl::pi / R{ 180 }));
	}


	template<typename T> requires (qpl::is_integer<T>())
		constexpr T binary_configurations(T n) {
		return T{ 1 } << n;
	}
	template<typename T>
	constexpr qpl::size type_configurations() {
		return qpl::binary_configurations<qpl::size>(qpl::bits_in_type<T>());
	}
	template<typename T>
	constexpr T type_max() {
		return std::numeric_limits<T>::max();
	}
	template<typename T>
	constexpr T type_min() {
		return std::numeric_limits<T>::min();
	}

	template<typename T>
	std::string type_to_string() {
		return typeid(T).name();
	}


	template<class Op, class compare, class... Args>
	constexpr bool is_operation_to_any(Op op, compare first, Args... rest) {
		return (op(first, rest) || ...);
	}
	template<class compare, class... Args>
	constexpr bool is_any_type_equal_to(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::equal_to{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_not_equal_to_any(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::not_equal_to{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_less_to_any(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::less{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_less_or_equal_to_any(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::less_equal{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_greater_to_any(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::greater{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_greater_or_equal_to_any(compare first, Args... rest) {
		return qpl::is_operation_to_any(std::greater_equal{}, first, rest...);
	}

	template<class Op, class compare, class... Args>
	constexpr bool is_operation_to_all(Op op, compare first, Args... rest) {
		return (op(first, rest) && ...);
	}
	template<class compare, class... Args>
	constexpr bool is_equal_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::equal_to{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_not_equal_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::not_equal_to{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_less_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::less{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_less_or_equal_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::less_equal{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_greater_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::greater{}, first, rest...);
	}
	template<class compare, class... Args>
	constexpr bool is_greater_or_equal_to_all(compare first, Args... rest) {
		return qpl::is_operation_to_all(std::greater_equal{}, first, rest...);
	}

	template<class Op, class... Args>
	constexpr bool is_any_operation_to(Op op, Args... args) {
		if constexpr (sizeof...(Args) == 1) {
			return false;
		}
		else {
			auto tuple = std::tuple(args...);

			constexpr auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
				return qpl::is_operation_to_any(op, std::get<sizeof...(Ints)>(tuple), std::get<Ints>(tuple)...);
			};
			return unpack_tuple(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)> -1>());
		}
	}

	template<class... Args>
	constexpr bool is_any_equal_to(Args... args) {
		return qpl::is_any_operation_to(std::equal_to{}, args...);
	}
	template<class... Args>
	constexpr bool is_any_not_equal_to(Args... args) {
		return qpl::is_any_operation_to(std::not_equal_to{}, args...);
	}
	template<class... Args>
	constexpr bool is_any_less_to(Args... args) {
		return qpl::is_any_operation_to(std::greater_equal{}, args...);
	}
	template<class... Args>
	constexpr bool is_any_less_or_equal_to(Args... args) {
		return qpl::is_any_operation_to(std::greater{}, args...);
	}
	template<class... Args>
	constexpr bool is_any_greater_to(Args... args) {
		return qpl::is_any_operation_to(std::less_equal{}, args...);
	}
	template<class... Args>
	constexpr bool is_any_greater_or_equal_to(Args... args) {
		return qpl::is_any_operation_to(std::less{}, args...);
	}


	template<class Op, class... Args>
	constexpr bool is_all_operation_to(Op op, Args... args) {
		if constexpr (sizeof...(Args) == 1) {
			return false;
		}
		else {
			auto tuple = std::tuple(args...);

			constexpr auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
				return qpl::is_operation_to_all(op, std::get<sizeof...(Ints)>(tuple), std::get<Ints>(tuple)...);
			};
			return unpack_tuple(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)> -1>());
		}
	}

	template<class... Args>
	constexpr bool is_all_equal_to(Args... args) {
		return qpl::is_all_operation_to(std::equal_to{}, args...);
	}
	template<class... Args>
	constexpr bool is_all_not_equal_to(Args... args) {
		return qpl::is_all_operation_to(std::not_equal_to{}, args...);
	}
	template<class... Args>
	constexpr bool is_all_less_to(Args... args) {
		return qpl::is_all_operation_to(std::less{}, args...);
	}
	template<class... Args>
	constexpr bool is_all_less_or_equal_to(Args... args) {
		return qpl::is_all_operation_to(std::less_equal{}, args...);
	}
	template<class... Args>
	constexpr bool is_all_greater_to(Args... args) {
		return qpl::is_all_operation_to(std::greater{}, args...);
	}
	template<class... Args>
	constexpr bool is_all_greater_or_equal_to(Args... args) {
		return qpl::is_all_operation_to(std::greater_equal{}, args...);
	}


	template<typename C> requires(qpl::is_container<C>())
		constexpr auto begin(C& container) {
		if constexpr (qpl::is_read_container<C>() && qpl::is_write_container<C>()) {
			return container.begin();
		}
		else if constexpr (qpl::is_read_container<C>()) {
			return container.cbegin();
		}
		else if constexpr (qpl::is_write_container<C>()) {
			return container.begin();
		}
	}
	template<typename C> requires(qpl::is_container<C>())
		constexpr auto cbegin(C& container) {
		if constexpr (qpl::is_read_container<C>() && qpl::is_write_container<C>()) {
			return container.cbegin();
		}
		else if constexpr (qpl::is_read_container<C>()) {
			return container.cbegin();
		}
		else if constexpr (qpl::is_write_container<C>()) {
			return container.begin();
		}
	}

	template<typename C> requires(qpl::is_container<C>())
		constexpr auto end(C& container) {
		if constexpr (qpl::is_read_container<C>() && qpl::is_write_container<C>()) {
			return container.end();
		}
		else if constexpr (qpl::is_read_container<C>()) {
			return container.cend();
		}
		else if constexpr (qpl::is_write_container<C>()) {
			return container.end();
		}
	}
	template<typename C> requires(qpl::is_container<C>())
		constexpr auto cend(C& container) {
		if constexpr (qpl::is_read_container<C>() && qpl::is_write_container<C>()) {
			return container.cend();
		}
		else if constexpr (qpl::is_read_container<C>()) {
			return container.cend();
		}
		else if constexpr (qpl::is_write_container<C>()) {
			return container.end();
		}
	}

	
	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
		qpl::size container_size(const C& data) {
		if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			qpl::size sum = qpl::size{};
			for (const auto& e : data) {
				sum += container_size(e);
			}
			return sum;
		}
		else {
			return data.size();
		}
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr qpl::size container_depth() {
		return qpl::is_container<C> ?
			(qpl::is_container<qpl::container_subtype<C>>() ? qpl::container_depth<qpl::container_subtype<C>>() + qpl::size{ 1 } : qpl::size{ 1 }) :
			qpl::size{};
	}



	template<typename C> requires (qpl::is_container<C>())
	constexpr auto container_sum(const C& data) {
		auto sum = data[0];
		for (qpl::u32 i = 1u; i < data.size(); ++i) {
			sum += data[i];
		}
		return sum;
	}


	template<typename T, qpl::size N>
	std::vector<T> array_to_vector(const std::array<T, N>& source) {
		std::vector<T> result(source.size());
		memcpy(result.data(), source.data(), source.size() * sizeof(T));
		return result;
	}
	template<typename T>
	void remove_vector_element(std::vector<T>& source, qpl::u32 index) {
		std::vector<T> result;
		result.reserve(source.size() - 1);
		for (qpl::u32 i = 0u; i < source.size(); ++i) {
			if (i != index) {
				result.push_back(source[i]);
			}
		}
		source = result;
	}
	template< qpl::size N, typename T>
	std::array<T, N> vector_to_array(const std::vector<T>& source) {
		std::array<T, N> result;
		memcpy(result.data(), source.data(), source.size() * sizeof(T));
		return result;
	}
	template<typename... Args>
	auto tuple_to_vector(std::tuple<Args...> tuple) -> std::vector<std::tuple_element_t<0, decltype(tuple)>> {
		std::vector<std::tuple_element_t<0, decltype(tuple)>> result(std::tuple_size_v<decltype(tuple)>);
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = qpl::type_cast<std::tuple_element_t<0, decltype(tuple)>>(std::get<Ints>(tuple))), ...);
		};
		unpack(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)>>());
		return result;
	}
	template<typename T, typename... Args>
	std::vector<T> tuple_to_vector(std::tuple<Args...> tuple) {
		std::vector<T> result(std::tuple_size_v<decltype(tuple)>);
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = qpl::type_cast<T>(std::get<Ints>(tuple))), ...);
		};
		unpack(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)>>());
		return result;
	}
	template<typename T, typename... Args>
	std::vector<T> to_vector(Args... args) {
		return qpl::tuple_to_vector<T>(std::make_tuple(args...));
	}
	template<typename... Args>
	auto to_vector(Args... args) {
		return qpl::tuple_to_vector(std::make_tuple(args...));
	}

	template<typename T> requires (qpl::is_tuple<T>())
	constexpr auto tuple_to_array(T tuple) {
		std::array<qpl::tuple_type<0, T>, qpl::tuple_size<T>()> result{};
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = qpl::type_cast<qpl::tuple_type<0, T>>(std::get<Ints>(tuple))), ...);
		};
		unpack(tuple, std::make_index_sequence<qpl::tuple_size<T>()>());
		return result;
	}
	template<typename R, typename T> requires (qpl::is_tuple<T>())
	constexpr auto tuple_to_array(T tuple) {
		std::array<R, qpl::tuple_size<T>()> result{};
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = qpl::type_cast<R>(std::get<Ints>(tuple))), ...);
		};
		unpack(tuple, std::make_index_sequence<qpl::tuple_size<T>()>());
		return result;
	}

	template<typename... Ts>
	constexpr auto variadic_to_array(Ts&&... values) {
		return qpl::tuple_to_array(std::make_tuple(values...));
	}
	template<typename R, typename... Ts>
	constexpr auto variadic_to_array(Ts&&... values) {
		return qpl::tuple_to_array<R>(std::make_tuple(values...));
	}
	template<typename... Ts>
	constexpr auto tuple_to_array(Ts&&... values) {
		return qpl::tuple_to_array(std::make_tuple(values...));
	}
	template<typename R, typename... Ts>
	constexpr auto tuple_to_array(Ts&&... values) {
		return qpl::tuple_to_array<R>(std::make_tuple(values...));
	}
	namespace impl {
		template<typename T, qpl::size>
		using ignore_size = T;
	}

	template<typename T, qpl::size N>
	constexpr auto array_to_tuple(const std::array<T, N>& array) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			std::tuple<impl::ignore_size<T, Ints>...> result;
			((std::get<Ints>(result) = array[Ints]), ...);
			return result;
		};
		return unpack(std::make_index_sequence<N>());
	}
	template<typename R, typename T, qpl::size N>
	constexpr auto array_to_tuple(const std::array<T, N>& array) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			std::tuple<impl::ignore_size<R, Ints>...> result;
			((std::get<Ints>(result) = qpl::type_cast<R>(array[Ints])), ...);
			return result;
		};
		return unpack(std::make_index_sequence<N>());
	}

	template<typename T, typename... Args>
	constexpr auto to_array(Args&&... args) {
		return qpl::tuple_to_array<T>(std::make_tuple(args...));
	}
	template<typename... Args>
	constexpr auto to_array(Args&&... args) {
		return qpl::tuple_to_array(std::make_tuple(args...));
	}

	template<typename... Args>
	constexpr auto to_tuple(Args&&... args) {
		return std::make_tuple(args...);
	}

	namespace impl {
		template<typename Tuple, qpl::size N>
		struct tuple_loop {
			constexpr tuple_loop(Tuple& tuple) : tuple(tuple) {

			}

			constexpr static qpl::size i = N;
			constexpr static qpl::size size = qpl::tuple_size<Tuple>();

			template<qpl::i64 I>
			constexpr const auto& at() const {
				if constexpr (I > 0 && I < size) {
					return qpl::tuple_value<I>(this->tuple);
				}
				else {
					return decltype(this->operator*()){};
				}
			}
			template<qpl::i64 I>
			constexpr auto& at() {
				return qpl::tuple_value<I>(this->tuple);
			}

			constexpr auto& operator*() {
				return qpl::tuple_value<N>(this->tuple);
			}
			constexpr const auto& operator*() const {
				return qpl::tuple_value<N>(this->tuple);
			}

			constexpr auto* operator->() {
				return &qpl::tuple_value<N>(this->tuple);
			}
			constexpr const auto* operator->() const {
				return &qpl::tuple_value<N>(this->tuple);
			}

		private:
			Tuple& tuple;
		};

		template<qpl::size N>
		struct constexpr_loop {
			constexpr static qpl::size i = N;

			constexpr static qpl::size index() {
				return N;
			}
		};

		template<qpl::size compare, qpl::i64 Start, qpl::i64 End, qpl::i64 Inc>
		constexpr bool tuple_range_valid() {
			return Inc > 0 && Start >= 0 && End <= 0 && (qpl::signed_cast(compare) - Start + End) / Inc > 0;
		}
	}
	
	template<typename T, typename F> requires (qpl::is_tuple<T>())
	constexpr void tuple_iterate(T tuple, F function) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(std::get<Ints>(tuple)), ...);
		};
		unpack(std::make_index_sequence<qpl::tuple_size<T>()>());
	}
	template<qpl::i64 Start, qpl::i64 End, qpl::i64 Inc, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, Inc>())
	constexpr void tuple_iterate(T tuple, F function) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(std::get<Ints * Inc + Start>(tuple)), ...);
		};
		constexpr auto size = qpl::unsigned_cast((qpl::signed_cast(qpl::tuple_size<T>()) - Start + End) / Inc);
		unpack(std::make_index_sequence<size>());
	}
	template<qpl::i64 Start, qpl::i64 End, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, 1>())
	constexpr void tuple_iterate(T tuple, F function) {
		qpl::tuple_iterate<Start, End, 1>(tuple, function);
	}
	template<qpl::i64 Start, typename T, typename F>requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, 0, 1>())
	constexpr void tuple_iterate(T tuple, F function) {
		qpl::tuple_iterate<Start, 0, 1>(tuple, function);
	}

	template<typename T, typename F> requires (qpl::is_tuple<T>())
	constexpr void tuple_iterate_indexed(T tuple, F function) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(std::get<Ints>(tuple), Ints), ...);
		};
		unpack(std::make_index_sequence<qpl::tuple_size<T>()>());
	}
	template<qpl::i64 Start, qpl::i64 End, qpl::i64 Inc, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, Inc>())
	constexpr void tuple_iterate_indexed(T tuple, F function) {
		constexpr auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(std::get<Ints * Inc + Start>(tuple), Ints * Inc + Start), ...);
		};
		constexpr auto size = qpl::unsigned_cast((qpl::signed_cast(qpl::tuple_size<T>()) - Start + End) / Inc);
		unpack(std::make_index_sequence<size>());
	}
	template<qpl::i64 Start, qpl::i64 End, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, 1>())
	constexpr void tuple_iterate_indexed(T tuple, F function) {
		qpl::tuple_iterate_indexed<Start, End, 1>(tuple, function);
	}
	template<qpl::i64 Start, typename T, typename F>requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, 0, 1>())
	constexpr void tuple_iterate_indexed(T tuple, F function) {
		qpl::tuple_iterate_indexed<Start, 0, 1>(tuple, function);
	}

	template<typename T, typename F> requires (qpl::is_tuple<T>())
	constexpr void tuple_iterate_control(T tuple, F function) {
		constexpr auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(qpl::impl::tuple_loop<T, Ints>(tuple)), ...);
		};
		unpack(std::make_index_sequence<qpl::tuple_size<T>()>());
	}

	template<qpl::i64 Start, qpl::i64 End, qpl::i64 Inc, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, Inc>())
	constexpr void tuple_iterate_control(T tuple, F function) {
		constexpr auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(qpl::impl::tuple_loop<T, Ints * Inc + Start>(tuple)), ...);
		};
		constexpr auto size = qpl::unsigned_cast((qpl::signed_cast(qpl::tuple_size<T>()) - Start + End) / Inc);
		unpack(std::make_index_sequence<size>());
	}
	template<qpl::i64 Start, qpl::i64 End, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, End, 1>())
	constexpr void tuple_iterate_control(T tuple, F function) {
		qpl::tuple_iterate_control<Start, End, 1>(tuple, function);
	}
	template<qpl::i64 Start, typename T, typename F> requires (qpl::is_tuple<T>() && qpl::impl::tuple_range_valid<qpl::tuple_size<T>(), Start, 0, 1>())
	constexpr void tuple_iterate_control(T tuple, F function) {
		qpl::tuple_iterate_control<Start, 0, 1>(tuple, function);
	}

	template<qpl::size N, typename F>
	constexpr void constexpr_iterate(F function) {
		constexpr auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(function(qpl::impl::constexpr_loop<Ints>()), ...);
		};
		unpack(std::make_index_sequence<N>());
	}

	template<typename C>
	constexpr auto make_span(const C& container) {
		return std::span(qpl::begin(container), qpl::end(container));
	}
	template<typename C> requires (qpl::is_span<C>())
	constexpr void span_pop_front(C& span, qpl::size size = 1) {
		auto begin = qpl::begin(span);
		std::advance(begin, size);
		span = std::span(begin, qpl::end(span));
	}
	template<typename C> requires (qpl::is_span<C>())
	constexpr void span_pop_back(C& span, qpl::size size = 1) {
		auto end = qpl::end(span);
		std::advance(end, -qpl::signed_cast(size));
		span = std::span(qpl::begin(span), end);
	}

	template<typename T, typename T1 = T>
	constexpr std::vector<T> vector_0_to_n(T n, T1 shift = T{}) {
		std::vector<T> vec(n);
		std::iota(vec.begin(), vec.end(), shift);
		return vec;
	}

	template<typename C, typename F> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr void sort(C& container, F compare) {
		if constexpr (qpl::is_sorted_container<C>()) {
			return;
		}
		else if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			if constexpr (qpl::is_long_string_type<qpl::container_subtype<C>>()) {
				std::sort(qpl::begin(container), qpl::end(container));
			}
			else {
				for (auto& i : container) {
					qpl::sort(i, compare);
				}
			}
		}
		else {
			std::sort(qpl::begin(container), qpl::end(container), compare);
		}
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr void sort_less(C& container) {
		qpl::sort(container, [](const auto& a, const auto& b) { return a < b; });
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr void sort_greater(C& container) {
		qpl::sort(container, [](const auto& a, const auto& b) { return a > b; });
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr void sort(C& container) {
		qpl::sort_less(container);
	}

	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr C sorted_less(const C& container) {
		auto result = container;
		qpl::sort_less(result);
		return result;
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr C sorted_greater(const C& container) {
		auto result = container;
		qpl::sort_greater(result);
		return result;
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr C sorted(const C& container) {
		auto result = container;
		qpl::sort(result);
		return result;
	}
	template<typename C, typename F> requires (qpl::is_container<C>() && qpl::is_sortable<C>())
	constexpr C sorted(const C& container, F compare) {
		auto result = container;
		qpl::sort(result, compare);
		return result;
	}


	template<typename C, typename F> requires (qpl::is_container<C>())
	constexpr bool is_sorted(const C& container, F compare) {
		if constexpr (qpl::is_sorted_container<C>()) {
			auto begin = qpl::cbegin(container);
			auto end = qpl::cend(container);
			if (begin == end) { return true; }
			--end;
			if (begin == end) { return true; }
			--end;
			while (begin != end) {
				auto c2 = begin;
				++c2;
				if constexpr (qpl::has_equal<qpl::container_subtype<C>>()) {
					if (!(*c2 == *begin)) {
						return !compare(*c2, *begin);
					}
				}
				else {
					auto equal = !compare(*c2, *begin) && !compare(*begin, *c2);
					if (!equal) {
						return !compare(*c2, *begin);
					}
				}
				++begin;
			}
			return true;
		}
		else {
			auto begin = qpl::cbegin(container);
			auto end = qpl::cend(container);
			if (begin == end) { return true; }
			--end;
			if (begin == end) { return true; }
			--end;
			while (begin != end) {
				auto c2 = begin;
				++c2;
				if (compare(*c2, *begin)) {
					return false;
				}
				++begin;
			}
			return true;
		}
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_sorted_less(const C& container) {
		return qpl::is_sorted(container, [](const auto& a, const auto& b) { return a < b; });
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_sorted_greater(const C& container) {
		return qpl::is_sorted(container, [](const auto& a, const auto& b) { return a > b; });
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_sorted(const C& container) {
		return qpl::is_sorted_less(container);
	}



	template<typename C> requires (qpl::is_container<C>())
	constexpr void reverse(C& container, bool recursive = false) {
		if (recursive) {
			if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
				for (auto& i : container) {
					qpl::reverse(i);
				}
				return;
			}
		}
		std::reverse(qpl::begin(container), qpl::end(container));
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr C reversed(const C& container, bool recursive = false) {
		auto result = container;
		qpl::reverse(result, recursive);
		return result;
	}

	namespace impl {
		template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
		constexpr void combine(C& container, T&& value) {
			if constexpr (qpl::has_resize_and_access<C>()) {
				container.resize(container.size() + 1);
				if constexpr (qpl::has_square_brackets<C>()) {
					container[container.size() - 1] = qpl::type_cast<qpl::container_subtype<C>>(value);
				}
				else if constexpr (qpl::has_at<C>()) {
					container.at(container.size() - 1) = qpl::type_cast<qpl::container_subtype<C>>(value);
				}
			}
			else if constexpr (qpl::has_push_back<C>()) {
				container.push_back(qpl::type_cast<qpl::container_subtype<C>>(value));
			}
			else if constexpr (qpl::has_insert<C>()) {
				container.insert(qpl::type_cast<qpl::container_subtype<C>>(value));
			}
			else {
				static_assert("qpl::combine: C has no way to add elements");
			}
		}
		
		template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
		constexpr void combine(C& container, const C2& value) {
			if constexpr (qpl::has_insert<C>()) {
				container.insert(qpl::cend(container), value.cbegin(), value.cend());
			}
			else if constexpr (qpl::has_resize_and_access<C>()) {
				auto size = container.size();
				container.resize(container.size() + value.size());
				if constexpr (qpl::is_same_decayed<qpl::container_subtype<C>, qpl::container_subtype<C2>>() && qpl::is_contiguous_container<C>()) {
					memcpy(container.data() + size, value.data(), value.size() * qpl::bytes_in_type<qpl::container_subtype<C2>>());
				}
				else if constexpr (qpl::has_square_brackets<C>()) {
					for (qpl::size i = size; i < container.size(); ++i) {
						container[i] = qpl::type_cast<qpl::container_subtype<C>>(value[i - size]);
					}
				}
				else if constexpr (qpl::has_at<C>()) {
					for (qpl::size i = size; i < container.size(); ++i) {
						container.at(i) = qpl::type_cast<qpl::container_subtype<C>>(value.at(i - size));
					}
				}
			}
			else if constexpr (qpl::has_push_back<C>()) {
				if constexpr (qpl::has_reserve<C>()) {
					container.reserve(container.size() + value.size());
				}
				for (const auto& i : value) {
					container.push_back(qpl::type_cast<qpl::container_subtype<C>>(i));
				}
			}
			else {
				static_assert("qpl::combine: C has no way to add elements");
			}
		}
	
		template<typename C, typename It> requires (qpl::is_container<C>() && qpl::is_iterator<It>())
		constexpr void combine(C& container, It start, It end) {
			if constexpr (qpl::has_insert<C>()) {
				container.insert(qpl::cend(container), start, end);
			}
			else if constexpr (qpl::has_resize_and_access<C>()) {
				auto size = container.size();
				auto it = start;
				container.resize(container.size() + std::distance(start, end));
				if constexpr (qpl::has_square_brackets<C>()) {
					for (qpl::size i = size; i < container.size(); ++i) {
						container[i] = qpl::type_cast<qpl::container_subtype<C>>(*it);
						++it;
					}
				}
				else if constexpr (qpl::has_at<C>()) {
					for (qpl::size i = size; i < container.size(); ++i) {
						container.at(i) = qpl::type_cast<qpl::container_subtype<C>>(*it);
						++it;
					}
				}
			}
			else if constexpr (qpl::has_push_back<C>()) {
				if constexpr (qpl::has_reserve<C>()) {
					container.reserve(container.size() + std::distance(start, end));
				}
				auto it = start;
				while (!(it == end)) {
					container.push_back(qpl::type_cast<qpl::container_subtype<C>>(*it));
					++it;
				}
			}
			else {
				static_assert("qpl::combine: C has no way to add elements");
			}
		}
	
		template<typename C> requires (qpl::is_container<C>())
		constexpr void remove_duplicates(C& container) {
			if constexpr (qpl::has_less<qpl::container_subtype<C>>()) {
				std::set<qpl::container_subtype<C>> seen;
				C result;
				for (auto& i : container) {
					if (seen.find(i) == seen.cend()) {
						seen.insert(i);
						qpl::impl::combine(result, i);
					}
				}
				std::swap(container, result);
			}
			else {
				std::unordered_set<qpl::container_subtype<C>> seen;
				C result;
				for (auto& i : container) {
					if (seen.find(i) == seen.cend()) {
						seen.insert(i);
						qpl::impl::combine(result, i);
					}
				}
				std::swap(container, result);
			}
		}
	}

	template<typename C, typename... T> requires (qpl::is_container<C>())
	constexpr void combine(C& container, const T... values) {
		(impl::combine(container, values), ...);
	}
	template<typename C,typename It> requires (qpl::is_container<C>() && qpl::is_iterator<It>())
	constexpr void combine(C& container, It begin, It end) {
		if (begin == end) {
			return;
		}
		impl::combine(container, begin, end);
	}

	template<typename C, typename... T> requires (qpl::is_container<C>())
	constexpr C combined(const C& container, const T... values) {
		C result;
		impl::combine(result, container);
		(impl::combine(result, values), ...);
		return result;
	}
	template<typename C, typename It> requires (qpl::is_container<C>() && qpl::is_iterator<It>())
	constexpr C combined(const C& container, It begin, It end) {
		C result;
		impl::combine(result, begin, end);
		return result;
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr void remove_duplicates(C& container, bool recursive = false) {
		if (recursive) {
			if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
				for (auto& i : container) {
					qpl::remove_duplicates(i, recursive);
				}
				return;
			}
		}
		qpl::impl::remove_duplicates(container);
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr C removed_duplicates(const C& container, bool recursive = false) {
		C result;
		qpl::remove_duplicates(result, recursive);
		return result;
	}

	template<typename C> requires (qpl::is_container<C>())
	constexpr void splice(C& container, qpl::size start, qpl::size end = qpl::size_max) {
		C result;
		if constexpr (qpl::has_insert<C>()) {
			auto it1 = qpl::cbegin(container);
			std::advance(it1, start);
			auto it2 = qpl::cbegin(container);
			std::advance(it2, qpl::min(container.size() - 1, end));
			result.insert(result.cend(), it1, it2);
		}
		else if constexpr (qpl::has_resize_and_access<C>()) {
			result.resize((qpl::min(container.size() - 1, end) - start) + 1);
			if constexpr (qpl::has_square_brackets<C>()) {
				for (qpl::size i = start; i <= end && i < container.size(); ++i) {
					result[i - start] = container[i];
				}
			}
			else if constexpr (qpl::has_at<C>()) {
				for (qpl::size i = start; i <= end && i < container.size(); ++i) {
					result.at(i - start) = container.at(i);
				}
			}
		}
		else if constexpr (qpl::has_push_back<C>()) {
			if constexpr (qpl::has_reserve<C>()) {
				result.reserve((qpl::min(container.size() - 1, end) - start) + 1);
			}
			auto it_begin = qpl::cbegin(container);
			auto it_end = qpl::cbegin(container);
			std::advance(it_begin, start);
			std::advance(it_end, end);
			while (it_begin != qpl::cend(container) && it_begin != it_end) {
				result.push_back(*it_begin);
				++it_begin;
			}
		}
		std::swap(container, result);
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr C spliced(const C& container, qpl::size start, qpl::size end = qpl::size_max) {
		auto result = container;
		qpl::splice(result, start, end);
		return result;
	}
	
	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	constexpr void splice_back(C& container, qpl::size count) {
		qpl::splice(container, container.size() - count, container.size() - 1);
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	constexpr C spliced_back(const C& container, qpl::size count) {
		return qpl::spliced(container, container.size() - count, container.size() - 1);
	}

	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	constexpr void splice_front(C& container, qpl::size count) {
		qpl::splice(container, 0, count - 1);
	}
	template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
	constexpr C spliced_front(const C& container, qpl::size count) {
		return qpl::spliced(container, 0, count - 1);
	}

	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr bool includes(const C& target, const C2& list) {
		for (auto& i : target) {
			bool found = false;
			for (auto& j : list) {
				if (i == j) {
					found = true;
					break;
				}
			}
			if (!found) {
				return false;
			}
		}
		return true;
	}
	
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr bool find_sorted(const C& container, T&& value) {
		return std::binary_search(qpl::cbegin(container), qpl::cend(container), value);
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr bool find(const C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>()) {
			return qpl::find_sorted(container, value);
		}
		else if constexpr (qpl::is_std_unordered_map_type<C>()) {
			return container.find(value) != container.cend();
		}
		else {
			for (auto& i : container) {
				if (i == value) {
					return true;
				}
			}
			return false;
		}
	}
	

	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr auto lower_bound(const C& container, T&& value) {
		return std::lower_bound(qpl::cbegin(container), qpl::cend(container), value);
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr auto upper_bound(const C& container, T&& value) {
		return std::upper_bound(qpl::cbegin(container), qpl::cend(container), value);
	}

	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr qpl::size find_lower_index_sorted(const C& container, T&& value) {
		auto it = qpl::lower_bound(container, value);
		if (*it == value) {
			return std::distance(qpl::cbegin(container), it);
		}
		else {
			return qpl::size_max;
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr qpl::size find_lower_index(const C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>()) {
			return qpl::find_lower_index_sorted(container, value);
		}
		else {
			qpl::size index = 0u;
			for (auto& i : container) {
				if (i == value) {
					return index;
				}
				++index;
			}
			return qpl::size_max;
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr qpl::size find_upper_index_sorted(const C& container, T&& value) {
		auto it = qpl::upper_bound(container, value);
		--it;
		if (*it == value) {
			return std::distance(qpl::cbegin(container), it);
		}
		else {
			return qpl::size_max;
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr qpl::size find_upper_index(const C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>()) {
			return qpl::find_upper_index_sorted(container, value);
		}
		else {
			bool found = false;
			qpl::size index = 0u;
			for (auto& i : container) {
				if (!found && i == value) {
					found = true;
				}
				else if (found && i != value) {
					return index;
				}
				++index;
			}
			return qpl::size_max;
		}
	}
	

	template<typename C, typename T> requires (qpl::is_container<C>() && qpl::is_equal_comparable<qpl::container_subtype<C>, T>())
	constexpr qpl::size count_sorted(const C& container, T&& value) {
		auto lower_index = qpl::find_lower_index_sorted(container, value);
		auto upper_index = qpl::find_upper_index_sorted(container, value);
		if (lower_index == qpl::size_max) {
			return 0u;
		}
		return (upper_index - lower_index) + 1;
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && qpl::is_equal_comparable<qpl::container_subtype<C>, T>())
	constexpr qpl::size count(const C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>()) {
			return qpl::count_sorted(container, value);
		}
		else {
			qpl::size sum = 0u;
			for (auto& i : container) {
				if (i == value) {
					++sum;
				}
			}
			return sum;
		}
	}

	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr void remove(C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>() && qpl::has_erase<C>()) {
			auto index = qpl::find_lower_index_sorted(container, value);
			if (index != qpl::size_max) {
				auto begin = qpl::cbegin(container);
				std::advance(begin, index);
				container.erase(begin);
			}
		}
		else {
			C result;
	
			if constexpr (qpl::has_reserve<C>()) {
				result.reserve(container.size());
			}
			bool allow = true;
			qpl::size index = 0u;
			for (auto& i : container) {
				if (!(i == value)) {
					qpl::combine(result, i);
				}
				else {
					break;
				}
				++index;
			}
	
	
			auto begin = qpl::cbegin(container);
			std::advance(begin, index + 1);
			qpl::combine(result, begin, qpl::cend(container));
			std::swap(result, container);
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr void remove_sorted(C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>() && qpl::has_erase<C>()) {
			auto index = qpl::find_lower_index_sorted(container, value);
			if (index != qpl::size_max) {
				auto begin = qpl::cbegin(container);
				std::advance(begin, index);
				container.erase(begin);
			}
		}
		else {
			C result;
			auto index = qpl::find_lower_index_sorted(container, value);
			if (index == qpl::size_max) {
				return;
			}
			if (index) {
				auto end = qpl::cbegin(container);
				std::advance(end, index);
				qpl::combine(result, qpl::cbegin(container), end);
			}
			auto begin = qpl::cbegin(container);
			std::advance(begin, index + 1);
			if (begin != qpl::cend(container)) {
				qpl::combine(result, begin, qpl::cend(container));
			}
			std::swap(result, container);
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr void remove_multiples(C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>() && qpl::has_erase<C>()) {
			container.erase(value);
		}
		else {
			C result;
	
			if constexpr (qpl::has_reserve<C>()) {
				result.reserve(container.size());
			}
			for (auto& i : container) {
				if (!(i == value)) {
					qpl::combine(result, i);
				}
			}
	
			std::swap(result, container);
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr void remove_sorted_multiples(C& container, T&& value) {
		if constexpr (qpl::is_sorted_container<C>() && qpl::has_erase<C>()) {
			container.erase(value);
		}
		else {
			C result;
			auto lower_index = qpl::find_lower_index_sorted(container, value);
			auto upper_index = qpl::find_upper_index_sorted(container, value);
	
			if (lower_index == qpl::size_max) {
				return;
			}
			if (lower_index) {
				auto end = qpl::cbegin(container);
				std::advance(end, lower_index);
				qpl::combine(result, qpl::cbegin(container), end);
			}
			auto begin = qpl::cbegin(container);
			std::advance(begin, upper_index + 1);
			if (begin != qpl::cend(container)) {
				qpl::combine(result, begin, qpl::cend(container));
			}
			std::swap(result, container);
		}
	}
	
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr C removed_multiples(const C& container, T&& value) {
		auto result = container;
		qpl::remove_duplicates(result, value);
		return result;
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr C removed(const C& container, T&& value) {
		auto result = container;
		qpl::remove(result, value);
		return result;
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr C removed_sorted_multiples(const C& container, T&& value) {
		auto result = container;
		qpl::remove_sorted_multiples(result, value);
		return result;
	}
	template<typename C, typename T> requires (qpl::is_container<C>() && !qpl::is_container<T>())
	constexpr C removed_sorted(const C& container, T&& value) {
		auto result = container;
		qpl::remove_sorted(result, value);
		return result;
	}

	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr C duplicates(const C& a, const C2& b) {
		if constexpr (qpl::is_sortable<C2>()) {
			auto sorted = qpl::sorted(b);
			auto list = qpl::make_span(sorted);
			C result;
			for (auto& i : a) {
				if (qpl::find_sorted(list, i)) {
					qpl::combine(result, i);
					qpl::span_pop_front(list, 1);
				}
			}
			return result;
		}
		else {
			auto list = b;
			C result;
			for (auto& i : a) {
				if (qpl::find(list, i)) {
					qpl::combine(result, i);
					qpl::remove(list, i);
				}
			}
			return result;
		}
	}

	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr void remove_sorted(C& target, const C2& list) {
		auto sorted = qpl::sorted(list);
		auto l = qpl::make_span(sorted);

		C result;
		qpl::size count = 0u;
		for (auto& i : target) {
			if (qpl::find_sorted(l, i)) {
				qpl::span_pop_front(l, 1);
			}
			else {
				qpl::combine(result, i);
			}
		}
		std::swap(target, result);
	}
	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr void remove(C& target, const C2& list) {
		if constexpr (qpl::is_sortable<C2>() && qpl::is_sortable<C>()) {
			auto sorted = qpl::sorted(list);
			auto l = qpl::make_span(sorted);
			auto t = qpl::sorted(target);

			C result;
			qpl::size count = 0u;
			for (auto& i : t) {
				if (qpl::find_sorted(l, i)) {
					qpl::span_pop_front(l, 1);
				}
				else {
					qpl::combine(result, i);
				}
			}
			std::swap(target, result);
		}
		else {
			C result = target;
			for (auto& i : list) {
				qpl::remove(result, i);
			}
			std::swap(target, result);
		}
	}

	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr void remove_sorted_multiples(C& target, const C2& list) {
		C result;
		qpl::size count = 0u;
		for (auto& i : target) {
			if (!qpl::find_sorted(list, i)) {
				qpl::combine(result, i);
			}
		}
		std::swap(target, result);
	}
	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr void remove_multiples(C& target, const C2& list) {
		C result;
		qpl::size count = 0u;
		for (auto& i : target) {
			if (!qpl::find(list, i)) {
				qpl::combine(result, i);
			}
		}
		std::swap(target, result);
	}
	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr C removed(const C& a, const C2& b) {
		auto result = a;
		qpl::remove(result, b);
		return result;
	}

	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr C removed_multiples(const C& a, const C2& b) {
		auto result = a;
		qpl::remove_multiples(result, b);
		return result;
	}
	template<typename C, typename C2> requires (qpl::is_container<C>() && qpl::is_container<C2>())
	constexpr C removed_sorted_multiples(const C& a, const C2& b) {
		auto result = a;
		qpl::remove_sorted_multiples(result, b);
		return result;
	}

	template<typename R, typename C> requires (qpl::is_container<C>())
	constexpr R sum(const C& container) {
		if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			R sum = R{ 0 };
			for (auto& i : container) {
				sum += qpl::sum<R>(i);
			}
			return sum;
		}
		else {
			R sum = R{ 0 };
			for (auto& i : container) {
				sum += qpl::type_cast<R>(i);
			}
			return sum;
		}
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr auto sum(const C& container) {
		if constexpr (qpl::is_floating_point<qpl::container_deepest_subtype<C>>()) {
			return qpl::sum<qpl::f64>(container);
		}
		else if constexpr (qpl::is_signed<qpl::container_deepest_subtype<C>>()) {
			return qpl::sum<qpl::isize>(container);
		}
		else {
			return qpl::sum<qpl::size>(container);
		}
	}

	template<typename R, typename C> requires (qpl::is_container<C>())
	constexpr R product(const C& container) {
		if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			R n = R{ 1 };
			for (auto& i : container) {
				n *= qpl::product<R>(i);
			}
			return n;
		}
		else {
			R n = R{ 1 };
			for (auto& i : container) {
				n *= qpl::type_cast<R>(i);
			}
			return n;
		}
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr auto product(const C& container) {
		if constexpr (qpl::is_floating_point<qpl::container_deepest_subtype<C>>()) {
			return qpl::product<qpl::f64>(container);
		}
		else if constexpr (qpl::is_signed<qpl::container_deepest_subtype<C>>()) {
			return qpl::product<qpl::isize>(container);
		}
		else {
			return qpl::product<qpl::size>(container);
		}
	}

	template<typename C> requires (qpl::is_container<C>())
	constexpr void fill_zeroes(C& container) {
		if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			for (auto& i : container) {
				qpl::fill_zeroes(i);
			}
		}
		else {
			if constexpr (qpl::is_contiguous_container<C>() && qpl::has_size<C>()) {
				memset(container.data(), 0, container.size() * qpl::bytes_in_type<qpl::container_subtype<C>>());
			}
			else {
				std::fill(qpl::begin(container), qpl::end(container), qpl::container_subtype<C>{0});
			}
		}
	}
	template<typename C> requires (qpl::is_container<C>())
	constexpr C fill_zeroes(const C& container) {
		auto result = container;
		qpl::fill_zeroes(result);
		return result;
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr void fill(C& container, T&& value) {
		if (!value) {
			qpl::fill_zeroes(container);
		}
		if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
			for (auto& i : container) {
				qpl::fill(i, value);
			}
		}
		else {
			std::fill(qpl::begin(container), qpl::end(container), qpl::type_cast<qpl::container_subtype<C>>(value));
		}
	}
	template<typename C, typename T> requires (qpl::is_container<C>())
	constexpr C filled(const C& container, T&& value) {
		auto result = container;
		qpl::fill(result, value);
		return result;
	}
	
	namespace impl {
		template<typename C, typename T> requires(qpl::is_container<C>() && qpl::has_resize<C>() && qpl::tuple_size<T>() > 0)
		void resize(C& container, T tuple) {
			container.resize(qpl::tuple_value_front(tuple));
			if constexpr (qpl::is_container<qpl::container_subtype<C>>() && qpl::has_resize<C>() && qpl::tuple_size<T>() > 1) {
				for (auto& i : container) {
					qpl::impl::resize(i, qpl::tuple_splice_front<1>(tuple));
				}
			}
		}
		template<typename C, typename T> requires(qpl::is_container<C>() && qpl::has_reserve<C>() && qpl::tuple_size<T>() > 0)
		void reserve(C& container, T tuple) {
			container.reserve(qpl::tuple_value_front(tuple));
			if constexpr (qpl::is_container<qpl::container_subtype<C>>() && qpl::has_reserve<C>() && qpl::tuple_size<T>() > 1) {
				for (auto& i : container) {
					qpl::impl::reserve(i, qpl::tuple_splice_front<1>(tuple));
				}
			}
		}
	}

	template<typename C, typename... Sizes> requires(qpl::is_container<C>() && qpl::has_resize<C>() && qpl::variadic_size<Sizes...>() > 0)
	void resize(C& container, Sizes... sizes) {
		qpl::impl::resize(container, std::make_tuple(sizes...));
	}
	template<typename C, typename... Sizes> requires(qpl::is_container<C>() && qpl::has_reserve<C>() && qpl::variadic_size<Sizes...>() > 0)
	void reserve(C& container, Sizes... sizes) {
		qpl::impl::reserve(container, std::make_tuple(sizes...));
	}



	namespace impl {
		template<typename T, qpl::size N>
		struct possibilities_iterator {
			std::array<T, N> values;
			std::array<T, N> max;


			constexpr possibilities_iterator(const std::array<T, N>& max) : values(), max() {
				this->values.fill(T{});
				this->max = max;
			}
			constexpr possibilities_iterator() : values(), max() {
				this->values.fill(T{});
				this->max.fill(T{});
			}
			constexpr bool operator==(const possibilities_iterator& other) {
				return this->values == other.values;
			}
			constexpr bool operator!=(const possibilities_iterator& other) {
				return this->values != other.values;
			}
			constexpr void set_end() {
				for (auto& i : this->values) {
					i = qpl::type_max<T>();
				}
			}
			constexpr possibilities_iterator& operator++(int) {
				bool ended = false;
				for (qpl::u32 i = 0u; i < this->values.size(); ++i) {
					++this->values[i];
					if (this->values[i] == this->max[i]) {
						this->values[i] = 0u;
					}
					else {
						ended = true;
						break;
					}
				}
				if (!ended) {
					this->set_end();
				}
				return *this;
			}
			constexpr possibilities_iterator operator++() {
				auto copy = *this;
				this->operator++(0);
				return copy;
			}
			constexpr auto operator*() const {
				return qpl::array_to_tuple(this->values);
			}
		};


		template<typename T, qpl::size N>
		struct possibilities_reverse_iterator {
			std::array<T, N> values;
			std::array<T, N> max;


			constexpr possibilities_reverse_iterator(const std::array<T, N>& max) : values(), max() {
				this->values = max;
				for (auto& i : this->values) {
					--i;
				}
				this->max = max;
			}
			constexpr possibilities_reverse_iterator() : values(), max() {
				this->values.fill(T{});
				this->max.fill(T{});
			}
			constexpr bool operator==(const possibilities_reverse_iterator& other) {
				return this->values == other.values;
			}
			constexpr bool operator!=(const possibilities_reverse_iterator& other) {
				return this->values != other.values;
			}
			constexpr void set_end() {
				for (auto& i : this->values) {
					i = qpl::type_max<T>();
				}
			}
			constexpr possibilities_reverse_iterator& operator++(int) {
				bool ended = false;
				for (qpl::u32 i = 0u; i < this->values.size(); ++i) {
					auto index = this->values.size() - i - 1;
					if (this->values[index] == T{}) {
						this->values[index] = this->max[index] - 1;
					}
					else {
						--this->values[index];
						ended = true;
						break;
					}
				}
				if (!ended) {
					this->set_end();
				}
				return *this;
			}
			constexpr possibilities_reverse_iterator operator++() {
				auto copy = *this;
				this->operator++(0);
				return copy;
			}
			constexpr auto operator*() const {
				return qpl::array_to_tuple(this->values);
			}
		};

		template<typename T, qpl::size N>
		struct possibilities {
			std::array<T, N> values;

			constexpr possibilities(const std::array<T, N>& values) : values() {
				this->values = values;
			}

			constexpr possibilities_iterator<T, N> cbegin() const {
				possibilities_iterator<T, N> it(this->values);
				return it;
			}
			constexpr possibilities_iterator<T, N> begin() const {
				return this->cbegin();
			}

			constexpr possibilities_iterator<T, N> cend() const {
				possibilities_iterator<T, N> it;
				it.set_end();
				return it;
			}
			constexpr possibilities_iterator<T, N> end() const {
				return this->cend();
			}


			constexpr possibilities_reverse_iterator<T, N> crbegin() const {
				possibilities_reverse_iterator<T, N> it(this->values);
				return it;
			}
			constexpr possibilities_reverse_iterator<T, N> rbegin() const {
				return this->crbegin();
			}

			constexpr possibilities_reverse_iterator<T, N> crend() const {
				possibilities_reverse_iterator<T, N> it;
				it.set_end();
				return it;
			}
			constexpr possibilities_reverse_iterator<T, N> rend() const {
				return this->crend();
			}
		};

		template<typename T, qpl::size N>
		struct reverse_possibilities {
			std::array<T, N> values;

			constexpr reverse_possibilities(const std::array<T, N>& values) : values() {
				this->values = values;
			}

			constexpr possibilities_reverse_iterator<T, N> cbegin() const {
				possibilities_reverse_iterator<T, N> it(this->values);
				return it;
			}
			constexpr possibilities_reverse_iterator<T, N> begin() const {
				return this->cbegin();
			}

			constexpr possibilities_reverse_iterator<T, N> cend() const {
				possibilities_reverse_iterator<T, N> it;
				it.set_end();
				return it;
			}
			constexpr possibilities_reverse_iterator<T, N> end() const {
				return this->cend();
			}


			constexpr possibilities_iterator<T, N> crbegin() const {
				possibilities_iterator<T, N> it(this->values);
				return it;
			}
			constexpr possibilities_iterator<T, N> rbegin() const {
				return this->crbegin();
			}

			constexpr possibilities_iterator<T, N> crend() const {
				possibilities_iterator<T, N> it;
				it.set_end();
				return it;
			}
			constexpr possibilities_iterator<T, N> rend() const {
				return this->crend();
			}
		};
	}

	template<typename... Ts>
	constexpr auto list_possibilities(Ts... values) {
		return qpl::impl::possibilities(qpl::tuple_to_array(values...));
	}
	template<typename... Ts>
	constexpr auto list_reverse_possibilities(Ts... values) {
		return qpl::impl::reverse_possibilities(qpl::tuple_to_array(values...));
	}

	template<std::integral T>
	constexpr T int_pow(T b, T e) {
		return (e == 0) ? T{ 1 } : b * int_pow(b, e - 1);
	}

	template<typename C>
	std::pair<qpl::container_subtype<C>, qpl::container_subtype<C>> min_max_vector(const C& data) {
		auto v = std::minmax_element(data.cbegin(), data.cend());
		return { *(v.first), *(v.second) };
	}

	template<typename T, typename T2>
	std::pair<std::decay_t<T>, std::decay_t<T>> min_max_vector(const std::span<T>& data, T2 skip_size) {
		if (data.empty()) {
			return std::make_pair(std::decay_t<T>{}, std::decay_t<T>{});
		}
		std::decay_t<T> min = data.front();
		std::decay_t<T> max = data.front();
		for (T2 i = 0; i < data.size(); i += skip_size) {
			min = qpl::min(min, data[i]);
			max = qpl::min(max, data[i]);
		}
		return std::make_pair(min, max);
	}

	template<typename T>
	constexpr T abs(T n) {
		if constexpr (qpl::is_signed<T>()) {
			return (n < T{} ? -n : n);
		}
		else {
			return n;
		}
	}

	template<typename F, typename T>
	constexpr F mod_abs(F n, T m) {
		return (m + (n % m)) % m;
	}

	template<typename T>
	constexpr T clamp(T min, T value, T max) {
		return qpl::min(max, qpl::max(min, value));
	}

	template<typename T>
	constexpr T clamp_0_1(T value) {
		return qpl::min(T{ 1 }, qpl::max({ 0 }, value));
	}
	template<typename T>
	constexpr T change_range(T start, T end, T value, T value_min = 0, T value_max = 1) {
		auto progress = (value - value_min) / (value_max - value_min);
		return (start + (progress * (end - start)));
	}

	template<typename T1, typename T2>
	constexpr T1 loop_index(T1 n, T2 size) {
		if (n < 0) {
			n *= -1;
			n %= size;
			return static_cast<T1>(size) - n;
		}
		return n % static_cast<T1>(size);
	}


	template<qpl::size N>
	std::array<qpl::u8, N> static_split(const std::string_view& string, const std::string& delimiter = ",") {
		std::array<qpl::u8, N> result;
		unsigned ctr = 0u;
		for (int i = 0; i < string.size(); ++i) {
			if (string[i] == ',') {
				result[ctr++] = i;
			}
		}
		return result;
	}


	template<typename T>
	constexpr T subtract_if(bool condition, T value, T subtract) {
		return condition ? (subtract - value) : value;
	}
	template<typename T>
	constexpr T add_if(bool condition, T value, T subtract) {
		return condition ? (subtract + value) : value;
	}

	template<typename T>
	constexpr T rotate_left(T val, qpl::size rotation) {
		return (rotation >= qpl::bits_in_type<T>() ? rotate_left(val, rotation % qpl::bits_in_type<T>()) : ((val << rotation) | (val >> (qpl::bits_in_type<T>() - rotation))));
	}
	template<typename T>
	constexpr T rotate_right(T val, qpl::size rotation) {
		return (rotation >= qpl::bits_in_type<T>()) ? rotate_right(val, rotation % qpl::bits_in_type<T>()) : ((val >> rotation) | (val << (qpl::bits_in_type<T>() - rotation)));
	}

	template<typename T, typename F>
	constexpr auto linear_interpolation(T a, T b, F delta) {
		return static_cast<T>(a * (F{ 1 } - delta) + (b * delta));
	}


	template<typename T, typename F>
	constexpr auto linear_interpolation_2D(T xx, T xy, T yx, T yy, F fx, F fy) {
		return qpl::linear_interpolation(qpl::linear_interpolation(xx, xy, fx), qpl::linear_interpolation(yx, yy, fx), fy);
	}


	template<typename T>
	std::vector<std::decay_t<T>> linear_vector_interpolation(std::span<T> data, qpl::size interpolations, qpl::size index_skip_size = 1u) {
		if (data.empty()) {
			return {};
		}
		if (data.size() == 1u) {
			return std::vector<std::decay_t<T>>{ data[0] };
		}


		std::vector<std::decay_t<T>> result(qpl::size_cast(data.size() / index_skip_size * interpolations));
		if (result.size() == data.size()) {
			for (qpl::u32 i = 0; i < result.size(); ++i) {
				result[i] = data[i];
			}
			return result;
		}

		std::decay_t<T> a, b;

		for (qpl::u32 i = 0u; i < result.size(); ++i) {
			auto f = i / static_cast<qpl::f64>(result.size() - 1);

			auto index = static_cast<qpl::u32>(f * (data.size() / index_skip_size - 1));

			a = b = data[index * index_skip_size];

			if ((index * index_skip_size) + 1 < data.size()) {
				b = data[(index * index_skip_size) + 1];
			}

			auto left_over = ((data.size() / index_skip_size - 1) * f) - index;

			result[i] = qpl::linear_interpolation(a, b, left_over);
		}

		return result;
	}


	template<typename T>
	std::decay_t<T> linear_vector_interpolation_at(std::span<T> data, qpl::f64 progress) {
		if (data.empty()) {
			return {};
		}

		progress = qpl::clamp_0_1(progress);
		auto index = static_cast<qpl::u32>(progress * (data.size() - 1));

		std::decay_t<T> a = data[index];
		std::decay_t<T> b = a;

		if (index < data.size() - 1) {
			b = data[index + 1];
		}

		auto left_over = ((data.size() - 1) * progress) - index;

		return qpl::linear_interpolation(a, b, left_over);
	}


	template<typename T, typename F>
	constexpr auto linear_interpolation(const std::vector<T>& data, F delta) {
		return qpl::linear_vector_interpolation_at(qpl::make_span(data), delta);
	}
	template<typename T, typename F>
	constexpr auto linear_interpolation(const std::initializer_list<T>& data, F delta) {
		return qpl::linear_vector_interpolation_at(qpl::make_span(data), delta);
	}



	template<typename T, typename F>
	constexpr auto cubic_interpolation(T a, T b, T c, T d, F delta) {
		return
			(d - c - a + b) * (delta * delta * delta) +
			(a * 2 - b * 2 - d + c) * (delta * delta) +
			(c - a) * delta + b;
	}
	template<typename T, typename F>
	constexpr auto cubic_interpolation(T a, T b, F delta) {
		return qpl::cubic_interpolation(a, a, b, b, delta);
	}



	template<typename T, typename F>
	constexpr auto cubic_interpolation_2D(
		T a1, T b1, T c1, T d1,
		T a2, T b2, T c2, T d2,
		T a3, T b3, T c3, T d3,
		T a4, T b4, T c4, T d4,
		F fx, F fy) {

		return qpl::cubic_interpolation(
			qpl::cubic_interpolation(a1, b1, c1, d1, fx),
			qpl::cubic_interpolation(a2, b2, c2, d2, fx),
			qpl::cubic_interpolation(a3, b3, c3, d3, fx),
			qpl::cubic_interpolation(a4, b4, c4, d4, fx), fy);
	}
	template<typename T, typename F>
	constexpr auto cubic_interpolation_2D(T xx, T xy, T yx, T yy, F fx, F fy) {
		return qpl::cubic_interpolation(qpl::cubic_interpolation(xx, xy, fx), qpl::cubic_interpolation(yx, yy, fx), fy);
	}


	template<typename T>
	std::vector<std::decay_t<T>> cubic_vector_interpolation(std::span<T> data, qpl::size interpolations, qpl::size index_skip_size = 1u) {
		if (data.empty()) {
			return {};
		}
		if (data.size() == 1u) {
			return std::vector<std::decay_t<T>>{ data[0] };
		}
		std::vector<std::decay_t<T>> result(qpl::size_cast(data.size() / index_skip_size * interpolations));

		std::decay_t<T> a, b, c, d;

		for (qpl::u32 i = 0u; i < result.size(); ++i) {
			auto f = i / static_cast<qpl::f64>(result.size() - 1);

			auto index = static_cast<qpl::u32>(f * (data.size() / index_skip_size - 1));

			c = a = b = data[index * index_skip_size];
			if (index >= 1u) {
				a = data[(index - 1) * index_skip_size];
			}

			if ((index + 1) * index_skip_size < data.size()) {
				c = data[(index + 1) * index_skip_size];
			}
			d = c;

			if ((index + 2) * index_skip_size < data.size()) {
				d = data[(index + 2) * index_skip_size];
			}

			auto left_over = ((data.size() / index_skip_size - 1) * f) - index;

			result[i] = qpl::cubic_interpolation(a, b, c, d, left_over);
		}

		return result;
	}


	template<typename T>
	std::decay_t<T> cubic_vector_interpolation_at(std::span<T> data, qpl::f64 progress) {
		if (data.empty()) {
			return {};
		}
		progress = qpl::clamp_0_1(progress);
		auto index = static_cast<qpl::u32>(progress * (data.size() - 1));
		std::decay_t<T> a, b, c, d;

		c = a = b = data[index];
		if (index >= 1u) {
			a = data[(index - 1)];
		}

		if ((index + 1) < data.size()) {
			c = data[(index + 1)];
		}
		d = c;

		if ((index + 2) < data.size()) {
			d = data[(index + 2)];
		}

		auto left_over = ((data.size() - 1) * progress) - index;

		return qpl::cubic_interpolation(a, b, c, d, left_over);
	}


	template<typename T, typename F>
	constexpr auto cubic_interpolation(const std::vector<T>& data, F delta) {
		return qpl::cubic_vector_interpolation_at(qpl::make_span(data), delta);
	}
	template<typename T, typename F>
	constexpr auto cubic_interpolation(const std::initializer_list<T>& data, F delta) {
		return qpl::cubic_vector_interpolation_at(qpl::make_span(data), delta);
	}

	template<typename T>
	std::pair<std::decay_t<T>, std::decay_t<T>> cubic_vector_interpolation_min_max(std::span<T> data, qpl::size interpolations, qpl::f64 interpolation_offset = 0.0, qpl::size index_skip_size = 1u, std::decay_t<T> low = qpl::type_max<T>(), std::decay_t<T> high = qpl::type_min<T>()) {
		if (data.empty()) {
			return std::make_pair(T{}, T{});
		}
		if (data.size() == 1u) {
			return std::make_pair(data[0], data[0]);
		}

		interpolation_offset = qpl::clamp_0_1(interpolation_offset);


		auto index_offset = interpolation_offset * interpolations;

		std::decay_t<T> a, b, c, d;

		auto size = qpl::size_cast(data.size() / index_skip_size * interpolations - index_offset);
		for (qpl::u32 i = 0u; i < size; ++i) {
			auto f = (i + index_offset) / static_cast<qpl::f64>(size - 1);

			auto index = static_cast<qpl::u32>(f * (data.size() / index_skip_size - 1));

			c = a = b = data[index * index_skip_size];
			if (index >= 1u) {
				a = data[(index - 1) * index_skip_size];
			}

			if ((index + 1) * index_skip_size < data.size()) {
				c = data[(index + 1) * index_skip_size];
			}
			d = c;

			if ((index + 2) * index_skip_size < data.size()) {
				d = data[(index + 2) * index_skip_size];
			}

			auto left_over = ((data.size() / index_skip_size - 1) * f) - index;

			auto point = qpl::cubic_interpolation(a, b, c, d, left_over);


			low = qpl::min(point, low);
			high = qpl::max(point, high);
		}

		return std::make_pair(low, high);
	}

	template<typename F>
	inline F cumulative_normal_distribution(F x, F c = F{ 3 }) {
		return F{ 0.5 } *erfc(-(x * 2 * c - c) * static_cast<F>(qpl::sqrt1_2));
	}

	template<typename F>
	inline F cumulative_pow_distribution(F x, F p, F c = F{ 3 }) {
		return std::pow(qpl::cumulative_normal_distribution(x, c), p);
	}

	template<typename F>
	constexpr F curve_slope(F progress, F slope = 2.0) {
		return std::pow(1 - std::pow(1 - progress, slope), 1.0 / slope);
	}
	template<typename F>
	constexpr F curve_slope_inverse(F n, F slope = 2.0) {
		return 1 - std::pow(1 - std::pow(n, slope), 1.0 / slope);
	}


	constexpr std::array<std::pair<qpl::f64, qpl::f64>, 23> slope_values = {
		std::pair{0.00000000000000000, 0.00000000000000000},
		std::pair{0.00008000000000000, 0.03314374470072128},
		std::pair{0.00012000000000000, 0.03897919509779974},
		std::pair{0.00018000000000000, 0.04584178845821749},
		std::pair{0.00027000000000000, 0.05391210997762626},
		std::pair{0.00040500000000000, 0.06340233339708143},
		std::pair{0.00060750000000000, 0.07456162357553127},
		std::pair{0.00091125000000000, 0.08768236842376569},
		std::pair{0.00136687500000000, 0.10310729301498289},
		std::pair{0.00205031250000000, 0.12123744666133446},
		std::pair{0.00307546875000000, 0.14254092614039940},
		std::pair{0.00461320312500000, 0.16756195149924544},
		std::pair{0.00691980468750000, 0.19692944941761062},
		std::pair{0.01037970703125000, 0.23136345252564658},
		std::pair{0.01556956054687501, 0.27167608658909514},
		std::pair{0.02335434082031251, 0.31876114394764743},
		std::pair{0.03503151123046876, 0.37356124233337512},
		std::pair{0.05254726684570314, 0.43699254043729757},
		std::pair{0.07882090026855471, 0.50979060096969120},
		std::pair{0.11823135040283206, 0.59221106275204727},
		std::pair{0.17734702560424809, 0.68346377446896467},
		std::pair{0.26602053840637213, 0.78065875326097234},
		std::pair{0.39903080760955822, 0.87687417030023973},
	};

	template<typename F>
	constexpr F smooth_slope_impl(F progress, F slope, F cutoff, F sub) {
		auto stretch = (1 - cutoff);
		auto multiply = 1.0 / (1 - sub);
		return (curve_slope(progress * stretch + cutoff, slope) - sub) * multiply;
	}

	template<typename F>
	constexpr F smooth_slope(F progress, qpl::u32 strength = 21u) {
		auto p = qpl::clamp_0_1(progress);
		if (strength >= slope_values.size()) {
			auto diff = std::pow(0.9, (strength - (slope_values.size() - 1)) / 2.0);
			auto val = smooth_slope_impl(p, F{ 2.5 }, slope_values.back().first, slope_values.back().second);
			return qpl::linear_interpolation(p, val, diff);
		}
		else {
			auto slope_info = slope_values[strength];
			return smooth_slope_impl(p, F{ 2.5 }, slope_info.first, slope_info.second);
		}
	}

	template<typename F>
	constexpr F smooth_slope_triangle(F progress, qpl::u32 strength = 21u) {
		if (progress >= 0.5) {
			return smooth_slope_triangle(1.0 - progress, strength);
		}
		return smooth_slope(progress * 2, strength);
	}

	template<typename F>
	constexpr F smooth_slope_s(F progress, qpl::u32 strength = 21u) {
		if (progress >= 0.5) {
			return 1 - smooth_slope_s(1 - progress, strength);
		}
		return 0.5 - smooth_slope(1 - progress * 2.0, strength) / 2;
	}


	template<typename F>
	constexpr F smooth_curve(F progress, F slope) {
		return curve_slope(qpl::clamp_0_1(progress), slope);
	}

	template<typename F>
	constexpr F smooth_curve_triangle(F progress, F slope) {
		if (progress >= 0.5) {
			return smooth_curve_triangle(1.0 - progress, slope);
		}
		return smooth_curve(progress * 2, slope);
	}
	template<typename F>
	constexpr F triangle_progression(F progress) {
		if (progress < 0.5) {
			return progress * 2;
		}
		else {
			return (1 - (progress - 0.5) * 2);
		}
	}
	template<typename F>
	constexpr F triangle_progression_fmod1(F n) {
		return triangle_progression(std::fmod(n, F{ 1 }));
	}

	template<typename F>
	constexpr F smooth_curve_s(F progress, F slope) {
		if (progress >= 0.5) {
			return 1 - smooth_curve_s(1 - progress, slope);
		}
		return 0.5 - smooth_curve(1 - progress * 2.0, slope) / 2;
	}



	template<typename T, typename F>
	constexpr T smooth_slope_interpolation(T a, T b, F x, qpl::u32 strength = 21u) {
		auto n = qpl::smooth_slope(x, strength);
		return a * (F{ 1 } - n) + (b * n);
	}
	template<typename T, typename F>
	constexpr T smooth_slope_triangle_interpolation(T a, T b, F x, qpl::u32 strength = 21u) {
		auto n = qpl::smooth_slope_triangle(x, strength);
		return a * (F{ 1 } - n) + (b * n);
	}
	template<typename T, typename F>
	constexpr T smooth_slope_s_interpolation(T a, T b, F x, qpl::u32 strength = 21u) {
		auto n = qpl::smooth_slope_s(x, strength);
		return a * (F{ 1 } - n) + (b * n);
	}


	template<typename F>
	constexpr F normal_distribution(F x) {
		constexpr auto exp = 0.3989422804014327;
		return exp * std::pow(qpl::e, -0.5 * std::pow(x, 2));
	}
	template<typename F>
	constexpr F normal_distribution(F x, F d = F{ 1 }, F u = F{ 0.0 }) {
		auto exp = 1.0 / (d * std::pow(2 * qpl::pi, 0.5));
		return exp * std::pow(qpl::e, -0.5 * std::pow((x - u) / d, 2));
	}
	template<typename F>
	constexpr F sigma(qpl::size N, F p) {
		return std::pow(N * p * (1 - p), 0.5);
	}

	template<typename F>
	constexpr F s_slope(F x, F slope = 1) {
		return
			x < 1 ? (
				F{ 1 } - std::pow(
					(F{ 1 } - x),
					slope
				)
				)
			: (
				F{ 1 } + std::pow(
					(x - F{ 1 }),
					slope
				)
				);
	}

	template<typename F>
	qpl::f64 inverse_tanh(F n) {
		return (std::log((1 + n) / (1 - n))) / 2;
	}
}

#endif