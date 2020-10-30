#ifndef QPL_SMOOTH_HPP
#define QPL_SMOOTH_CPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <vector>
#include <array>
#include <memory>
#include <qpl/time.hpp>
#include <functional>

namespace qpl {
	class cubic_generator {
	public:
		cubic_generator(double min = 0, double max = 100) {
			this->set_random_range(min, max);
			this->init();
		}
		QPLDLL std::array<double, 4> get_values() const;
		QPLDLL void set_random_range(double min, double max);
		QPLDLL void set_random_range(double max);
		QPLDLL std::pair<double, double> get_random_range() const;
		QPLDLL double get() const;

		QPLDLL double update(double delta);
		QPLDLL double get_progress() const;
		QPLDLL void reset_progress();
		QPLDLL double get_approaching_value() const;
		QPLDLL void next();
		QPLDLL void back();
		QPLDLL void randomize_and_reset(bool enable_random_values = true);
		QPLDLL void randomize(bool enable_random_values = true);

		QPLDLL void set_speed(double speed);
		QPLDLL double get_speed() const;

		QPLDLL void enable_clamp();
		QPLDLL void disable_clamp();
		QPLDLL bool is_clamp_enabled() const;

		QPLDLL void enable_cubic_interpolation();
		QPLDLL void enable_linear_interpolation();
		QPLDLL bool is_cubic_interpolation_enabled() const;
		QPLDLL bool is_linear_interpolation_enabled() const;

		QPLDLL void enable_random_values();
		QPLDLL void disable_random_values();
		QPLDLL bool is_random_values_enabled() const;

		QPLDLL void enable_specific_values();
		QPLDLL void disable_spefific_values();
		QPLDLL bool is_specific_values_enabled() const;

		QPLDLL void set_specific_values(const std::vector<double>& values, bool enable_specific_values = true);
		QPLDLL void fill(double initial_value, bool enable_specific_values = true);
		QPLDLL void set_next(double value);
		QPLDLL std::vector<double> get_specific_values() const;

		QPLDLL void reset_specific_index();
		QPLDLL void set_specific_index(int n);
		QPLDLL int get_specific_index() const;

		QPLDLL std::size_t size() const;
		QPLDLL double& operator[](unsigned index);
		QPLDLL const double& operator[](unsigned index) const;
	private:
		QPLDLL void gen();
		QPLDLL void init();

		bool m_cubic_interpolation;
		int m_specific_index;
		std::vector<double> m_specific_values;
		bool m_random_values_enabled;
		bool m_clamp;
		double m_min, m_max;
		double m_prog;
		double m_a, m_b, m_c, m_d;
		double m_speed;
	};

	template<typename T>
	struct smooth_value {
		void start(T& value, T end, qpl::time duration = qpl::secs(1)) {
			if (this->is_running()) {
				this->clock.reset();
			}
			else {
				this->clock.resume();
			}
			this->pointer = &value;
			this->start_value = value;
			this->end_value = end;
			this->duration = duration;
		}
		void update() {
			if (!this->is_running()) {
				return;
			}

			if (this->clock.has_elapsed(this->duration)) {
				this->clock.reset_pause();
				*this->pointer = this->end_value;
			}
			else {
				*this->pointer = this->start_value + (this->end_value - this->start_value) * qpl::smooth_progression(this->clock.elapsed().nsecs_f() / this->duration.nsecs_f());
			}
		}
		bool is_running() const {
			return this->clock.is_running();
		}

		qpl::time duration = qpl::secs(1);
		qpl::halted_clock clock;

		T start_value;
		T end_value;
		T* pointer;
	};


	template<typename C, typename T>
	struct smooth_value_setter {

		void start(T start, std::function<void(C*, T)> set_function, T end, qpl::time duration = qpl::secs(1)) {
			if (this->is_running()) {
				this->clock.reset();
			}
			else {
				this->clock.resume();
			}
			this->set_function = set_function;
			this->start_value = start;
			this->end_value = end;
			this->duration = duration;
		}

		void update() {
			if (!this->is_running()) {
				return;
			}

			if (this->clock.has_elapsed(this->duration)) {
				this->clock.reset_pause();
				this->set_function(this->end_value);
			}
			else {
				this->set_function(this->start_value + (this->end_value - this->start_value) * qpl::smooth_progression(this->clock.elapsed().nsecs_f() / this->duration.nsecs_f()));
			}

		}
		bool is_running() const {
			return this->clock.is_running();
		}

		qpl::time duration = qpl::secs(1);
		qpl::halted_clock clock;

		std::function<void(C*, T)> set_function;
		T start_value;
		T end_value;
		T* pointer;
	};
}

#endif