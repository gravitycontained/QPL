#ifndef QPL_RANGES_HPP
#define QPL_RANGES_HPP
#pragma once

#include <qpl/random.hpp>
#include <qpl/qpldeclspec.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <vector>

namespace qpl {
	template<typename T, typename U = T>
	struct range {
		range() {

		}
		range(T a, T b, U c) {
			this->a = a;
			this->b = b;
			this->c = c;
		}
		range(T a, T b) {
			this->a = a;
			this->b = b;
		}
		range(T ab) {
			this->a = this->b = ab;
		}

		auto get_random() const {
			return qpl::random(this->a, this->b) * this->c;
		}
		qpl::size arrangement_size() const {
			return static_cast<qpl::size>((this->b - this->a) / this->c + 1);
		}
		std::vector<T> get_range() const {
			std::vector<T> result(this->arrangement_size());

			for (qpl::u32 i = 0u; i < result.size(); ++i) {
				result[i] = this->a + (this->c * i);
			}

			return result;
		}

		T a;
		T b;
		U c = U{ 1 };
	};

	template<typename T>
	std::vector<std::vector<T>> list_all_permutations(const std::vector<std::vector<T>>& ranges) {
		std::vector<std::vector<T>> result;

		std::vector<qpl::u32> indices(ranges.size());

		qpl::size sum = 1u;
		for (qpl::u32 i = 0u; i < ranges.size(); ++i) {
			auto possibilities = ranges[i];
			sum *= possibilities.size();
		}

		result.resize(sum);
		for (qpl::u32 i = 0u; i < sum; ++i) {
			result[i].resize(indices.size());
			for (qpl::u32 j = 0u; j < indices.size(); ++j) {
				auto index = indices.size() - 1 - j;
				if (i) {
					++indices[index];
				}
				if (indices[index] >= ranges[index].size()) {
					indices[index] = 0u;
				}
				else {
					break;
				}
			}

			for (qpl::u32 j = 0u; j < indices.size(); ++j) {
				result[i][j] = ranges[j][indices[j]];
			}
			//qpl::println_container(result[i]);
		}

		return result;
	}


	template<typename T>
	std::vector<std::vector<T>> list_all_permutations(const std::vector<T>& array) {


		std::vector<std::vector<T>> result;
		auto copy = array;

		for (qpl::i32 i = array.size() - 2; i >= 0; --i) {
			copy = array;

			//1 2 3
			//1 3 2


			for (qpl::i32 s = array.size() - 2; s >= i; --s) {
				std::swap(copy[s], copy[s + 1]);
				result.push_back(copy);
			}
		}
		return result;
	}

	template<typename T>
	std::vector<std::vector<T>> list_all_arrangements(const std::vector<T>& array, qpl::size size) {
		std::vector<qpl::u32> indices = qpl::vector_0_to_n<qpl::u32>(size);

		std::vector<std::vector<T>> result;
		while (true) {
			result.push_back({});
			result.back().resize(size);
			for (qpl::u32 i = 0u; i < size; ++i) {
				result.back()[i] = array[indices[i]];
			}

			qpl::size index = size - 1;

			qpl::size max = array.size() - 1;
			while (indices[index] >= max) {
				if (index < 1) {
					return result;
				}

				--max;
				--index;
			}

			++indices[index];
			for (qpl::size j = index; j < size; ++j) {
				if (j < size - 1) {
					indices[j + 1] = indices[j] + 1;
				}
			}
		}
		return result;
	}
}

#endif