#ifndef QPL_MATHS_HPP
#define QPL_MATHS_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <qpl/bits.hpp>

#include <cmath>
#include <sstream>
#include <vector>

namespace qpl {

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


	struct exponential_moving_average {
		exponential_moving_average(qpl::f64 time_period = 5.0) {
			this->time_period = time_period;
		}
		qpl::f64 time_period;
		qpl::size time_ctr = 0u;
		qpl::f64 last_ma = 0.0;

		QPLDLL void reset();
		QPLDLL void add_value(double value);
		QPLDLL double get_average() const;
	};


	template<typename T>
	struct fast_moving_average {
		void reset() {
			this->ctr = 0u;
			this->sum = T{ 0 };
			this->progress = 0u;
		}
		void set_time_period(qpl::u32 time_period) {
			this->time_period = time_period;
		}
		void set_step(qpl::u32 ctr) {
			this->ctr = ctr;
			this->sum = T{ 0 };
			this->progress = 0u;
		}
		qpl::size used_size() const {
			return qpl::min(this->time_period, this->progress);
		}
		template<typename R = qpl::f64>
		R get_average() const {
			return static_cast<R>(this->sum) / static_cast<R>(this->used_size());
		}
		void add(T add, T subtract) {

			this->sum += add;
			if (this->progress >= this->time_period) {
				this->sum -= subtract;
			}
			++this->ctr;
			++this->progress;
		}
		qpl::size get_needed_index(qpl::u32 index) const {
			return index - this->used_size();
		}


		T sum = T{ 0 };
		qpl::u32 time_period = 1u;
		qpl::u32 ctr = 0u;
		qpl::u32 progress = 0u;
	};


	enum class mathematical_operation {
		none, add, sub, mul, div, pow
	};

	QPLDLL std::string mathematical_operation_string(mathematical_operation op);
	enum class mathematical_prefix {
		none, positive, negated
	};


	template<typename T>
	struct mathematical_functon {
		struct number_operation {

			constexpr mathematical_operation get_operation() const {
				return static_cast<mathematical_operation>(this->info[{0, 3}].get());
			}
			constexpr void set_operation(mathematical_operation operation) {
				this->info[{0, 3}] = qpl::u64_cast(operation);
			}
			constexpr mathematical_prefix get_prefix() const {
				return static_cast<mathematical_prefix>(this->info[{4, 5}].get());
			}
			constexpr void set_prefix(mathematical_prefix prefix) {
				this->info[{4, 5}] = qpl::u64_cast(prefix);
			}
			constexpr bool is_link() const {
				return this->info[6];
			}
			constexpr qpl::u32 get_link() const {
				return qpl::u32_cast(this->number);
			}
			constexpr void set_link(qpl::u32 link) {
				this->info[6] = true;
				this->number = qpl::f64_cast(link);
			}
			constexpr bool is_variable() const {
				return this->info[7];
			}
			constexpr qpl::u32 get_variable() const {
				return qpl::u32_cast(this->number);
			}
			constexpr void set_variable(qpl::u32 variable_index) {
				this->info[7] = true;
				this->number = qpl::f64_cast(variable_index);
			}
			constexpr void clear() {
				this->info.clear();
				this->number = 0u;
			}

			std::string string() const {
				std::ostringstream stream;
				if (this->get_prefix() == mathematical_prefix::negated) {
					stream << "-";
				}
				else if (this->get_prefix() == mathematical_prefix::positive) {
					stream << "+";
				}

				if (this->is_link()) {
					stream << qpl::to_string("#", this->get_link());
				}
				else if (this->is_variable()) {
					stream << qpl::to_string("v", this->get_variable());
				}
				else {
					stream << this->number;
				}

				if (this->get_operation() != mathematical_operation::none) {
					stream << " " << mathematical_operation_string(this->get_operation());
				}
				return stream.str();
			}

			T number;
			qpl::bitset<32> info;
		};
		std::vector<number_operation> chain;
		std::vector<number_operation> result;
		
		template <class C>
		constexpr T solve(const C& variables) {
			this->result = this->chain;

			for (qpl::u32 i = 0u; i < this->result.size() - 1; ++i) {
				for (qpl::u32 c = i; c < this->result.size() - 1; ++c) {
					auto op = this->result[c].get_operation();
					auto is_c_non = (this->result[c + 1].get_operation() == mathematical_operation::none);

					if (i == c) {
						if (this->result[i].is_link()) {
							this->result[i].number = this->result[this->result[i].get_link()].number;
						}
						else if (this->result[i].is_variable()) {
							this->result[i].number = variables[this->result[i].get_variable()];
						}
						auto prefix = this->result[i].get_prefix();
						if (prefix == qpl::mathematical_prefix::negated) {
							this->result[i].number = -this->result[i].number;
						}
						else if (prefix == qpl::mathematical_prefix::positive) {
							this->result[i].number = +this->result[i].number;
						}
						//qpl::print(this->result[i].number, " ", mathematical_operation_string(op), " ");
					}
					

					//qpl::print(this->result[c + 1].string(), " ");


					T right = this->result[c + 1].number;
					auto prefix = this->result[c + 1].get_prefix();
					if (this->result[c + 1].is_link()) {
						right = this->result[this->result[c + 1].get_link()].number;
					}
					else if (this->result[c + 1].is_variable()) {
						right = variables[this->result[c + 1].get_variable()];
					}
					if (prefix == qpl::mathematical_prefix::negated) {
						right = -right;
					}
					else if (prefix == qpl::mathematical_prefix::positive) {
						right = +right;
					}
					switch (op) {
					case mathematical_operation::add:
						this->result[i].number += right; break;
					case mathematical_operation::sub:
						this->result[i].number -= right; break;
					case mathematical_operation::mul:
						this->result[i].number *= right; break;
					case mathematical_operation::div:
						if (right == 0) this->result[i].number = 0;
						else this->result[i].number /= right;
						break;
					case mathematical_operation::pow:
						this->result[i].number = std::pow(this->result[i].number, right); break;
					}

					if (is_c_non) {
						//qpl::println("=> ", this->result[i].number);
						if (c == this->result.size() - 2) {
							return this->result[i].number;
						}
						i = c + 1;
						break;
					}
				}
			}
		}

		template<typename... Args>
		constexpr T operator()(Args... variables) {
			return this->solve(qpl::to_array<T>(variables...));
		}

		std::string string() const {
			std::ostringstream stream;
			for (auto& i : this->chain) {
				stream << i.string() << " ";
				if (i.get_operation() == qpl::mathematical_operation::none) {
					stream << '\n';
				}
			}
			return stream.str();
		}
	};

}

#endif