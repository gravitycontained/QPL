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

	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time nsecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time usecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_usec) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time msecs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_msec) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time secs(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_sec) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time mins(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_min) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time hours(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_hour) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
	constexpr qpl::time days(T value) {
		return qpl::time{ static_cast<qpl::u64>(value * qpl::time::nsecs_in_day) };
	}
	template<typename T, QPLCONCEPT(qpl::is_arithmetic<T>())>
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
	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::clock& clock);
	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::halted_clock& clock);


	struct fps_counter {
		fps_counter() {
			this->ema.reset();
			this->ema.time_period = 100u;
			this->clock.reset();
			this->start = false;
		}

		void set_time_period(qpl::u32 time_period) {
			this->ema.time_period = time_period;
		}
		qpl::u32 get_time_period() const {
			return this->ema.time_period;
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
		qpl::ema ema;
	};

	QPLDLL void wait(qpl::time duration);
	QPLDLL void wait(double seconds);

	//YYYY-MM-DD-HH-MM-SS
	QPLDLL std::string get_current_time_string();
	QPLDLL std::string get_current_time_string_ms();
	QPLDLL std::string get_current_time_string_ymd_hmsms();
	QPLDLL std::string get_current_time_string_ymd_hmsms_compact();
	QPLDLL std::string unix_to_date(qpl::u32 unix);

	QPLDLL qpl::time get_remaining_time(qpl::f64 progress, qpl::time elapsed);

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
	QPLDLL void begin_benchmark(const std::string& name);
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

	template<typename T>
	struct animation_key_frame {
		animation_key_frame(T& value, T new_value, qpl::f32 duration) {
			this->reference = &value;
			this->old_value = value;
			this->new_value = new_value;
			this->time = qpl::f32{};
			this->duration = duration;
			this->before = value;
		}
		animation_key_frame(const animation_key_frame& other) {
			this->reference = other.reference;
			this->old_value = other.old_value;
			this->new_value = other.new_value;
			this->time = other.time;
			this->duration = other.duration;
			this->before = other.before;
		}
		mutable T* reference;
		mutable T old_value;
		mutable T new_value;
		mutable T diff;
		mutable T before;
		mutable qpl::f32 time = qpl::f32{};
		mutable qpl::f32 duration = qpl::f32{ 1 };


		qpl::f32 get_progress() const {
			return this->time / this->duration;
		}
		void set(T& value, T new_value, qpl::f32 duration) const {
			this->reconfigure = &value;
			this->old_value = value;
			this->new_value = new_value;
			this->time = qpl::f32{};
			this->duration = duration;
			this->diff = T{};
			this->before = value;
		}
		void reconfigure(T new_value, qpl::f32 duration) const {
			this->old_value = *reference;
			this->new_value = new_value;
			this->time = qpl::f32{};
			this->duration = duration;
			this->diff = T{};
			this->before = *reference;
		}

		void update(qpl::f32 delta_time) const {
			if (this->get_progress() < 1.0f) {
				this->time += delta_time;
				*this->reference = static_cast<T>(qpl::linear_interpolation(this->old_value, this->new_value, qpl::smooth_progression(this->get_progress())));
			}
			this->diff = *this->reference - this->before;
			this->before = *this->reference;
		}
		T get_difference() const {
			return this->diff;
		}
	};
	namespace detail {
		QPLDLL extern std::vector<animation_key_frame<qpl::u8>> animation_key_frames_u8;
		QPLDLL extern std::vector<animation_key_frame<qpl::i8>> animation_key_frames_i8;
		QPLDLL extern std::vector<animation_key_frame<qpl::u16>> animation_key_frames_u16;
		QPLDLL extern std::vector<animation_key_frame<qpl::i16>> animation_key_frames_i16;
		QPLDLL extern std::vector<animation_key_frame<qpl::u32>> animation_key_frames_u32;
		QPLDLL extern std::vector<animation_key_frame<qpl::i32>> animation_key_frames_i32;
		QPLDLL extern std::vector<animation_key_frame<qpl::u64>> animation_key_frames_u64;
		QPLDLL extern std::vector<animation_key_frame<qpl::i64>> animation_key_frames_i64;
		QPLDLL extern std::vector<animation_key_frame<qpl::f32>> animation_key_frames_f32;
		QPLDLL extern std::vector<animation_key_frame<qpl::f64>> animation_key_frames_f64;

	}


	template<typename T>
	void add_animation_key_frame(T& value, T new_value, qpl::f32 duration) {
		if constexpr (std::is_same_v<std::decay_t<T>, qpl::u8>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_u8.begin(); it != qpl::detail::animation_key_frames_u8.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_u8.emplace_back(qpl::animation_key_frame<qpl::u8>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i8>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_i8.begin(); it != qpl::detail::animation_key_frames_i8.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_i8.emplace_back(qpl::animation_key_frame<qpl::i8>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u16>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_u16.begin(); it != qpl::detail::animation_key_frames_u16.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_u16.emplace_back(qpl::animation_key_frame<qpl::u16>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i16>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_i16.begin(); it != qpl::detail::animation_key_frames_i16.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_i16.emplace_back(qpl::animation_key_frame<qpl::i16>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u32>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_u32.begin(); it != qpl::detail::animation_key_frames_u32.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_u32.emplace_back(qpl::animation_key_frame<qpl::u32>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i32>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_i32.begin(); it != qpl::detail::animation_key_frames_i32.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_i32.emplace_back(qpl::animation_key_frame<qpl::i32>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u64>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_u64.begin(); it != qpl::detail::animation_key_frames_u64.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_u64.emplace_back(qpl::animation_key_frame<qpl::u64>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i64>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_i64.begin(); it != qpl::detail::animation_key_frames_i64.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_i64.emplace_back(qpl::animation_key_frame<qpl::i64>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f32>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_f32.begin(); it != qpl::detail::animation_key_frames_f32.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_f32.emplace_back(qpl::animation_key_frame<qpl::f32>(value, new_value, duration));
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f64>) {
			bool found = false;
			for (auto it = qpl::detail::animation_key_frames_f64.begin(); it != qpl::detail::animation_key_frames_f64.end(); ++it) {
				if (it->reference == &value) {
					it->reconfigure(new_value, duration); found = true; return;
				}
			}
			if (!found) {
				qpl::detail::animation_key_frames_f64.emplace_back(qpl::animation_key_frame<qpl::f64>(value, new_value, duration));
			}
		}
		else {
			static_assert("unkown animation_key_frame type");
		}
	}
	template<typename T>
	qpl::f32 get_animation_key_frame_progress(T& value) {
		if constexpr (std::is_same_v<std::decay_t<T>, qpl::u8>) {
			for (auto& i : qpl::detail::animation_key_frames_u8) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i8>) {
			for (auto& i : qpl::detail::animation_key_frames_i8) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u16>) {
			for (auto& i : qpl::detail::animation_key_frames_u16) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i16>) {
			for (auto& i : qpl::detail::animation_key_frames_i16) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u32>) {
			for (auto& i : qpl::detail::animation_key_frames_u32) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i32>) {
			for (auto& i : qpl::detail::animation_key_frames_i32) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u64>) {
			for (auto& i : qpl::detail::animation_key_frames_u64) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i64>) {
			for (auto& i : qpl::detail::animation_key_frames_i64) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f32>) {
			for (auto& i : qpl::detail::animation_key_frames_f32) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f64>) {
			for (auto& i : qpl::detail::animation_key_frames_f64) {
				if (i.reference == &value) {
					return i.get_progress();
				}
			}
		}
		return qpl::f32{-1.f};
	}

	template<typename T>
	T get_animation_key_frame_difference(T& value) {
		if constexpr (std::is_same_v<std::decay_t<T>, qpl::u8>) {
			for (auto& i : qpl::detail::animation_key_frames_u8) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i8>) {
			for (auto& i : qpl::detail::animation_key_frames_i8) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u16>) {
			for (auto& i : qpl::detail::animation_key_frames_u16) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i16>) {
			for (auto& i : qpl::detail::animation_key_frames_i16) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u32>) {
			for (auto& i : qpl::detail::animation_key_frames_u32) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i32>) {
			for (auto& i : qpl::detail::animation_key_frames_i32) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::u64>) {
			for (auto& i : qpl::detail::animation_key_frames_u64) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::i64>) {
			for (auto& i : qpl::detail::animation_key_frames_i64) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f32>) {
			for (auto& i : qpl::detail::animation_key_frames_f32) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, qpl::f64>) {
			for (auto& i : qpl::detail::animation_key_frames_f64) {
				if (i.reference == &value) {
					return i.get_difference();
				}
			}
		}
		return qpl::f32{ 0.f };
	}
	QPLDLL void update_all_animation_key_frames(qpl::f32 delta_time);

	template<typename T>
	qpl::f32 is_animation_key_frame_running(T& value) {
		auto progress = qpl::get_animation_key_frame_progress(value);
		return progress > 0.0f && progress < 1.0f;
	}
	template<typename T>
	qpl::f32 is_animation_key_frame_done(T& value) {
		return !(qpl::is_animation_key_frame_running(value));
	}
}

#endif