#ifndef QPL_TIME_HPP
#define QPL_TIME_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/maths.hpp>
#include <qpl/vardef.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <chrono>


namespace qpl {
	class time {
	public:
		constexpr time(qpl::u64 ns = qpl::u64{}) noexcept : m_ns(ns) {

		}
		QPLDLL time& operator=(const qpl::time& other);

		QPLDLL time& operator+=(const qpl::time& other);
		QPLDLL time operator+(const qpl::time& other) const;

		QPLDLL time& operator-=(const qpl::time& other);
		QPLDLL time operator-(const qpl::time& other) const;

		QPLDLL time& operator/=(const qpl::time& other);
		QPLDLL time operator/(const qpl::time& other) const;

		QPLDLL time& operator%=(const qpl::time& other);
		QPLDLL time operator%(const qpl::time& other) const;

		QPLDLL bool operator==(const qpl::time& other) const;
		QPLDLL bool operator!=(const qpl::time& other) const;
		QPLDLL bool operator<(const qpl::time& other) const;
		QPLDLL bool operator<=(const qpl::time& other) const;
		QPLDLL bool operator>(const qpl::time& other) const;
		QPLDLL bool operator>=(const qpl::time& other) const;

		QPLDLL std::string string_non_zero() const;
		QPLDLL std::string string() const;
		QPLDLL std::string string_until_sec() const;
		QPLDLL std::string string_until_ms() const;
		QPLDLL std::string string_full() const;
		QPLDLL static time clock_time();
		QPLDLL qpl::f64 frequency() const;

		QPLDLL qpl::u64 nsecs() const;
		QPLDLL qpl::u64 usecs() const;
		QPLDLL qpl::u64 msecs() const;
		QPLDLL qpl::u64 secs() const;
		QPLDLL qpl::u64 mins() const;
		QPLDLL qpl::u64 hours() const;
		QPLDLL qpl::u64 days() const;
		QPLDLL qpl::u64 years() const;

		QPLDLL qpl::u64 nsecs_mod() const;
		QPLDLL qpl::u64 usecs_mod() const;
		QPLDLL qpl::u64 msecs_mod() const;
		QPLDLL qpl::u64 secs_mod() const;
		QPLDLL qpl::u64 mins_mod() const;
		QPLDLL qpl::u64 hours_mod() const;
		QPLDLL qpl::u64 days_mod() const;
		QPLDLL qpl::u64 years_mod() const;

		QPLDLL qpl::f64 nsecs_f() const;
		QPLDLL qpl::f64 usecs_f() const;
		QPLDLL qpl::f64 msecs_f() const;
		QPLDLL qpl::f64 secs_f() const;
		QPLDLL qpl::f64 mins_f() const;
		QPLDLL qpl::f64 hours_f() const;
		QPLDLL qpl::f64 days_f() const;
		QPLDLL qpl::f64 years_f() const;

		constexpr static qpl::u64 nsecs_in_usec = qpl::u64{ 1000ull };
		constexpr static qpl::u64 usecs_in_msec = qpl::u64{ 1000ull };
		constexpr static qpl::u64 msecs_in_sec = qpl::u64{ 1000ull };
		constexpr static qpl::u64 secs_in_min  = qpl::u64{ 60ull };
		constexpr static qpl::u64 mins_in_hour  = qpl::u64{ 60ull };
		constexpr static qpl::u64 hours_in_day = qpl::u64{ 24ull };
		constexpr static qpl::u64 days_in_year = qpl::u64{ 365ull };

		constexpr static qpl::u64 nsecs_in_nsec = qpl::u64{ 1ull };
		constexpr static qpl::u64 nsecs_in_msec = usecs_in_msec * nsecs_in_usec;
		constexpr static qpl::u64 nsecs_in_sec  = msecs_in_sec * nsecs_in_msec;
		constexpr static qpl::u64 nsecs_in_min  = secs_in_min * nsecs_in_sec;
		constexpr static qpl::u64 nsecs_in_hour = mins_in_hour * nsecs_in_min;
		constexpr static qpl::u64 nsecs_in_day  = hours_in_day * nsecs_in_hour;
		constexpr static qpl::u64 nsecs_in_year = days_in_year * nsecs_in_day;
	private:
		QPLDLL void set(qpl::u64 ns);

