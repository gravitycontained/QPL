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
#include <set>
#include <functional>


#ifdef QPL_INTERN_SFML_USE
namespace qsf {
	struct event_info;
}
#endif

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

		QPLDLL std::string single_short_nsecs_string() const;
		QPLDLL std::string single_short_usecs_string() const;
		QPLDLL std::string single_short_msecs_string() const;
		QPLDLL std::string single_short_secs_string() const;
		QPLDLL std::string single_short_mins_string() const;
		QPLDLL std::string single_short_hours_string() const;
		QPLDLL std::string single_short_days_string() const;
		QPLDLL std::string single_short_days_week_string() const;
		QPLDLL std::string single_short_weeks_string() const;
		QPLDLL std::string single_short_years_string() const;

		QPLDLL std::string single_descriptive_nsecs_string() const;
		QPLDLL std::string single_descriptive_usecs_string() const;
		QPLDLL std::string single_descriptive_msecs_string() const;
		QPLDLL std::string single_descriptive_secs_string() const;
		QPLDLL std::string single_descriptive_mins_string() const;
		QPLDLL std::string single_descriptive_hours_string() const;
		QPLDLL std::string single_descriptive_days_string() const;
		QPLDLL std::string single_descriptive_days_week_string() const;
		QPLDLL std::string single_descriptive_weeks_string() const;
		QPLDLL std::string single_descriptive_years_string() const;

		QPLDLL std::string nsecs_string(bool short_string = true) const;
		QPLDLL std::string usecs_string(bool short_string = true) const;
		QPLDLL std::string msecs_string(bool short_string = true) const;
		QPLDLL std::string secs_string(bool short_string = true) const;
		QPLDLL std::string mins_string(bool short_string = true) const;
		QPLDLL std::string hours_string(bool short_string = true) const;
		QPLDLL std::string days_string(bool short_string = true) const;
		QPLDLL std::string days_week_string(bool short_string = true) const;
		QPLDLL std::string weeks_string(bool short_string = true) const;
		QPLDLL std::string years_string(bool short_string = true) const;

		QPLDLL std::vector<std::string> get_string_segments(bool short_string = true, bool weeks = true) const;
		QPLDLL std::vector<std::string> get_full_string_segments(bool short_string = true, bool weeks = true) const;
		QPLDLL std::vector<std::string> get_string_active_segments(qpl::size stop_at_segment, bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;

		QPLDLL std::string string_full(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true) const;
		QPLDLL std::string string_until_segment(qpl::size stop_at_segment, std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;

		QPLDLL std::string string(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;
		QPLDLL std::string string_until_hour(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;
		QPLDLL std::string string_until_min(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;
		QPLDLL std::string string_until_sec(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;
		QPLDLL std::string string_until_ms(std::string_view parantheses = "[]", std::string_view seperation = " : ", bool short_string = true, bool weeks = true, qpl::size precision = qpl::size_max) const;

		QPLDLL std::string descriptive_string(qpl::size precision = qpl::size_max, bool weeks = true, qpl::size stop_at_segment = qpl::size_max) const;
		QPLDLL std::string compact_string(qpl::size precision = qpl::size_max, bool weeks = true, qpl::size stop_at_segment = qpl::size_max) const;

		QPLDLL std::string small_string(qpl::size precision = 2u, bool weeks = true, bool short_string = true, std::string_view parantheses = "[]", std::string_view seperation = " : ") const;
		QPLDLL std::string small_descriptive_string(qpl::size precision = 2u, bool weeks = true) const;
		QPLDLL std::string small_compact_string(qpl::size precision = 2u, bool weeks = true) const;


		QPLDLL static time clock_time();
		QPLDLL qpl::f64 frequency() const;

		QPLDLL qpl::u64 nsecs() const;
		QPLDLL qpl::u64 usecs() const;
		QPLDLL qpl::u64 msecs() const;
		QPLDLL qpl::u64 secs() const;
		QPLDLL qpl::u64 mins() const;
		QPLDLL qpl::u64 hours() const;
		QPLDLL qpl::u64 days() const;
		QPLDLL qpl::u64 weeks() const;
		QPLDLL qpl::u64 years() const;

		QPLDLL qpl::u64 nsecs_mod() const;
		QPLDLL qpl::u64 usecs_mod() const;
		QPLDLL qpl::u64 msecs_mod() const;
		QPLDLL qpl::u64 secs_mod() const;
		QPLDLL qpl::u64 mins_mod() const;
		QPLDLL qpl::u64 hours_mod() const;
		QPLDLL qpl::u64 days_mod() const;
		QPLDLL qpl::u64 days_week_mod() const;
		QPLDLL qpl::u64 weeks_mod() const;
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
		constexpr static qpl::u64 days_in_week = qpl::u64{ 7ull };
		constexpr static qpl::u64 weeks_in_year = qpl::u64{ 52ull };

		constexpr static qpl::u64 nsecs_in_nsec = qpl::u64{ 1ull };
		constexpr static qpl::u64 nsecs_in_msec = usecs_in_msec * nsecs_in_usec;
		constexpr static qpl::u64 nsecs_in_sec  = msecs_in_sec * nsecs_in_msec;
		constexpr static qpl::u64 nsecs_in_min  = secs_in_min * nsecs_in_sec;
		constexpr static qpl::u64 nsecs_in_hour = mins_in_hour * nsecs_in_min;
		constexpr static qpl::u64 nsecs_in_day  = hours_in_day * nsecs_in_hour;
		constexpr static qpl::u64 nsecs_in_week  = days_in_week * nsecs_in_day;
		constexpr static qpl::u64 nsecs_in_year = days_in_year * nsecs_in_day;
	
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
		QPLDLL void add(qpl::time time);
		QPLDLL void subtract(qpl::f64 seconds);
		QPLDLL void subtract(qpl::time time);

		QPLDLL bool has_elapsed(qpl::f64 seconds) const;
		QPLDLL bool has_elapsed(qpl::time duration) const;
		QPLDLL bool has_elapsed_reset(qpl::f64 seconds);
		QPLDLL bool has_elapsed_reset(qpl::time duration);
	
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
		QPLDLL void add(qpl::time time);
		QPLDLL void subtract(qpl::f64 seconds);
		QPLDLL void subtract(qpl::time time);

		QPLDLL bool has_elapsed(qpl::f64 seconds) const;
		QPLDLL bool has_elapsed(qpl::time duration) const;
		QPLDLL bool has_elapsed_reset(qpl::f64 seconds);
		QPLDLL bool has_elapsed_reset(qpl::time duration);
	

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
	QPLDLL void wait_while(double seconds);

	template <typename T>
	constexpr std::time_t to_time_t(T t) {
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(t - T::clock::now() + system_clock::now());
		return system_clock::to_time_t(sctp);
	}
	template<typename T>
	std::string get_time_string(T duration, std::string format = "%Y-%m-%d-%H-%M-%S") {
		std::time_t time;
		if constexpr (qpl::is_same<T, std::time_t>()) {
			time = duration;
		}
		else {
			time = qpl::to_time_t(duration);
		}
		
#pragma warning( push )
#pragma warning( disable : 4996)
		std::tm* gmt = std::localtime(&time);
#pragma warning( pop ) 

		std::ostringstream buffer;
		buffer << std::put_time(gmt, format.c_str());
		return buffer.str();
	}

	QPLDLL std::chrono::system_clock::time_point get_current_system_time();
	QPLDLL std::chrono::utc_clock::time_point get_current_utc_time();

	//YYYY-MM-DD-HH-MM-SS
	QPLDLL std::string get_current_time_string();
	QPLDLL std::string get_current_time_string_ms();
	QPLDLL std::string get_current_time_string_ymd_hms();
	QPLDLL std::string get_current_time_string_ymd_hmsms();
	QPLDLL std::string get_current_time_string_ymd_hmsms_compact();
	QPLDLL std::string get_current_time_string_ymdhmsms_compact();
	QPLDLL std::string unix_to_date(qpl::u32 unix);
	
	//template <typename C> requires (qpl::is_container<C>())
	//std::chrono::system_clock::time_point data_ymdhm_to_utc_timepoint(const C& values) {
	//	std::tm tm = {
	//		/* tm_sec  */ 0,
	//		/* tm_min  */ qpl::i32_cast(values.at(4)),
	//		/* tm_hour */ qpl::i32_cast(values.at(3)),
	//		/* tm_mday */ qpl::i32_cast(values.at(2)),
	//		/* tm_mon  */ qpl::i32_cast(values.at(1) - 1),
	//		/* tm_year */ qpl::i32_cast(values.at(0) - 1900),
	//	};
	//	//tm.tm_isdst = 0; //utc
	//
	//	auto tm = std::mktime(&tm);
	//	auto local_field = *std::gmtime(&tm);
	//	return std::chrono::system_clock::from_time_t(std::mktime(&local_field));
	//}
	QPLDLL std::chrono::system_clock::time_point utc_data_ymdhm_to_utc_timepoint(std::string date, std::string format = "%F %T");
	template <typename C> requires (qpl::is_container<C>())
	auto data_ymdhm_to_timepoint(const C& values) {
		std::tm tm = {
			/* tm_sec  */ 0,
			/* tm_min  */ qpl::i32_cast(values.at(4)),
			/* tm_hour */ qpl::i32_cast(values.at(3)),
			/* tm_mday */ qpl::i32_cast(values.at(2)),
			/* tm_mon  */ qpl::i32_cast(values.at(1) - 1),
			/* tm_year */ qpl::i32_cast(values.at(0) - 1900),
		};
		//tm.tm_isdst = -1; // local time zone
		return std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}
	template <typename C> requires (qpl::is_container<C>())
	auto data_ymdhms_to_timepoint(const C& values) {
		std::tm tm = {
			/* tm_sec  */ qpl::i32_cast(values.at(5)),
			/* tm_min  */ qpl::i32_cast(values.at(4)),
			/* tm_hour */ qpl::i32_cast(values.at(3)),
			/* tm_mday */ qpl::i32_cast(values.at(2)),
			/* tm_mon  */ qpl::i32_cast(values.at(1) - 1),
			/* tm_year */ qpl::i32_cast(values.at(0) - 1900),
		};
		//tm.tm_isdst = -1; // local time zone
		return std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}

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

	template<typename F>
	void benchmark(std::string name, F&& function) {
		qpl::begin_benchmark(name);
		std::invoke(function);
		qpl::end_benchmark(name);
	}

	QPLDLL void clear_benchmark();
	QPLDLL void reset_benchmark();
	QPLDLL void begin_benchmark_segments();
	QPLDLL void print_benchmark();
	QPLDLL void print_benchmark(const std::string& name);

	QPLDLL void reset_time_signal();
	QPLDLL bool get_time_signal(double seconds);
	QPLDLL bool get_time_signal(qpl::time time);

	QPLDLL void reset_count_signal();
	QPLDLL bool get_count_signal(qpl::u64 when);

	struct animation {
		qpl::f64 duration = 1.0;
		qpl::f64 progress = 0.0;
		qpl::f64 progress_before = qpl::f64_max;
		bool reversed = false;
		bool running = false;
		bool just_finish = false;
		bool just_finish_reverse = false;
		bool just_finish_no_reverse = false;
		bool internal_change_flag = false;
		bool progress_change_flag = true;

		QPLDLL void internal_reset();
		QPLDLL void reset();
		QPLDLL void start();
		QPLDLL void stop();
		QPLDLL void reset_and_start();
		QPLDLL void reset_and_start_reverse();
		QPLDLL void set_duration(qpl::f64 duration);
		QPLDLL void update(qpl::f64 frame_time);
#ifdef QPL_INTERN_SFML_USE
		QPLDLL void update(const qsf::event_info& event);
#endif
		QPLDLL bool is_running() const;
		QPLDLL bool is_reversed() const;
		QPLDLL bool changed() const;
		QPLDLL void go_forwards();
		QPLDLL void go_backwards();
		QPLDLL bool just_finished() const;
		QPLDLL bool just_finished_reverse() const;
		QPLDLL bool just_finished_no_reverse() const;
		QPLDLL bool has_progress() const;
		QPLDLL void set_progress(qpl::f64 progress, bool backwards = false);
		QPLDLL qpl::f64 get_progress() const;
		QPLDLL qpl::f64 get_curve_progress(qpl::f64 curve = 1.5) const;
		QPLDLL qpl::f64 get_double_curve_progress(qpl::f64 curve = 1.5) const;
	};

	template<typename T>
	struct value_animation : animation {
		T begin;
		T end;
		double curve = 1.5;
		bool use_double_curve = true;

		void set_begin(T value) {
			this->begin = value;
		}
		void set_end(T value) {
			this->end = value;
		}
		void set_range(T begin, T end) {
			this->begin = begin;
			this->end = end;
		}
		void set_curve(qpl::f64 curve) {
			this->curve = curve;
		}
		void enable_double_curve() {
			this->use_double_curve = true;
		}
		void disable_double_curve() {
			this->use_double_curve = false;
		}
		qpl::f64 get_resulting_progress() const {
			if (this->use_double_curve) {
				return this->get_double_curve_progress(this->curve);
			}
			else {
				return this->get_curve_progress(this->curve);
			}
		}
		T get() const {
			auto p = this->get_resulting_progress();
			return qpl::linear_interpolation(this->begin, this->end, p);
		}
		T get(T begin) const {
			auto p = this->get_resulting_progress();
			return qpl::linear_interpolation(begin, this->end, p);
		}
		T get(T begin, T end) const {
			auto p = this->get_resulting_progress();
			return qpl::linear_interpolation(begin, end, p);
		}
	};

	struct timed_task {
		qpl::small_clock clock;
		qpl::time wait_duration;
		std::function<void(void)> function;
		std::string name = "";
		bool done_before = false;
		bool just_finish = false;

		timed_task() {

		}
		timed_task(qpl::time wait_duration, std::function<void(void)> function, const std::string& name = "") {
			this->wait_duration = wait_duration;
			this->function = function;
			this->name = name;
		}
		timed_task(qpl::f64 wait_duration, std::function<void(void)> function, const std::string& name = "") {
			this->wait_duration = qpl::secs(wait_duration);
			this->function = function;
			this->name = name;
		}

		QPLDLL void update();
		QPLDLL bool is_done() const;
		QPLDLL bool is_running() const;
		QPLDLL bool just_finished() const;
		QPLDLL void set_wait_duration(qpl::time time);
		QPLDLL void set_wait_duration(qpl::f64 seconds);
		QPLDLL qpl::f64 get_wait_progress() const;
	};

	struct timed_task_manager {
		std::vector<timed_task> tasks;
		std::set<std::string> finished_tasks;

		QPLDLL void clear();
		QPLDLL void cleanup();
		QPLDLL bool is_task_finished(const std::string& name);
		QPLDLL bool is_task_running(const std::string& name);
		QPLDLL void add_timed_task(qpl::time time, std::function<void(void)> function, const std::string& name = "");
		QPLDLL void add_timed_task(qpl::f64 time, std::function<void(void)> function, const std::string& name = "");
		QPLDLL void update();
	};

	namespace detail {
		QPLDLL extern timed_task_manager tasks;
	}

	QPLDLL void start_timed_task(qpl::time time, const std::string& name, std::function<void(void)> function);
	QPLDLL void start_timed_task(qpl::time time, std::function<void(void)> function);
	QPLDLL void start_timed_task(qpl::f64 time, const std::string& name, std::function<void(void)> function);
	QPLDLL void start_timed_task(qpl::f64 time, std::function<void(void)> function);
	QPLDLL bool timed_task_finished(const std::string& name = "");
	QPLDLL bool timed_task_running(const std::string& name = "");
	QPLDLL void clear_timed_tasks();
	QPLDLL void update_tasks();
}

#endif