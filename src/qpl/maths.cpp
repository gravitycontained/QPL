#include <qpl/maths.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/string.hpp>

namespace qpl {
	
	void qpl::exponential_moving_average::reset() {
		this->last_ma = 0.0;
		this->time_ctr = 0;
	}
	void qpl::exponential_moving_average::add(qpl::f64 value) {
		++this->time_ctr;
		this->time_ctr = qpl::min(static_cast<qpl::size>(this->time_period), this->time_ctr);

		auto m = 2.0 / (1.0 + this->time_ctr);
		this->last_ma = m * value + (1 - m) * this->last_ma;
	}
	qpl::f64 qpl::exponential_moving_average::get_average() const {
		return this->last_ma;
	}

	void qpl::moving_average::reset() {
		this->array.clear();
	}
	void qpl::moving_average::add(qpl::f64 value) {
		this->array.add(value);
	}
	qpl::f64 qpl::moving_average::get_average() const {
		return this->array.get_average();
	}
	qpl::size qpl::moving_average::time_period() const {
		return this->array.size();
	}
	void qpl::moving_average::set_time_period(qpl::size time_period) {
		this->array.resize(time_period);
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