		qpl::u64 m_ns;
	};

	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::time& time);

	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time nsecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time usecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_usec) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time msecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_msec) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time secs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_sec) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time mins(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_min) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time hours(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_hour) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time days(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_day) };
	}
	template<typename T> requires(qpl::is_arithmetic<T>())
	constexpr qpl::time years(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_year) };
	}

	class clock {
	public:
		clock(bool running = true) {
			this->init(running);
		}
		QPLDLL void reset();
		QPLDLL void pause();
		QPLDLL void reset_pause();
		QPLDLL void resume();
		QPLDLL bool is_running() const;
		QPLDLL bool is_paused() const;

		QPLDLL qpl::time elapsed() const;
		QPLDLL std::string elapsed_str() const;
		QPLDLL qpl::f64 elapsed_f() const;

		QPLDLL qpl::time elapsed_reset();
		QPLDLL std::string elapsed_str_reset();
		QPLDLL qpl::f64 elapsed_f_reset();

		QPLDLL void add(qpl::f64 seconds);
		QPLDLL void subtract(qpl::f64 seconds);

		QPLDLL bool has_elapsed(qpl::f64 seconds) const;
		QPLDLL bool has_elapsed(qpl::time duration) const;
		QPLDLL bool has_elapsed_reset(qpl::f64 seconds);
		QPLDLL bool has_elapsed_reset(qpl::time duration);
	private:
		QPLDLL void measure() const;
		QPLDLL void init(bool running);

		qpl::time m_pause;
		qpl::time m_pause_sum;
		qpl::time m_begin;
		mutable qpl::time m_end;
		bool m_running;
	};
	class halted_clock : public qpl::clock {
	public:
		halted_clock() : qpl::clock(false) {
		}
	};



	class small_clock {
	public:
		small_clock() {
			this->reset();
		}
		QPLDLL void reset();

		QPLDLL qpl::time elapsed() const;
		QPLDLL std::string elapsed_str() const;
		QPLDLL qpl::f64 elapsed_f() const;

		QPLDLL qpl::time elapsed_reset();
		QPLDLL std::string elapsed_str_reset();
		QPLDLL qpl::f64 elapsed_f_reset();

		QPLDLL void add(qpl::f64 seconds);
		QPLDLL void subtract(qpl::f64 seconds);

		QPLDLL bool has_elapsed(qpl::f64 seconds) const;
		QPLDLL bool has_elapsed(qpl::time duration) const;
		QPLDLL bool has_elapsed_reset(qpl::f64 seconds);
		QPLDLL bool has_elapsed_reset(qpl::time duration);
	private:

		qpl::time m_begin;
	};


	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::clock& clock);
	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::halted_clock& clock);


	struct fps_counter {
		fps_counter() {
			this->reset();
		}

		void set_time_period(qpl::u32 time_period) {
			this->ema.time_period = time_period;
		}
		qpl::u32 get_time_period() const {
			return qpl::u32_cast(this->ema.time_period);
		}

		void reset() {
			this->ema.reset();
			this->ema.time_period = 100u;
			this->clock.reset();
			this->start = false;
		}

		void measure() {
			if (!this->start) {
				this->start = true;
				this->clock.reset();
			}
			else {
				this->ema.add_value(this->clock.elapsed_f_reset());
			}
		}

		qpl::u32 get_fps_u32() const {
			if (!this->ema.get_average()) {
				return 0u;
			}
			return qpl::u32_cast(1.0 / this->ema.get_average());
		}
		qpl::f64 get_fps() const {
			if (!this->ema.get_average()) {
				return 0.0;
			}
			return 1.0 / this->ema.get_average();
		}

		bool start;
		qpl::clock clock;
		qpl::exponential_moving_average ema;
	};

	QPLDLL void wait(qpl::time duration);
	QPLDLL void wait(double seconds);

	template <typename T>
	constexpr std::time_t to_time_t(T t) {
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(t - T::clock::now()
			+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}
	template<typename T>
	std::string get_time_string(T duration, std::string format = "%Y-%m-%d-%H-%M-%S") {
		auto time = qpl::to_time_t(duration);
		
#pragma warning( push )
#pragma warning( disable : 4996)
		std::tm* gmt = std::localtime(&time);
#pragma warning( pop ) 

		std::ostringstream buffer;
		buffer << std::put_time(gmt, format.c_str());
		return buffer.str();
	}
	//YYYY-MM-DD-HH-MM-SS
	QPLDLL std::string get_current_time_string();
	QPLDLL std::string get_current_time_string_ms();
	QPLDLL std::string get_current_time_string_ymd_hms();
	QPLDLL std::string get_current_time_string_ymd_hmsms();
	QPLDLL std::string get_current_time_string_ymd_hmsms_compact();
	QPLDLL std::string unix_to_date(qpl::u32 unix);

	QPLDLL qpl::time get_remaining_time(qpl::f64 progress, qpl::time elapsed);
	QPLDLL qpl::time get_remaining_time(qpl::f64 progress, qpl::clock timer);

	template<typename T, typename U> requires(qpl::is_integer<T>() && qpl::is_integer<U>())
	qpl::time get_remaining_time(T start, U end, qpl::time elapsed) {
		return qpl::get_remaining_time(qpl::f64_cast(start) / end, elapsed);
	}
	template<typename T, typename U> requires(qpl::is_integer<T>() && qpl::is_integer<U>())
	qpl::time get_remaining_time(T start, U end, qpl::clock timer) {
		return qpl::get_remaining_time(qpl::f64_cast(start) / end, timer.elapsed());
	}

	namespace detail {
		QPLDLL extern std::unordered_map<std::string, std::unordered_map<std::string, qpl::halted_clock>> sub_benchmark_clocks;
		QPLDLL extern std::unordered_map<std::string, qpl::halted_clock> benchmark_clocks;
		QPLDLL extern std::string last_benchmark_name;
		QPLDLL extern std::string last_sub_benchmark_name;

		QPLDLL extern clock signal_clock;
		QPLDLL extern qpl::u64 signal_count;
	}


	QPLDLL void begin_benchmark_end_previous(const std::string& name);
	QPLDLL void begin_benchmark_end_previous(const std::string& sub, const std::string& name);
	QPLDLL qpl::halted_clock get_benchmark(const std::string& name = "");
	QPLDLL void begin_benchmark(const std::string& name = "");
	QPLDLL void begin_benchmark(const std::string& sub, const std::string& name);
	QPLDLL void end_benchmark();
	QPLDLL void end_benchmark(const std::string& name);


	QPLDLL void begin_benchmark_segments();
	QPLDLL void print_benchmark();
	QPLDLL void print_benchmark(const std::string& name);

	QPLDLL void reset_time_signal();
	QPLDLL bool get_time_signal(double seconds);
	QPLDLL bool get_time_signal(qpl::time time);

	QPLDLL void reset_count_signal();
	QPLDLL bool get_count_signal(qpl::u64 when);

	struct animation {
		qpl::small_clock timer;
		qpl::f64 duration = 1.0;
		qpl::f64 reverse_progress_state = 0.0;
		qpl::small_clock wait_timer;
		qpl::f64 wait_duration = 0.0;
		bool wait_reverse = false;
		qpl::f64 progress_value = 0.0;
		bool running = false;
		bool just_finish = false;
		bool started = false;
		bool reversed = false;

		animation(qpl::f64 duration = 1.0) {
			this->duration = duration;
		}
		void set_duration(qpl::f64 duration) {
			this->duration = duration;
		}
		void stop() {
			this->started = false;
		}
		void reverse(bool running = true) {
			if (running) {
				if (this->reversed) {
					if (this->is_running()) {
						auto progress = this->get_progress();
						this->reverse_progress_state = 0.0;
						this->timer.reset();
						this->timer.subtract(progress * this->duration);
						this->reversed = false;
					}
					else {
						this->reset(running);
						this->reverse_progress_state = 0.0;
						this->reversed = false;
					}
				}
				else {
					if (this->is_running()) {
						this->reverse_progress_state = this->get_progress() * 2;
					}
					else {
						this->reset(running);
						this->reverse_progress_state = 1.0;
					}
					this->progress_value = 1.0;
					this->reversed = true;
				}
			}
			else {
				this->reset();
				this->reverse_progress_state = 0.0;
				this->reversed = true;
			}
		}
		void reset(bool running = false) {
			this->timer.reset();
			this->running = running;
			this->started = true;
			this->just_finish = false;
			this->reversed = false;
			this->progress_value = 0.0;
			this->wait_duration = 0.0;
			this->wait_reverse = false;
		}
		void start() {
			this->reset(true);
		}
		void start_and_reverse() {
			this->reverse();
		}
		void reset_and_reverse() {
			this->reset();
			this->reverse();
		}
		void reset_and_start() {
			this->start();
		}
		void start_in(qpl::f64 time) {
			if (!time) {
				this->start();
				return;
			}
			this->started = true;
			this->wait_duration = time;
			this->wait_timer.reset();
			this->wait_reverse = false;
		}
		void reverse_in(qpl::f64 time) {
			if (!time) {
				this->reverse();
				return;
			}
			
			this->started = true;
			this->wait_duration = time;
			this->wait_timer.reset();
			this->wait_reverse = true;
		}
		void update() {
			if (this->wait_duration) {
				if (this->wait_timer.has_elapsed(this->wait_duration)) {
					if (this->wait_reverse) {
						this->reverse();
					}
					else {
						this->start();
					}
					this->wait_duration = 0.0;
				}
				return;
			}			
			this->just_finish = false;
			if (!this->running) {
				return;
			}
			if (!this->started) {
				this->running = false;
				this->progress_value = 0.0;
				return;
			}
			auto before = this->running;
			this->progress_value = (this->timer.elapsed_f()) / this->duration;
			if (this->reversed) {
				this->progress_value = this->reverse_progress_state - this->progress_value;
			}
			this->running = (this->progress_value >= 0.0 && this->progress_value <= 1.0);
			if (before && !this->running) {
				this->just_finish = true;
			}
		}

		bool is_reversed() const {
			return this->reversed;
		}
		bool is_running() const {
			return this->running || this->just_finish;
		}
		bool is_running(bool update) {
			if (update) {
				this->update();
			}
			return this->is_running();
		}
		bool is_started() const {
			return this->started;
		}
		bool just_finished() const {
			return this->just_finish;
		}
		bool just_finished(bool update) {
			if (update) {
				this->update();
			}
			return this->just_finished();
		}
		bool is_done() const {
			if (this->reversed) {
				return this->started && this->get_progress() <= 0.0;
			}
			else {
				return this->started && this->get_progress() >= 1.0;
			}
		}
		bool is_done(bool update) {
			if (update) {
				this->update();
			}
			return this->is_done();
		}
		bool is_done_reverse() const {
			return this->is_done() && this->reversed;
		}
		bool is_done_no_reverse() const {
			return this->is_done() && !this->reversed;
		}
		qpl::f64 get_progress() const {
			return qpl::clamp_0_1(this->progress_value);
		}
		qpl::f64 get_progress(bool update) {
			if (update) {
				this->update();
			}
			return this->get_progress();
		}
		qpl::f64 get_normalized_progress() const {
			if (this->reversed) {
				return 1 - this->get_progress();
			}
			else {
				return this->get_progress();
			}
		}
		qpl::f64 get_normalized_progress(bool update)  {
			if (update) {
				this->update();
			}
			return this->get_normalized_progress();
		}
	};
}

#endif