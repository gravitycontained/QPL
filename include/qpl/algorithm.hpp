#ifndef QPL_ALGORITHM_HPP
#define QPL_ALGORITHM_HPP
#pragma once


#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>
#include <qpl/span.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <tuple>

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

	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::size number_of_digits(T value, T base = T{ 10 }) {
		return value < base ? 1 : 1 + number_of_digits(value / base, base);
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T approximate_multiple_down(T value, T multiple) {
		return (value / multiple) * multiple;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
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
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T factorial(T a) {
		return a <= T{ 1 } ? T{ 1 } : (a == T{ 2 } ? T{ 2 } : static_cast<T>(a * qpl::factorial(a - 1)));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T log(T a, T b) {
		return b < a ? T{} : T{ 1 } + qpl::log(a, static_cast<T>(b / a));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr qpl::i64 log2(T a) {
		return a ? qpl::i64{ 1 } + qpl::log2(static_cast<T>(a >> 1)) : qpl::i64{ -1 };
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T shift(T a, T b) {
		return b == T{} ? T{ 1 } : static_cast<T>(a * qpl::pow<T>(a, b - T{ 1 }));
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr qpl::size significant_digit(T n, T base = 10) {
		return n ? qpl::size{ 1 } + qpl::significant_digit(static_cast<T>(n / base), base) : qpl::size{};
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr qpl::size significant_bit(T n) {
		return n ? qpl::size{ 1 } + qpl::significant_bit(static_cast<T>(n >> 1)) : qpl::size{};
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


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr qpl::size number_of_set_bits(T n) {
		return qpl::number_of_set_bits(qpl::u64_cast(n));
	}
	template<>
	constexpr qpl::size number_of_set_bits(qpl::u64 n) {
		n = n - ((n >> 1) & 0x5555'5555'5555'5555ull);
		n = (n & 0x3333'3333'3333'3333ull) + ((n >> 2) & 0x3333'3333'3333'3333ull);
		return (((n + (n >> 4)) & 0x0F0F'0F0F'0F0F'0F0Full) * 0x0101'0101'0101'0101ull) >> 56;
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr bool get_bit(T value, qpl::size position) {
		return qpl::bool_cast(value & (T{ 1 } << position));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void set_bit(T& value, qpl::size position, bool flag) {
		value = flag ? (value | (T{ 1 } << position)) : (value & ~(T{ 1 } << position));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T set_bit_copied(T value, qpl::size position, bool flag) {
		return flag ? (value | (T{ 1 } << position)) : (value & ~(T{ 1 } << position));
	}


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void toggle_bit(T& value, qpl::size position) {
		value = (value & (T{ 1 } << position)) ? (value & ~(T{ 1 } << position)) : (value | (T{ 1 } << position));
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_bits(T& value) {
		value = ~value;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_bits(T value) {
		return ~value;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_bits_until(T& value, qpl::size position) {
		value = (value & (~T{} << position)) | (~value & ~(~T{} << position));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_bits_until(T value, qpl::size position) {
		return (value & (~T{} << position)) | (~value & ~(~T{} << position));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_bits_until_msb(T& value) {
		qpl::flip_bits_until(value, qpl::significant_bit(value));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_bits_until_msb(T value) {
		return qpl::flipped_bits_until(value, qpl::significant_bit(value));
	}



	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
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


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T reverse_bits_copied(T value) {
		T copy;
		qpl::reverse_bits(copy);
		return copy;
	}


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T get_digit(T value, qpl::size position, T base = 10) {
		return static_cast<T>((value / qpl::pow<qpl::size>(base, position)) % base);
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
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
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T set_digit_copied(T value, qpl::size position, T digit, T base = 10) {
		auto copy = value;
		qpl::set_digit(copy, position, digit, base);
		return copy;
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T log_approximation(T a, T b, T c = T{ 1 }) {
		return c > static_cast<T>(b / a) ? c : qpl::log_approximation(a, b, static_cast<T>(c * a));
	}



	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr bool is_power_of_two(T n) {
		return n == T{} ? false : (n == (T{ 1 } << (qpl::significant_bit(n) - 1)));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr bool base_full_bit_usage(T base) {
		return is_power_of_two(base) ? qpl::bits_in_type<T>() % (qpl::significant_bit(base) - 1) == 0 : false;
	}


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T base_max_log(T base) {
		return qpl::base_full_bit_usage(base) ? static_cast<T>(qpl::bits_in_type<T>() / (qpl::significant_bit(base) - 1)) : qpl::log<T>(base, T{ 1 } << (qpl::bits_in_type<T>() - 1));
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T base_max(T base) {
		return qpl::base_full_bit_usage(base) ? T{} : qpl::pow(base, qpl::base_max_log(base));
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_digits(T& value, qpl::u32 base = 10) {
		value = qpl::base_max<T>(static_cast<T>(base)) - 1 - value;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_digits(T value, qpl::u32 base = 10) {
		return qpl::base_max<T>(static_cast<T>(base)) - 1 - value;
	}


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_digits_until(T& value, qpl::size position, qpl::u32 base = 10) {
		auto pow = qpl::pow<qpl::size>(base, position);
		value = (value + (pow - 1 - (value % pow) * 2));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_digits_until(T value, qpl::size position, qpl::u32 base = 10) {
		auto pow = qpl::pow<qpl::size>(base, position);
		return (value + (pow - 1 - (value % pow) * 2));
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr void flip_digits_until_msd(T& value, qpl::u32 base = 10) {
		qpl::flip_digits_until(value, qpl::significant_digit(value, base), base);
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	constexpr T flipped_digits_until_msd(T value, qpl::u32 base = 10) {
		return qpl::flipped_digits_until(value, qpl::significant_digit(value, base), base);
	}


	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	T constexpr cantor_pairing(T x, T y) {
		return (x + y) * (x + y + 1) / 2 + y;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	std::pair<T, T> cantor_pairing_inverse(T z) {
		std::pair<T, T> result;
		auto w = static_cast<T>((std::sqrt(T{ 8 } *z + T{ 1 }) - T{ 1 }) / T{ 2 });
		result.second = z - (w + T{ 1 }) * w / T{ 2 };
		result.first = w - result.second;
		return result;
	}

	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	T constexpr rosenberg_pairing(T x, T y) {
		return ((x > y ? x : y) * (x > y ? x : y)) + (x > y ? x : y) + x - y;
	}
	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
	std::pair<T, T> rosenberg_pairing_inverse(T z) {
		auto m = static_cast<T>(std::sqrt(z));
		if (z - m * m < m) {
			return std::make_pair(z - m * m, m);
		}
		else {
			return std::make_pair(m, m * m + T{ 2 } *m - z);
		}
	}



	template<typename T, QPLCONCEPT(qpl::is_integer<T>())>
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
	constexpr bool is_equal_to_any(compare first, Args... rest) {
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
			return unpack_tuple(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)> - 1>());
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


	template<typename C, QPLCONCEPT(qpl::is_container<C>() && qpl::has_size<C>())>
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
	template<typename C>
	constexpr qpl::size container_depth() {
		return qpl::is_container<C> ?
			(qpl::is_container<qpl::container_subtype<C>>() ? qpl::container_depth<qpl::container_subtype<C>>() + qpl::size{ 1 } : qpl::size{ 1 }) :
			qpl::size{};
	}



	template<typename C>
	auto container_sum(const C& data) {
		if constexpr (!qpl::is_container<C>()) {
			return data;
		}
		else {
			auto sum = data[0];
			for (qpl::u32 i = 1u; i < data.size(); ++i) {
				sum += data[i];
			}
			return sum;
		}
	}


	template<typename C1, typename C2>
	void combine_containers(C1& destination, const C2& source) {
		destination.reserve(destination.size() + source.size());
		destination.insert(destination.end(), source.cbegin(), source.cend());
	}

	template<typename T, typename R>
	std::vector<R> vector_subtype_conversion(const std::vector<T>& source) {
		std::vector<R> result;
		result.resize(source.size());
		for (qpl::u32 i = 0; i < source.size(); ++i) {
			result[i] = static_cast<R>(source[i]);
		}
		return result;
	}
	template<typename R, typename T>
	std::vector<R> vector_subtype_reinterpret(const std::vector<T>& source) {
		std::vector<R> result;
		result.resize(source.size());
		memcpy(result.data(), source.data(), source.size() * sizeof(T));
		return result;
	}

	template<typename R, typename T, qpl::size N>
	std::array<R, N> array_subtype_conversion(const std::array<T, N>& source) {
		std::array<R, N> result;
		for (qpl::u32 i = 0; i < source.size(); ++i) {
			result[i] = static_cast<R>(source[i]);
		}
		return result;
	}
	template<typename R, typename T, qpl::size N>
	std::array<R, N> array_subtype_reinterpret(const std::array<T, N>& source) {
		std::array<R, N> result;
		memcpy(result.data(), source.data(), source.size() * sizeof(T));
		return result;
	}
	template<typename T, qpl::size N>
	std::vector<T> array_to_vector(const std::array<T, N>& source) {
		std::vector<T> result(source.size());
		memcpy(result.data(), source.data(), source.size() * sizeof(T));
		return result;
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
			((result[Ints] = std::get<Ints>(tuple)), ...);
		};
		unpack(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)>>());
		return result;
	}
	template<typename T, typename... Args>
	std::vector<T> tuple_to_vector(std::tuple<Args...> tuple) {
		std::vector<T> result(std::tuple_size_v<decltype(tuple)>);
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = std::get<Ints>(tuple)), ...);
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

	template<typename... Args>
	constexpr auto tuple_to_array(std::tuple<Args...> tuple) -> std::array<std::tuple_element_t<0, decltype(tuple)>, std::tuple_size_v<decltype(tuple)>> {
		std::array<std::tuple_element_t<0, decltype(tuple)>, std::tuple_size_v<decltype(tuple)>> result{};
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = std::get<Ints>(tuple)), ...);
		};
		unpack(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)>>());
		return result;
	}
	template<typename T, typename... Args>
	constexpr std::array<T, sizeof...(Args)> tuple_to_array(std::tuple<Args...> tuple) {
		std::array<T, sizeof...(Args)> result{};
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(Tuple tuple, std::index_sequence<Ints...>) {
			((result[Ints] = std::get<Ints>(tuple)), ...);
		};
		unpack(tuple, std::make_index_sequence<std::tuple_size_v<decltype(tuple)>>());
		return result;
	}
	template<typename T, typename... Args>
	constexpr auto to_array(Args... args) {
		return qpl::tuple_to_array<T>(std::make_tuple(args...));
	}
	template<typename... Args>
	constexpr auto to_array(Args... args) {
		return qpl::tuple_to_array(std::make_tuple(args...));
	}
	template<typename It>
	constexpr auto make_span(It begin, It end) {
		return qpl::span<std::remove_pointer_t<It::pointer>>(&(*begin), std::distance(begin, end));
	}
	template<typename C>
	constexpr auto make_span(const C& container) {
		return qpl::make_span(container.begin(), container.end());
	}


	template<typename T, typename T1 = T>
	std::vector<T> vector_0_to_n(T n, T1 shift = T{}) {
		std::vector<T> vec(n);
		std::iota(vec.begin(), vec.end(), shift);
		return vec;
	}

	template<typename C>
	std::pair<qpl::container_subtype<C>, qpl::container_subtype<C>> min_max_vector(const C& data) {
		auto v = std::minmax_element(data.cbegin(), data.cend());
		return { *(v.first), *(v.second) };
	}

	template<typename T>
	std::pair<std::decay_t<T>, std::decay_t<T>> min_max_vector(const qpl::span<T>& data, qpl::u32 skip_size) {
		if (data.empty()) {
			return std::make_pair(std::decay_t<T>{}, std::decay_t<T>{});
		}
		std::decay_t<T> min = data.front();
		std::decay_t<T> max = data.front();
		for (int i = 0; i < data.size(); i += skip_size) {
			min = qpl::min(min, data[i]);
			max = qpl::min(max, data[i]);
		}
		return std::make_pair(min, max);
	}


	template<typename T>
	std::vector<T> vector_including_values(const std::vector<T>& target, const std::vector<T>& compare) {
		std::vector<T> result;
		for (auto& i : compare) {
			auto find = std::find(target.cbegin(), target.cend(), i);
			if (find != target.cend()) {
				result.push_back(*find);
			}
		}

		return result;
	}
	template<typename T>
	std::vector<T> vector_including_values(const std::vector<std::vector<T>>& targets, const std::vector<T>& compare) {
		auto sum = compare;
		for (auto& i : targets) {
			auto v = qpl::vector_including_values(i, compare);
			sum = qpl::vector_including_values(v, sum);
		}
		return sum;
	}
	template<typename T>
	std::vector<T> vector_excluding_values(const std::vector<T>& target, const std::vector<T>& compare) {
		std::vector<T> result;
		for (auto& i : compare) {
			auto find = std::find(target.cbegin(), target.cend(), i);
			if (find == target.cend()) {
				result.push_back(i);
			}
		}

		return result;
	}
	template<typename T>
	std::vector<T> vector_excluding_values(const std::vector<std::vector<T>>& targets, const std::vector<T>& compare) {
		auto sum = compare;
		for (auto& i : targets) {
			auto v = qpl::vector_excluding_values(i, compare);
			sum = qpl::vector_including_values(v, sum);
		}
		return sum;
	}

	template<typename T>
	void vector_flip_x_axis(std::vector<std::vector<T>>& source) {
		auto copy = source;
		for (qpl::u32 y = 0u; y < copy.size(); ++y) {
			for (qpl::u32 x = 0u; x < copy[0].size(); ++x) {
				source[y][x] = copy[y][copy[0].size() - 1 - x];
			}
		}
	}
	template<typename T>
	void vector_flip_y_axis(std::vector<std::vector<T>>& source) {
		auto copy = source;
		for (qpl::u32 y = 0u; y < copy.size(); ++y) {
			for (qpl::u32 x = 0u; x < copy[0].size(); ++x) {
				source[y][x] = copy[copy.size() - 1 - y][x];
			}
		}
	}

	template<typename T>
	std::vector<std::vector<T>> vector_rotate_right_copy(std::vector<std::vector<T>> source) {
		std::vector<std::vector<T>> result(source[0].size(), std::vector<T>(source.size()));
		for (qpl::u32 y = 0u; y < result.size(); ++y) {
			for (qpl::u32 x = 0u; x < source.size(); ++x) {
				result[y][x] = source[source.size() - 1 - x][y];
			}
		}
		return result;
	}
	template<typename T>
	void vector_rotate_right(std::vector<std::vector<T>>& source) {
		source = vector_rotate_right_copy(source);
	}
	template<typename T>
	std::vector<std::vector<T>> vector_flip_x_axis_copy(std::vector<std::vector<T>> source) {
		std::vector<std::vector<T>> result(source.size(), std::vector<T>(source[0].size()));
		for (qpl::u32 y = 0u; y < source.size(); ++y) {
			for (qpl::u32 x = 0u; x < source[0].size(); ++x) {
				result[y][x] = source[y][source[0].size() - 1 - x];
			}
		}
		return result;
	}
	template<typename T>
	std::vector<std::vector<T>> vector_flip_y_axis_copy(std::vector<std::vector<T>> source) {
		std::vector<std::vector<T>> result(source.size(), std::vector<T>(source[0].size()));
		for (qpl::u32 y = 0u; y < source.size(); ++y) {
			for (qpl::u32 x = 0u; x < source[0].size(); ++x) {
				result[y][x] = source[source.size() - 1 - y][x];
			}
		}
		return result;
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
	template<typename T>
	constexpr T clamp(T min, T value, T max) {
		return qpl::min(max, qpl::max(min, value));
	}

	template<typename T>
	constexpr T clamp_0_1(T value) {
		return qpl::min(T{ 1 }, qpl::max({ 0 }, value));
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
	std::array<qpl::u8, N> static_split(const qpl::string_view& string, const std::string& delimiter = ",") {
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
	constexpr qpl::u8 rotate_left(T val, qpl::size rotation) {
		return (rotation >= qpl::bits_in_type<T>() ? rotate_left(val, rotation % qpl::bits_in_type<T>()) : ((val << rotation) | (val >> (qpl::bits_in_type<T>() - rotation))));
	}
	template<typename T>
	constexpr qpl::u8 rotate_right(T val, qpl::size rotation) {
		return (rotation >= qpl::bits_in_type<T>()) ? rotate_right(val, rotation % qpl::bits_in_type<T>()) : ((val >> rotation) | (val << (qpl::bits_in_type<T>() - rotation)));
	}

	template<typename T, typename F>
	constexpr auto linear_interpolation(T a, T b, F delta) {
		return a * (F{ 1 } -delta) + (b * delta);
	}

	template<typename T, typename F>
	constexpr auto linear_interpolation_2D(T xx, T xy, T yx, T yy, F fx, F fy) {
		return qpl::linear_interpolation(qpl::linear_interpolation(xx, xy, fx), qpl::linear_interpolation(yx, yy, fx), fy);
	}


	template<typename T>
	std::vector<std::decay_t<T>> linear_vector_interpolation(qpl::span<T> data, qpl::size interpolations, qpl::size index_skip_size = 1u) {
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

			auto index = static_cast<qpl::u32>(f * (data.size() / index_skip_size - 1)) * index_skip_size;

			a = b = data[index * index_skip_size];

			if (index < data.size() / index_skip_size - 1) {
				b = data[(index * index_skip_size) + 1];
			}

			auto left_over = ((data.size() / index_skip_size - 1) * f) - index;

			result[i] = qpl::linear_interpolation(a, b, left_over);
		}

		return result;
	}


	template<typename T>
	std::decay_t<T> linear_vector_interpolation_at(qpl::span<T> data, qpl::f64 progress) {
		if (data.empty()) {
			return {};
		}

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
	std::vector<std::decay_t<T>> cubic_vector_interpolation(qpl::span<T> data, qpl::size interpolations, qpl::size index_skip_size = 1u) {
		if (data.empty()) {
			return {};
		}
		if (data.size() == 1u) {
			return std::vector<std::decay_t<T>>{ data[0] };
		}
		std::vector<std::decay_t<T>> result(qpl::size_cast(data.size() / index_skip_size * interpolations));

		//if (result.size() == data.size()) {
		//	for (qpl::u32 i = 0; i < result.size(); ++i) {
		//		result[i] = data[i];
		//	}
		//	return result;
		//}

		std::decay_t<T> a, b, c, d;

		for (qpl::u32 i = 0u; i < result.size(); ++i) {
			auto f = i / static_cast<qpl::f64>(result.size() - 1);

			auto index = static_cast<qpl::u32>(f * (data.size() / index_skip_size - 1));

			c = a = b = data[index * index_skip_size];
			if (index >= 1u) {
				a = data[(index - 1) * index_skip_size];
			}

			if (index < (data.size() - 1) / index_skip_size) {
				c = data[(index + 1) * index_skip_size];
			}
			d = c;

			if (index < (data.size() - 2) / index_skip_size) {
				d = data[(index + 2) * index_skip_size];
			}

			auto left_over = ((data.size() / index_skip_size - 1) * f) - index;

			result[i] = qpl::cubic_interpolation(a, b, c, d, left_over);
		}

		return result;
	}


	template<typename T>
	std::pair<std::decay_t<T>, std::decay_t<T>> cubic_vector_interpolation_min_max(qpl::span<T> data, qpl::size interpolations, qpl::f64 interpolation_offset = 0.0, qpl::size index_skip_size = 1u, std::decay_t<T> low = qpl::type_max<T>(), std::decay_t<T> high = qpl::type_min<T>()) {
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

			if (index < (data.size() - 1) / index_skip_size) {
				c = data[(index + 1) * index_skip_size];
			}
			d = c;

			if (index < (data.size() - 2) / index_skip_size) {
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
		return F{ 0.5 } * erfc(-(x * 2 * c - c) * static_cast<F>(qpl::sqrt1_2));
	}

	template<typename F>
	inline F cumulative_pow_distribution(F x, F p, F c = F{ 3 }) {
		return std::pow(qpl::cumulative_normal_distribution(x, c), p);
	}
	template<typename F>
	constexpr F smooth_progression(F x) {
		return qpl::clamp(F{}, (qpl::cumulative_pow_distribution(x, F{ 0.5 }, F{ 1.9 }) - F{ 0.17 }) * F { 1.22 }, F{ 1 });
	}

	template<typename F>
	constexpr F normal_distribution(F x, F o = F{ 1 }, F u = F{ 0.5 }) {
		return (F{ 1 } / (o * std::sqrt(2 * qpl::pi))) * std::pow(qpl::e, -std::pow((x - u) / o, 2) / F{ 2 });
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