#include <qpl/smooth.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/random.hpp>

namespace qpl {
	std::array<double, 4> qpl::cubic_generator::get_values() const {
		std::array<double, 4> result;
		result[0] = this->m_a;
		result[1] = this->m_b;
		result[2] = this->m_c;
		result[3] = this->m_d;
		return result;
	}
	void qpl::cubic_generator::set_random_range(double min, double max) {
		this->m_min = min;
		this->m_max = max;
		this->gen();
	}
	void qpl::cubic_generator::set_random_range(double max) {
		this->set_random_range(0.0, max);
	}
	std::pair<double, double> qpl::cubic_generator::get_random_range() const {
		return std::make_pair(this->m_min, this->m_max);
	}
	double qpl::cubic_generator::get() const {
		double interpolated;
		if (this->is_cubic_interpolation_enabled()) {
			interpolated = qpl::cubic_interpolation(this->m_a, this->m_b, this->m_c, this->m_d, this->m_prog);
		}
		else {
			interpolated = qpl::linear_interpolation(this->m_b, this->m_c, this->m_prog);
		}
		if (this->m_clamp) {
			return qpl::clamp(this->m_min, interpolated, this->m_max);
		}
		return interpolated;
	}
	double qpl::cubic_generator::update(double delta) {
		this->m_prog += delta * this->m_speed;
		while (this->m_prog > 1.0) {
			this->m_prog -= 1;
			this->next();
		}
		while (this->m_prog < 0.0) {
			this->m_prog += 1;
			this->back();
		}
		return this->get();
	}
	double qpl::cubic_generator::get_progress() const {
		return this->m_prog;
	}
	void qpl::cubic_generator::reset_progress() {
		this->m_prog = 0.0;
	}
	double qpl::cubic_generator::get_approaching_value() const {
		if (this->m_random_values_enabled) {
			return this->m_c;
		}
		return this->m_specific_values[qpl::loop_index((this->m_specific_index + 1), this->m_specific_values.size())];
	}
	void qpl::cubic_generator::next() {
		this->m_a = this->m_b;
		this->m_b = this->m_c;
		this->m_c = this->m_d;
		if (this->m_random_values_enabled) {
			this->m_d = qpl::random(this->m_min, this->m_max);
		}
		else {
			++this->m_specific_index;
			this->m_specific_index = qpl::loop_index(this->m_specific_index, this->m_specific_values.size());
			this->m_d = this->m_specific_values[qpl::loop_index(this->m_specific_index + 2, this->m_specific_values.size())];
		}
	}
	void qpl::cubic_generator::back() {
		this->m_d = this->m_c;
		this->m_c = this->m_b;
		this->m_b = this->m_a;
		if (this->m_random_values_enabled) {
			this->m_d = qpl::random(this->m_min, this->m_max);
		}
		else {
			--this->m_specific_index;
			this->m_specific_index = qpl::loop_index(this->m_specific_index, this->m_specific_values.size());
			this->m_a = this->m_specific_values[qpl::loop_index(this->m_specific_index - 1, this->m_specific_values.size())];
		}
	}
	void qpl::cubic_generator::randomize_and_reset(bool enable_random_values) {
		this->reset_progress();
		this->randomize(enable_random_values);
	}
	void qpl::cubic_generator::randomize(bool enable_random_values) {
		if (enable_random_values) {
			this->enable_random_values();
		}
		this->m_a = qpl::random(this->m_min, this->m_max);
		this->m_b = qpl::random(this->m_min, this->m_max);
		this->m_c = qpl::random(this->m_min, this->m_max);
		this->m_d = qpl::random(this->m_min, this->m_max);
	}
	void qpl::cubic_generator::set_speed(double speed) {
		this->m_speed = speed;
	}
	double qpl::cubic_generator::get_speed() const {
		return this->m_speed;
	}

	void qpl::cubic_generator::enable_clamp() {
		this->m_clamp = true;
	}
	void qpl::cubic_generator::disable_clamp() {
		this->m_clamp = false;
	}
	bool qpl::cubic_generator::is_clamp_enabled() const {
		return this->m_clamp;
	}
	void qpl::cubic_generator::enable_cubic_interpolation() {
		this->m_cubic_interpolation = true;
	}
	void qpl::cubic_generator::enable_linear_interpolation() {
		this->m_cubic_interpolation = false;
	}
	bool qpl::cubic_generator::is_cubic_interpolation_enabled() const {
		return this->m_cubic_interpolation;
	}
	bool qpl::cubic_generator::is_linear_interpolation_enabled() const {
		return !this->m_cubic_interpolation;
	}

	void qpl::cubic_generator::enable_random_values() {
		this->m_random_values_enabled = true;
	}
	void qpl::cubic_generator::disable_random_values() {
		this->m_random_values_enabled = false;
	}
	bool qpl::cubic_generator::is_random_values_enabled() const {
		return this->m_random_values_enabled;
	}

	void qpl::cubic_generator::enable_specific_values() {
		this->disable_random_values();
	}
	void qpl::cubic_generator::disable_spefific_values() {
		this->enable_random_values();
	}
	bool qpl::cubic_generator::is_specific_values_enabled() const {
		return !this->is_random_values_enabled();
	}

	void qpl::cubic_generator::set_specific_values(const std::vector<double>& values, bool enable_specific_values) {
		this->m_specific_values = values;
		if (enable_specific_values) {
			this->enable_specific_values();
		}
		this->update(values.size());
	}
	void qpl::cubic_generator::fill(double initial_value, bool enable_specific_values) {
		this->set_specific_values({ initial_value, initial_value, initial_value, initial_value }, enable_specific_values);
	}
	void qpl::cubic_generator::set_next(double value) {
		this->m_specific_values[qpl::loop_index(this->m_specific_index + 1, this->m_specific_values.size())] = value;
	}
	std::vector<double> qpl::cubic_generator::get_specific_values() const {
		return this->m_specific_values;
	}

	void qpl::cubic_generator::reset_specific_index() {
		this->m_specific_index = 0u;
	}
	void qpl::cubic_generator::set_specific_index(int n) {
		this->m_specific_index = n;
	}
	int qpl::cubic_generator::get_specific_index() const {
		return this->m_specific_index;
	}

	std::size_t qpl::cubic_generator::size() const {
		return this->m_specific_values.size();
	}
	double& qpl::cubic_generator::operator[](unsigned index) {
		return this->m_specific_values[index];
	}
	const double& qpl::cubic_generator::operator[](unsigned index) const {
		return this->m_specific_values[index];
	}

	void qpl::cubic_generator::gen() {
		if (this->m_random_values_enabled) {
			this->randomize();
		}
		else {
			if (this->m_specific_values.empty()) {
				return;
			}
			this->m_a = this->m_specific_values[qpl::loop_index(this->m_specific_index - 1, this->m_specific_values.size())];
			this->m_b = this->m_specific_values[qpl::loop_index(this->m_specific_index, this->m_specific_values.size())];
			this->m_c = this->m_specific_values[qpl::loop_index(this->m_specific_index + 1, this->m_specific_values.size())];
			this->m_d = this->m_specific_values[qpl::loop_index(this->m_specific_index + 2, this->m_specific_values.size())];
		}
	}
	void qpl::cubic_generator::init() {
		this->reset_specific_index();
		this->enable_random_values();
		this->disable_clamp();
		this->gen();
		this->set_speed(1.0);
		this->enable_cubic_interpolation();
	}
}