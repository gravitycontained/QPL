#ifndef QPL_BIGINT_HPP
#define QPL_BIGINT_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/string.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>

#include <vector>
#include <array>
#include <iostream>

namespace qpl {
	
	
	template <typename T, qpl::size N, T B, bool S, T C>
	class bigint_impl {
	public:
		constexpr static bool is_signed() {
			return S;
		}
		constexpr static bool is_unsigned() {
			return !S;
		}
		constexpr static bool is_static() {
			return N != qpl::size{};
		}
		constexpr static bool is_dynamic() {
			return N == qpl::size{};
		}

		constexpr static qpl::size optimal_block_digits() {
			return static_cast<qpl::size>(qpl::base_max_log(B));
		}
		constexpr static qpl::size block_digits() {
			return static_cast<qpl::size>(C ? C : optimal_block_digits());
		}
		constexpr static qpl::size array_size() {
			return is_dynamic() ? qpl::size{} : static_cast<qpl::size>(((N - 1) / block_digits()) + 1);
		}
		constexpr static qpl::size last_block_digits() {
			return is_dynamic() ? qpl::size{} : static_cast<qpl::size>((N - 1 - S) % block_digits() + 1);
		}
		constexpr static bool is_uniform() {
			return block_digits() == last_block_digits();
		}
		constexpr static bool is_optimal_base() {
			return qpl::base_full_bit_usage(B);
		}
		constexpr static T block_limit() {
			return is_optimal_base() ? T{} : qpl::pow(B, static_cast<T>(block_digits()));
		}
	private:

	};
}

#endif


