#include <qpl/maths.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>
#include <qpl/ranges.hpp>

namespace qpl {
	std::vector<std::vector<qpl::u32>> sudoku_sum_possibilities(qpl::size sum, qpl::size numbers) {
		std::vector<std::vector<qpl::u32>> result;

		auto arrangements = qpl::list_all_arrangements(qpl::vector_0_to_n(9u, 1u), numbers);

		for (auto& i : arrangements) {
			if (qpl::container_sum(i) == sum) {
				result.push_back(i);
			}
		}
		
		return result;
	}

	void qpl::ema::reset() {
		this->last_ma = 0.0;
		this->time_ctr = 0;
	}
	void qpl::ema::add_value(double value) {
		++this->time_ctr;
		this->time_ctr = qpl::min(static_cast<qpl::size>(this->time_period), this->time_ctr);

		double m = 2.0 / (1.0 + this->time_ctr);
		this->last_ma = m * value + (1 - m) * this->last_ma;
	}

	double qpl::ema::get_average() const {
		return this->last_ma;
	}
	std::string qpl::mathematical_operation_string(mathematical_operation op) {
		switch (op) {
		case mathematical_operation::add:
			return "+";
		case mathematical_operation::sub:
			return "-";
		case mathematical_operation::mul:
			return "*";
		case mathematical_operation::div:
			return "/";
		case mathematical_operation::pow:
			return "^";
		}
		return "";
	}
}