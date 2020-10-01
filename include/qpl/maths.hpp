#ifndef QPL_MATHS_HPP
#define QPL_MATHS_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <cmath>
#include <vector>

namespace qpl {

	QPLDLL std::vector<std::vector<qpl::u32>> sudoku_sum_possibilities(qpl::size sum, qpl::size numbers);
	
	template<typename T>
	bool is_prime(T value) {
		if (value < 5u) {
			return value == 2u || value == 3u;
		}

		qpl::u32 add = 2u;
		auto sqrt = std::sqrt(value);
		for (qpl::u32 i = 5u; i < sqrt; i += add) {
			if (value % i == 0) {
				return false;
			}

			add = 6u - add;
		}
		return true;
	}


	template<typename T>
	std::vector<T> prime_factors(T value) {
		if (qpl::is_prime(value)) {
			return {};
		}
		std::vector<T> result;

		for (T i = 2; value > T{ 1 }; ++i) {
			if (qpl::is_prime(i)) {
				while (value % i == T{}) {
					result.push_back(i);
					value /= i;
				}
			}
		}
		return result;
	}


	template<typename T>
	std::vector<T> dividers(T value) {
		std::vector<T> result;

		for (T i = 1; i < value; ++i) {
			if (value % i == T{}) {
				result.push_back(i);
			}

		}
		return result;
	}

	template<typename T>
	qpl::u32 collatz_conjecture(T n) {
		qpl::u32 ctr = 0u;
		while (n != 1) {
			if (n % 2 == 0) {
				n >>= 1;
			}
			else {
				n *= 3;
				++n;
			}

			qpl::println(n);
			++ctr;
		}
		return ctr;
	}


	struct ema {
		qpl::f64 time_period;
		qpl::size time_ctr = 0u;
		qpl::f64 last_ma = 0.0;

		QPLDLL void reset();
		QPLDLL void add_value(double value);
		QPLDLL double get_average() const;
	};
}

#endif