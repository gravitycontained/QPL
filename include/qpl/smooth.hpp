#ifndef QPL_SMOOTH_HPP
#define QPL_SMOOTH_CPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/time.hpp>
#include <qpl/random.hpp>
#include <qpl/vector.hpp>
#include <vector>
#include <array>
#include <memory>
#include <functional>

namespace qpl {
	template<typename T> requires (qpl::is_floating_point<T>())
	class cubic_generator_t {
	public:
		cubic_generator_t(T min = 0, T max = 1) {
			this->set_random_range(min, max);
			this->init();
		}
		
		std::array<T, 4> get_values() const {
			std::array<T, 4> result;
			result[0] = this->m_a;
			result[1] = this->m_b;
			result[2] = this->m_c;
			result[3] = this->m_d;
			return result;
		}
		void set_random_range(T min, T max) {
			this->m_min = min;
			this->m_max = max;
			this->randomize();
		}
		void set_random_range(T max) {
			this->set_random_range(0.0, max);
		}
		std::pair<T, T> get_random_range() const {
			return std::make_pair(this->m_min, this->m_max);
		}
		T get() const {
			T interpolated = qpl::cubic_interpolation(this->m_a, this->m_b, this->m_c, this->m_d, this->m_prog);
			if (this->m_clamp) {
				return qpl::clamp(this->m_min, interpolated, this->m_max);
			}
			return interpolated;
		}
		T update(double delta) {
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
		double get_progress() const {
			return this->m_prog;
		}
		void reset_progress() {
			this->m_prog = 0.0;
		}
		T get_approaching_value() const {
			if (this->m_random_values_enabled) {
				return this->m_c;
			}
			return this->m_specific_values[qpl::loop_index((this->m_specific_index + 1), this->m_specific_values.size())];
		}
		void next() {
			this->m_a = this->m_b;
			this->m_b = this->m_c;
			this->m_c = this->m_d;
			this->m_d = qpl::random(this->m_min, this->m_max);
		}
		void back() {
			this->m_d = this->m_c;
			this->m_c = this->m_b;
			this->m_b = this->m_a;
			this->m_d = qpl::random(this->m_min, this->m_max);
		}
		void randomize_and_reset() {
			this->reset_progress();
			this->randomize();
		}
		void randomize() {
			this->m_a = qpl::random(this->m_min, this->m_max);
			this->m_b = qpl::random(this->m_min, this->m_max);
			this->m_c = qpl::random(this->m_min, this->m_max);
			this->m_d = qpl::random(this->m_min, this->m_max);
		}
		void set_speed(qpl::f64 speed) {
			this->m_speed = speed;
		}
		qpl::f64 get_speed() const {
			return this->m_speed;
		}

		void enable_clamp() {
			this->m_clamp = true;
		}
		void disable_clamp() {
			this->m_clamp = false;
		}
		bool is_clamp_enabled() const {
			return this->m_clamp;
		}

	
		void init() {
			this->m_prog = 0.0;
			this->disable_clamp();
			this->randomize();
			this->set_speed(1.0);
		}

		qpl::u32 m_specific_index;
		bool m_clamp;
		qpl::f64 m_prog;
		T m_a, m_b, m_c, m_d;
		T m_min, m_max;
		qpl::f64 m_speed;
	};

	using cubic_generator = cubic_generator_t<qpl::f64>;



	template<typename T, qpl::size N> requires (qpl::is_floating_point<T>())
	struct cubic_generator_vectorN {
		using vectorT = qpl::vectorN<T, N>;


		cubic_generator_vectorN(vectorT min = vectorT::zero(), vectorT max = vectorT::one()) {
			this->set_random_range(min, max);
			this->init();
		}

		std::array<vectorT, 4> get_values() const {
			std::array<vectorT, 4> result;
			result[0] = this->m_a;
			result[1] = this->m_b;
			result[2] = this->m_c;
			result[3] = this->m_d;
			return result;
		}
		void set_random_range(vectorT min, vectorT max) {
			this->m_min = min;
			this->m_max = max;
			this->randomize();
		}
		void set_random_range(T min, T max) {
			this->m_min = vectorT::values(min);
			this->m_max = vectorT::values(max);
			this->randomize();
		}
		void set_random_range(T max) {
			this->set_random_range(0.0, max);
		}
		void set_speed(qpl::f64 speed) {
			this->m_speed = speed;
		}
		qpl::f64 get_speed() const {
			return this->m_speed;
		}

		double get_progress() const {
			return this->m_prog;
		}
		void reset_progress() {
			this->m_prog = 0.0;
		}
		vectorT get_approaching_value() const {
			return this->m_c;
		}

		void enable_clamp() {
			this->m_clamp = true;
		}
		void disable_clamp() {
			this->m_clamp = false;
		}
		bool is_clamp_enabled() const {
			return this->m_clamp;
		}


		void randomize_and_reset() {
			this->reset_progress();
			this->randomize();
		}

		void randomize() {
			for (qpl::u32 i = 0u; i < N; ++i) {
				this->m_a.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
				this->m_b.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
				this->m_c.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
				this->m_d.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
			}
		}
		vectorT get() const {
			vectorT interpolated = qpl::cubic_interpolation(this->m_a, this->m_b, this->m_c, this->m_d, this->m_prog);
			if (this->m_clamp) {
				return qpl::clamp(this->m_min, interpolated, this->m_max);
			}
			return interpolated;
		}
		vectorT update(double delta) {
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

		void next() {
			this->m_a = this->m_b;
			this->m_b = this->m_c;
			this->m_c = this->m_d;
			for (qpl::u32 i = 0u; i < N; ++i) {
				this->m_d.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
			}
		}
		void back() {
			this->m_d = this->m_c;
			this->m_c = this->m_b;
			this->m_b = this->m_a;
			for (qpl::u32 i = 0u; i < N; ++i) {
				this->m_d.data[i] = qpl::random(this->m_min.data[i], this->m_max.data[i]);
			}
		}
	
		void init() {
			this->m_prog = 0.0;
			this->disable_clamp();
			this->randomize();
			this->set_speed(1.0);
		}

		vectorT m_a, m_b, m_c, m_d;
		vectorT m_min, m_max;
		qpl::f64 m_prog = 0;
		qpl::f64 m_speed = 1;
		bool m_clamp;
	};

	template<typename T>
	using cubic_generator_vector2 = cubic_generator_vectorN<T, 2>;
	using cubic_generator_vector2f = cubic_generator_vector2<qpl::f32>;
	using cubic_generator_vector2d = cubic_generator_vector2<qpl::f64>;

	template<typename T>
	using cubic_generator_vector3 = cubic_generator_vectorN<T, 3>;
	using cubic_generator_vector3f = cubic_generator_vector3<qpl::f32>;
	using cubic_generator_vector3d = cubic_generator_vector3<qpl::f64>;
	

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
				*this->pointer = this->start_value + (this->end_value - this->start_value) * qpl::smooth_slope(this->clock.elapsed().nsecs_f() / this->duration.nsecs_f());
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
}

#endif