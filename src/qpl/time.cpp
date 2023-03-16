#include <qpl/algorithm.hpp>
#include <qpl/time.hpp>
#include <qpl/string.hpp>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <thread>

#ifdef QPL_INTERN_SFML_USE
#include <qpl/QSF/event_info.hpp>
#endif

namespace qpl {
	qpl::time qpl::time::clock_time() {
		return qpl::time{ 
			static_cast<qpl::u64>(
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) 
		};
	}
	qpl::time& qpl::time::operator=(const qpl::time& other) {
		this->m_ns = other.m_ns;
		return *this;
	}
	qpl::time& qpl::time::operator+=(const qpl::time& other) {
		this->m_ns += other.m_ns;
		return *this;
	}
	qpl::time qpl::time::operator+(const qpl::time& other) const {
		auto copy = *this;
		copy += other;
		return copy;
	}	
	qpl::time& qpl::time::operator-=(const qpl::time& other) {
		if (other.m_ns >= this->m_ns) {
			this->m_ns = 0;
		}
		else {
			this->m_ns -= other.m_ns;
		}
		return *this;
	}
	qpl::time qpl::time::operator-(const qpl::time& other) const {
		auto copy = *this;
		copy -= other;
		return copy;
	}
	qpl::time& qpl::time::operator/=(const qpl::time& other) {
		this->m_ns /= other.m_ns;
		return *this;
	}
	qpl::time qpl::time::operator/(const qpl::time& other) const {
		auto copy = *this;
		copy /= other;
		return copy;
	}
	qpl::time& qpl::time::operator%=(const qpl::time& other) {
		this->m_ns %= other.m_ns;
		return *this;
	}
	qpl::time qpl::time::operator%(const qpl::time& other) const {
		auto copy = *this;
		copy %= other;
		return copy;
	}

	bool qpl::time::operator==(const qpl::time& other) const {
		return this->m_ns == other.m_ns;
	}
	bool qpl::time::operator!=(const qpl::time& other) const {
		return this->m_ns != other.m_ns;
	}
	bool qpl::time::operator<(const qpl::time& other) const {
		return this->m_ns < other.m_ns;
	}
	bool qpl::time::operator<=(const qpl::time& other) const {
		return this->m_ns <= other.m_ns;
	}
	bool qpl::time::operator>(const qpl::time& other) const {
		return this->m_ns > other.m_ns;
	}
	bool qpl::time::operator>=(const qpl::time& other) const {
		return this->m_ns >= other.m_ns;
	}

	std::string qpl::time::single_short_nsecs_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->nsecs_mod()), '0', qpl::number_of_digits(this->nsecs_in_usec - 1)), this->nsecs_mod(), "ns");
	}
	std::string qpl::time::single_short_usecs_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->usecs_mod()), '0', qpl::number_of_digits(this->usecs_in_msec - 1)), this->usecs_mod(), "us");
	}
	std::string qpl::time::single_short_msecs_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->msecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)), this->msecs_mod(), "ms");
	}
	std::string qpl::time::single_short_secs_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)), this->secs_mod(), 's');
	}
	std::string qpl::time::single_short_mins_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)), this->mins_mod(), 'm');
	}
	std::string qpl::time::single_short_hours_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)), this->hours_mod(), 'h');
	}
	std::string qpl::time::single_short_days_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)), this->days_mod(), 'd');
	}
	std::string qpl::time::single_short_days_week_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->days_week_mod()), '0', qpl::number_of_digits(this->days_in_week - 1)), this->days_week_mod(), 'd');
	}
	std::string qpl::time::single_short_weeks_string() const {
		return qpl::to_string(qpl::string_to_fit(qpl::to_string(this->weeks_mod()), '0', qpl::number_of_digits(this->weeks_in_year - 1)), this->weeks_mod(), 'w');
	}
	std::string qpl::time::single_short_years_string() const {
		return qpl::to_string(this->years(), 'y');
	}

	std::string qpl::time::single_descriptive_nsecs_string() const {
		return qpl::to_string(this->nsecs_mod(), " nanosecond", this->nsecs_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_usecs_string() const {
		return qpl::to_string(this->usecs_mod(), " microsecond", this->usecs_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_msecs_string() const {
		return qpl::to_string(this->msecs_mod(), " millisecond", this->msecs_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_secs_string() const {
		return qpl::to_string(this->secs_mod(), " second", this->secs_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_mins_string() const {
		return qpl::to_string(this->mins_mod(), " minute", this->mins_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_hours_string() const {
		return qpl::to_string(this->hours_mod(), " hour", this->hours_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_days_string() const {
		return qpl::to_string(this->days_mod(), " day", this->days_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_days_week_string() const {
		return qpl::to_string(this->days_week_mod(), " day", this->days_week_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_weeks_string() const {
		return qpl::to_string(this->weeks_mod(), " week", this->weeks_mod() == 1u ? "" : "s");
	}
	std::string qpl::time::single_descriptive_years_string() const {
		return qpl::to_string(this->years(), " year", this->years() == 1u ? "" : "s");
	}

	std::string qpl::time::nsecs_string(bool short_string) const {
		return short_string ? this->single_short_nsecs_string() : this->single_descriptive_nsecs_string();
	}
	std::string qpl::time::usecs_string(bool short_string) const {
		return short_string ? this->single_short_usecs_string() : this->single_descriptive_usecs_string();
	}
	std::string qpl::time::msecs_string(bool short_string) const {
		return short_string ? this->single_short_msecs_string() : this->single_descriptive_msecs_string();
	}
	std::string qpl::time::secs_string(bool short_string) const {
		return short_string ? this->single_short_secs_string() : this->single_descriptive_secs_string();
	}
	std::string qpl::time::mins_string(bool short_string) const {
		return short_string ? this->single_short_mins_string() : this->single_descriptive_mins_string();
	}
	std::string qpl::time::hours_string(bool short_string) const {
		return short_string ? this->single_short_hours_string() : this->single_descriptive_hours_string();
	}
	std::string qpl::time::days_string(bool short_string) const {
		return short_string ? this->single_short_days_string() : this->single_descriptive_days_string();
	}
	std::string qpl::time::days_week_string(bool short_string) const {
		return short_string ? this->single_short_days_week_string() : this->single_descriptive_days_week_string();
	}
	std::string qpl::time::weeks_string(bool short_string) const {
		return short_string ? this->single_short_weeks_string() : this->single_descriptive_weeks_string();
	}
	std::string qpl::time::years_string(bool short_string) const {
		return short_string ? this->single_short_years_string() : this->single_descriptive_years_string();
	}


	std::vector<std::string> qpl::time::get_string_segments(bool short_string, bool weeks) const {
		std::vector<std::string> result;
		if (this->years()) {
			result.push_back(this->years_string(short_string));
		}
		else {
			result.push_back("");
		}
		if (weeks) {
			if (this->weeks_mod()) {
				result.push_back(this->weeks_string(short_string));
			}
			else {
				result.push_back("");
			}
			if (this->days_week_mod()) {
				result.push_back(this->days_week_string(short_string));
			}
			else {
				result.push_back("");
			}
		}
		else {
			if (this->days_mod()) {
				result.push_back(this->days_string(short_string));
			}
			else {
				result.push_back("");
			}
		}
		if (this->hours_mod()) {
			result.push_back(this->hours_string(short_string));
		}
		else {
			result.push_back("");
		}
		if (this->mins_mod()) {
			result.push_back(this->mins_string(short_string));
		}
		else {
			result.push_back("");
		}
		if (this->secs_mod()) {
			result.push_back(this->secs_string(short_string));
		}
		else {
			result.push_back("");
		}
		if (this->msecs_mod()) {
			result.push_back(this->msecs_string(short_string));
		}
		else {
			result.push_back("");
		}
		if (this->usecs_mod()) {
			result.push_back(this->usecs_string(short_string));
		}
		else {
			result.push_back("");
		}
		return result;
	}

	std::vector<std::string> qpl::time::get_full_string_segments(bool short_string, bool weeks) const {
		std::vector<std::string> result;
		result.push_back(this->years_string(short_string));
		if (weeks) {
			result.push_back(this->weeks_string(short_string));
			result.push_back(this->days_week_string(short_string));
		}
		else {
			result.push_back(this->days_string(short_string));
		}
		result.push_back(this->hours_string(short_string));
		result.push_back(this->mins_string(short_string));
		result.push_back(this->secs_string(short_string));
		result.push_back(this->msecs_string(short_string));
		result.push_back(this->usecs_string(short_string));
	return result;
	}
	std::vector<std::string> qpl::time::get_string_active_segments(qpl::size stop_at_segment, bool short_string, bool weeks, qpl::size precision) const {
		auto segments = this->get_string_segments(short_string, weeks);

		auto stop = qpl::min(stop_at_segment, segments.size());
		qpl::size found = 0u;
		std::vector<std::string> result;
		for (qpl::size i = 0u; i < stop && found < precision; ++i) {
			if (!segments[i].empty()) {
				++found;
				result.push_back(segments[i]);
			}
		}
		if (segments.empty()) {
			segments.push_back(segments[stop_at_segment]);
		}
		return result;
	}

	std::string qpl::time::string_until_segment(qpl::size stop_at_segment, std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		std::ostringstream stream;
		if (parantheses.size() > 0) {
			stream << parantheses[0];
		}
		auto segments = this->get_string_active_segments(stop_at_segment, short_string, weeks, precision);
		auto stop = qpl::min(segments.size(), stop_at_segment);

		for (qpl::size i = 0u; i < stop; ++i) {
			if (i) {
				if (!short_string && i == stop - 1) {
					stream << " and ";
				}
				else {
					stream << seperation;
				}
			}
			stream << segments[i];
		}

		if (parantheses.size() > 1) {
			stream << parantheses[1];
		}
		return stream.str();
	}


	std::string qpl::time::string_full(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks) const {
		std::ostringstream stream;
		if (parantheses.size() > 0) {
			stream << parantheses[0];
		}

		auto segments = this->get_full_string_segments(short_string, weeks);
		for (qpl::size i = 0u; i < segments.size(); ++i) {
			if (i) {
				if (!short_string && i == segments.size() - 1) {
					stream << " and ";
				}
				else {
					stream << seperation;
				}
			}
			stream << segments[i];
		}

		if (parantheses.size() > 1) {
			stream << parantheses[1];
		}
		return stream.str();
	}

	std::string qpl::time::string(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		return this->string_until_segment(qpl::size_max, parantheses, seperation, short_string, weeks, precision);
	}


	std::string qpl::time::string_until_hour(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		return this->string_until_segment(weeks ? 4u : 3u, parantheses, seperation, short_string, weeks, precision);
	}
	std::string qpl::time::string_until_min(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		return this->string_until_segment(weeks ? 5u : 4u, parantheses, seperation, short_string, weeks, precision);
	}
	std::string qpl::time::string_until_sec(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		return this->string_until_segment(weeks ? 6u : 5u, parantheses, seperation, short_string, weeks, precision);
	}
	std::string qpl::time::string_until_ms(std::string_view parantheses, std::string_view seperation, bool short_string, bool weeks, qpl::size precision) const {
		return this->string_until_segment(weeks ? 7u : 6u, parantheses, seperation, short_string, weeks, precision);
	}

	std::string qpl::time::descriptive_string(qpl::size precision, bool weeks, qpl::size stop_at_segment) const {
		return this->string_until_segment(stop_at_segment, "", ", ", false, weeks, precision);
	}
	std::string qpl::time::compact_string(qpl::size precision, bool weeks, qpl::size stop_at_segment) const {
		return this->string_until_segment(stop_at_segment, "[]", " : ", true, weeks, precision);
	}

	std::string qpl::time::small_string(qpl::size precision, bool weeks, bool short_string, std::string_view parantheses, std::string_view seperation) const {
		return this->string(parantheses, seperation, short_string, weeks, precision);
	}
	std::string qpl::time::small_descriptive_string(qpl::size precision, bool weeks) const {
		return this->descriptive_string(precision, weeks);
	}
	std::string qpl::time::small_compact_string(qpl::size precision, bool weeks) const {
		return this->compact_string(precision, weeks);
	}



	qpl::f64 qpl::time::frequency() const {
		auto x = qpl::f64_cast(this->m_ns);
		return qpl::time::nsecs_in_sec / x;
	}

	qpl::u64 qpl::time::nsecs() const {
		return this->m_ns;
	}	
	qpl::u64 qpl::time::usecs() const {
		return this->m_ns / qpl::time::nsecs_in_usec;
	}
	qpl::u64 qpl::time::msecs() const {
		return this->m_ns / qpl::time::nsecs_in_msec;
	}
	qpl::u64 qpl::time::secs() const {
		return this->m_ns / qpl::time::nsecs_in_sec;
	}
	qpl::u64 qpl::time::mins() const {
		return this->m_ns / qpl::time::nsecs_in_min;
	}
	qpl::u64 qpl::time::hours() const {
		return this->m_ns / qpl::time::nsecs_in_hour;
	}
	qpl::u64 qpl::time::days() const {
		return this->m_ns / qpl::time::nsecs_in_day;
	}
	qpl::u64 qpl::time::weeks() const {
		return this->m_ns / qpl::time::nsecs_in_week;
	}
	qpl::u64 qpl::time::years() const {
		return this->m_ns / qpl::time::nsecs_in_year;
	}

	qpl::u64 qpl::time::nsecs_mod() const {
		return this->nsecs() % this->nsecs_in_usec;
	}
	qpl::u64 qpl::time::usecs_mod() const {
		return this->usecs() % this->usecs_in_msec;
	}
	qpl::u64 qpl::time::msecs_mod() const {
		return this->msecs() % this->msecs_in_sec;
	}
	qpl::u64 qpl::time::secs_mod() const {
		return this->secs() % this->secs_in_min;
	}
	qpl::u64 qpl::time::mins_mod() const {
		return this->mins() % this->mins_in_hour;
	}
	qpl::u64 qpl::time::hours_mod() const {
		return this->hours() % this->hours_in_day;
	}
	qpl::u64 qpl::time::days_mod() const {
		return this->days() % this->days_in_year;
	}
	qpl::u64 qpl::time::days_week_mod() const {
		return (this->days() % this->days_in_year) % this->days_in_week;
	}
	qpl::u64 qpl::time::weeks_mod() const {
		return (this->weeks() % this->weeks_in_year);
	}
	qpl::u64 qpl::time::years_mod() const {
		return this->years();
	}

	qpl::f64 qpl::time::nsecs_f() const {
		return static_cast<qpl::f64>(this->m_ns);
	}
	qpl::f64 qpl::time::usecs_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_usec;
	}
	qpl::f64 qpl::time::msecs_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_msec;
	}
	qpl::f64 qpl::time::secs_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_sec;
	}
	qpl::f64 qpl::time::mins_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_min;
	}
	qpl::f64 qpl::time::hours_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_hour;
	}
	qpl::f64 qpl::time::days_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_day;
	}
	qpl::f64 qpl::time::years_f() const {
		return static_cast<qpl::f64>(this->m_ns) / qpl::time::nsecs_in_year;
	}

	void qpl::time::set(qpl::u64 ns) {
		this->m_ns = ns;
	}

	std::ostream& operator<<(std::ostream& os, const qpl::time& time) {
		return (os << time.string());
	}

	void qpl::clock::reset() {
		this->m_begin = qpl::time::clock_time();
		this->m_running = true;
		this->m_pause_sum = qpl::u64{};
	}
	void qpl::clock::pause() {
		if (this->is_running()) {
			this->measure();
		}
		this->m_running = false;
		this->m_pause = qpl::time::clock_time();
	}
	void qpl::clock::reset_pause() {
		if (this->is_running()) {
			this->measure();
		}
		this->m_begin = qpl::time::clock_time();
		this->m_running = false;
		this->m_pause = qpl::time::clock_time();
		this->m_pause_sum = qpl::u64{};
	}
	qpl::time qpl::clock::elapsed() const {
		if (this->is_running()) {
			this->measure();
		}
		return this->m_end - (this->m_begin + this->m_pause_sum);
	}
	std::string qpl::clock::elapsed_str() const {
		return this->elapsed().string();
	}
	qpl::f64 qpl::clock::elapsed_f() const {
		return this->elapsed().secs_f();
	}

	qpl::time qpl::clock::elapsed_reset() {
		auto result = this->elapsed();
		this->reset();
		return result;
	}
	std::string qpl::clock::elapsed_str_reset() {
		auto result = this->elapsed_str();
		this->reset();
		return result;
	}
	qpl::f64 qpl::clock::elapsed_f_reset() {
		auto result = this->elapsed_f();
		this->reset();
		return result;
	}
	void qpl::clock::add(qpl::f64 seconds) {
		this->m_begin += qpl::u64_cast(seconds * qpl::time::nsecs_in_sec);
	}
	void qpl::clock::add(qpl::time time) {
		this->m_begin += time;
	}
	void qpl::clock::subtract(qpl::f64 seconds) {
		this->m_begin -= qpl::u64_cast(seconds * qpl::time::nsecs_in_sec);
	}
	void qpl::clock::subtract(qpl::time time) {
		this->m_begin -= time;
	}

	bool qpl::clock::has_elapsed(qpl::f64 seconds) const {
		return this->elapsed_f() > seconds;
	}
	bool qpl::clock::has_elapsed(qpl::time duration) const {
		return this->elapsed() > duration;
	}
	bool qpl::clock::has_elapsed_reset(qpl::f64 seconds) {
		auto result = this->has_elapsed(seconds);
		if (result) {
			this->reset();
		}
		return result;
	}
	bool qpl::clock::has_elapsed_reset(qpl::time duration) {
		auto result = this->has_elapsed(duration);
		if (result) {
			this->reset();
		}
		return result;
	}
	void qpl::clock::resume() {
		if (this->is_running()) {
			return;
		}
		this->m_running = true;
		this->m_pause_sum += (qpl::time::clock_time() - this->m_pause);
	}
	bool qpl::clock::is_running() const {
		return this->m_running;
	}
	bool qpl::clock::is_paused() const {
		return !this->m_running;
	}


	void qpl::clock::measure() const {
		if (this->is_running()) {
			this->m_end = qpl::time::clock_time();
		}
	}
	void qpl::clock::init(bool running) {
		this->m_begin = qpl::time::clock_time();
		this->m_end = this->m_begin;
		this->m_pause_sum = qpl::u64{};
		this->m_running = running;
		if (this->is_paused()) {
			this->m_pause = this->m_begin;
		}
	}	

	void qpl::small_clock::reset() {
		this->m_begin = qpl::time::clock_time();
	}
	qpl::time qpl::small_clock::elapsed() const {
		auto time = qpl::time::clock_time();
		return time - this->m_begin;
	}
	std::string qpl::small_clock::elapsed_str() const {
		return this->elapsed().string();
	}
	qpl::f64 qpl::small_clock::elapsed_f() const {
		return this->elapsed().secs_f();
	}

	qpl::time qpl::small_clock::elapsed_reset() {
		auto result = this->elapsed();
		this->reset();
		return result;
	}
	std::string qpl::small_clock::elapsed_str_reset() {
		auto result = this->elapsed_str();
		this->reset();
		return result;
	}
	qpl::f64 qpl::small_clock::elapsed_f_reset() {
		auto result = this->elapsed_f();
		this->reset();
		return result;
	}
	void qpl::small_clock::add(qpl::f64 seconds) {
		this->m_begin += qpl::u64_cast(seconds * qpl::time::nsecs_in_sec);
	}
	void qpl::small_clock::add(qpl::time time) {
		this->m_begin += time;
	}
	void qpl::small_clock::subtract(qpl::f64 seconds) {
		this->m_begin -= qpl::u64_cast(seconds * qpl::time::nsecs_in_sec);
	}
	void qpl::small_clock::subtract(qpl::time time) {
		this->m_begin -= time;
	}

	bool qpl::small_clock::has_elapsed(qpl::f64 seconds) const {
		return this->elapsed_f() > seconds;
	}
	bool qpl::small_clock::has_elapsed(qpl::time duration) const {
		return this->elapsed() > duration;
	}
	bool qpl::small_clock::has_elapsed_reset(qpl::f64 seconds) {
		auto result = this->has_elapsed(seconds);
		if (result) {
			this->reset();
		}
		return result;
	}
	bool qpl::small_clock::has_elapsed_reset(qpl::time duration) {
		auto result = this->has_elapsed(duration);
		if (result) {
			this->reset();
		}
		return result;
	}

	std::ostream& operator<<(std::ostream& os, const qpl::clock& clock) {
		return (os << clock.elapsed().string());
	}
	std::ostream& operator<<(std::ostream& os, const qpl::halted_clock& clock) {
		return (os << clock.elapsed().string());
	}

	void wait(qpl::time duration) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(duration.nsecs()));
	}
	void wait(double seconds) {
		if (seconds <= 0) return;
		qpl::wait(qpl::secs(seconds));
	}
	void wait_while(double seconds) {
		if (seconds <= 0) return;
		auto n = qpl::secs(seconds);
		qpl::clock clock;
		while (clock.elapsed() < n) {

		}
	}

	std::chrono::system_clock::time_point get_current_system_time() {
		return std::chrono::system_clock::now();
	}
	std::chrono::utc_clock::time_point get_current_utc_time() {
		return std::chrono::utc_clock::now();
	}
	//std::chrono::system_clock::time_point get_current_utc_time() {
	//	std::chrono::zoned_time zt{ "UTC", qpl::get_current_system_time()};
	//	auto utc_time = std::chrono::system_clock::to_time_t(zt.get_sys_time());
	//	return std::chrono::system_clock::from_time_t(utc_time);
	//}

	std::string get_current_time_string() {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 



		std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
		return { buffer };
	}

	std::string unix_to_date(qpl::u32 unix) {
		std::time_t rawtime = unix;
		std::tm* timeinfo;
		char buffer[80];


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 


		std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
		return { buffer };
	}

	/*
	std::chrono::system_clock::time_point utc_data_ymdhm_to_utc_timepoint(std::string date, std::string format) {
		std::istringstream stream{ date };
		std::chrono::system_clock::time_point result;
		stream >> parse(format, result);
		return result;
	}
	*/
	std::chrono::system_clock::time_point utc_data_ymdhm_to_utc_timepoint(std::string date, std::string format) {
		std::istringstream stream{ date };
		std::chrono::system_clock::time_point result;
		if (!std::chrono::from_stream(stream, "%F %T", result)) {
			throw qpl::exception("utc_data_ymdhm_to_utc_timepoint error : cannot parse date ", date);
		}
		return result;
	}

	qpl::time get_remaining_time(qpl::f64 progress, qpl::time elapsed) {
		qpl::time f = 0;
		if (progress) {
			f = qpl::u64_cast(elapsed.nsecs() / progress);
		}
		return qpl::u64_cast(f.nsecs() * (1 - progress));
	}
	qpl::time get_remaining_time(qpl::f64 progress, qpl::clock timer) {
		return qpl::get_remaining_time(progress, timer.elapsed());
	}

	std::string get_current_time_string_ms() {
		auto current_time = std::chrono::system_clock::now();
		
		auto transformed = current_time.time_since_epoch().count() / 10000;
		
		auto millis = transformed % 1000;
		
		auto str = get_current_time_string();
		return qpl::to_string(str, "-", qpl::prepended_to_string_to_fit(millis, '0', 3));
	}


	std::string get_current_time_string_ymd_hms() {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 



		std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

		return qpl::to_string(buffer);
	}
	std::string get_current_time_string_ymd_hmsms() {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 



		std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

		auto current_time = std::chrono::system_clock::now();

		auto transformed = current_time.time_since_epoch().count() / 10000;

		auto millis = transformed % 1000;

		return qpl::to_string(buffer, ":", qpl::prepended_to_string_to_fit(millis, '0', 3));
	}
	std::string qpl::get_current_time_string_ymd_hmsms_compact() {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 



		std::strftime(buffer, 80, "%Y%m%d% H%M%S", timeinfo);

		auto current_time = std::chrono::system_clock::now();

		auto transformed = current_time.time_since_epoch().count() / 10000;

		auto millis = transformed % 1000;

		return qpl::to_string(buffer, qpl::prepended_to_string_to_fit(millis, '0', 3));
	}
	std::string qpl::get_current_time_string_ymdhmsms_compact() {
		std::time_t rawtime;
		std::tm* timeinfo;
		char buffer[80];

		std::time(&rawtime);


#pragma warning( push )
#pragma warning( disable : 4996)
		timeinfo = std::localtime(&rawtime);
#pragma warning( pop ) 



		std::strftime(buffer, 80, "%Y%m%d%H%M%S", timeinfo);

		auto current_time = std::chrono::system_clock::now();

		auto transformed = current_time.time_since_epoch().count() / 10000;

		auto millis = transformed % 1000;

		return qpl::to_string(buffer, qpl::prepended_to_string_to_fit(qpl::to_string(millis), '0', 3));
	}

	std::unordered_map<std::string, std::unordered_map<std::string, qpl::halted_clock>> qpl::detail::sub_benchmark_clocks;
	std::unordered_map<std::string, qpl::halted_clock> qpl::detail::benchmark_clocks;
	std::string qpl::detail::last_benchmark_name;
	std::string qpl::detail::last_sub_benchmark_name;

	qpl::clock qpl::detail::signal_clock;
	qpl::u64 qpl::detail::signal_count;

	void qpl::begin_benchmark_end_previous(const std::string& name) {
		qpl::end_benchmark();
		qpl::begin_benchmark(name);
	}
	void qpl::begin_benchmark_end_previous(const std::string& sub, const std::string& name) {
		qpl::end_benchmark();
		qpl::begin_benchmark(sub, name);
	}
	qpl::halted_clock qpl::get_benchmark(const std::string& name) {
		return qpl::detail::benchmark_clocks[name];
	}
	void qpl::begin_benchmark(const std::string& name) {
		qpl::detail::last_benchmark_name = name;
		qpl::detail::benchmark_clocks[name].resume();
	}
	void qpl::begin_benchmark(const std::string& sub, const std::string& name) {
		qpl::detail::last_benchmark_name = name;
		qpl::detail::last_sub_benchmark_name = sub;
		qpl::detail::sub_benchmark_clocks[sub][name].resume();
	}
	void qpl::end_benchmark(const std::string& name) {
		qpl::detail::benchmark_clocks[name].pause();
	}
	void qpl::end_benchmark() {
		if (!qpl::detail::last_sub_benchmark_name.empty()) {
			qpl::detail::sub_benchmark_clocks[qpl::detail::last_sub_benchmark_name][qpl::detail::last_benchmark_name].pause();
			qpl::detail::last_sub_benchmark_name.clear();
			qpl::detail::last_benchmark_name.clear();
		}
		else if (!qpl::detail::last_benchmark_name.empty()) {
			qpl::detail::benchmark_clocks[qpl::detail::last_benchmark_name].pause();
			qpl::detail::last_benchmark_name.clear();
		}
	}
	void qpl::begin_benchmark_segments() {
		qpl::detail::benchmark_clocks.clear();
	}
	
	void qpl::clear_benchmark() {
		qpl::detail::benchmark_clocks.clear();
	}
	void qpl::reset_benchmark() {
		qpl::clear_benchmark();
	}
	void qpl::print_benchmark() {
		qpl::time sum = 0;

		qpl::size length_max = 0u;
		std::vector<std::pair<std::string, double>> sorted;
		for (auto i : qpl::detail::benchmark_clocks) {
			length_max = qpl::max(length_max, i.first.length());
			sum += i.second.elapsed();
			sorted.push_back(std::make_pair(i.first, i.second.elapsed_f()));
		}
		std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
			return a.second < b.second;
			});
		

		for (auto i : qpl::detail::benchmark_clocks) {
			if (i.first == sorted.front().first && sorted.size() >= 2u) {
				qpl::print(qpl::foreground::light_green);
			}
			else if (i.first == sorted.back().first && sorted.size() >= 3u) {
				qpl::print(qpl::foreground::light_red);
			}
			auto f = i.second.elapsed().nsecs_f() / sum.nsecs_f();

			auto precision = 3;
			auto percentage_string = qpl::percentage_string_precision(f, precision);
			percentage_string = qpl::prepended_to_string_to_fit(percentage_string, ' ', precision + 4);
			qpl::print(qpl::to_string(qpl::appended_to_string_to_fit(i.first, ' ', length_max + 1), " - ", qpl::str_spaced(percentage_string, 10), " time usage : ", i.second.elapsed().string()));

			if (i.first != sorted.back().first && sorted.size() >= 2u) {
				double p = sorted.back().second / i.second.elapsed_f();
				qpl::print(" [ ", qpl::to_string_precision(3, p), "x ]");
			}
			qpl::println();
		}

		for (auto& sub : qpl::detail::sub_benchmark_clocks) {
			qpl::println();
			sum = 0;
			length_max = 0u;
			sorted.clear();
			for (auto i : sub.second) {
				length_max = qpl::max(length_max, i.first.length());
				sum += i.second.elapsed();
				sorted.push_back(std::make_pair(i.first, i.second.elapsed_f()));
			}
			std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
				return a.second < b.second;
				});



			for (auto i : sub.second) {
				if (i.first == sorted.front().first && sorted.size() >= 2u) {
					qpl::print(qpl::foreground::light_green);
				}
				else if (i.first == sorted.back().first && sorted.size() >= 3u) {
					qpl::print(qpl::foreground::light_red);
				}

				
				auto f = i.second.elapsed().nsecs_f() / sum.nsecs_f();
				if (i.first != sorted.back().first && sorted.size() >= 2u) {
					double p = sorted.back().second / i.second.elapsed_f();
					qpl::println(qpl::to_string(sub.first, ": ", qpl::appended_to_string_to_fit(i.first, ' ', length_max + 1), " - ", qpl::str_spaced(qpl::to_string_precision(7, f * 100), 14), "% time usage : ", i.second.elapsed().string(), " [ ", qpl::to_string_precision(3, p), "x ]"));
				}
				else {
					qpl::println(qpl::to_string(sub.first, ": ", qpl::appended_to_string_to_fit(i.first, ' ', length_max + 1), " - ", qpl::str_spaced(qpl::to_string_precision(7, f * 100), 14), "% time usage : ", i.second.elapsed().string()));
				}
			}
		}
	}

	void qpl::print_benchmark(const std::string& name) {
		qpl::println(name, " took ", qpl::detail::benchmark_clocks[name].elapsed_reset().string());
	}


	void qpl::reset_time_signal() {
		qpl::detail::signal_clock.reset();
	}
	bool qpl::get_time_signal(double seconds) {
		return (qpl::detail::signal_clock.has_elapsed_reset(seconds));
	}
	bool qpl::get_time_signal(qpl::time time) {
		return (qpl::detail::signal_clock.has_elapsed_reset(time));
	}


	void qpl::reset_count_signal() {
		qpl::detail::signal_count = qpl::u64{};
	}
	bool qpl::get_count_signal(qpl::u64 when) {
		++qpl::detail::signal_count;
		if (qpl::detail::signal_count >= when) {
			qpl::detail::signal_count = qpl::u64{};
			return true;
		}
		return false;
	}


	void qpl::animation::internal_reset() {
		this->just_finish = false;
		this->just_finish_reverse = false;
		this->just_finish_no_reverse = false;
		this->internal_change_flag = false;
	}
	void qpl::animation::reset() {
		this->progress = 0.0;
		this->running = false;
		this->reversed = false;
		this->internal_reset();
	}
	void qpl::animation::start() {
		this->running = true;
	}
	void qpl::animation::stop() {
		this->running = true;
	}
	void qpl::animation::reset_and_start() {
		this->progress = 0.0;
		this->running = true;
		this->reversed = false;
		this->internal_reset();
	}
	void qpl::animation::reset_and_start_reverse() {
		this->progress = 1.0;
		this->running = true;
		this->reversed = true;
		this->internal_reset();
	}
	void qpl::animation::set_duration(qpl::f64 duration) {
		this->duration = duration;
	}
	void qpl::animation::update(qpl::f64 frame_time) {
		if (this->just_finish && !this->internal_change_flag) {
			this->running = false;
		}
		this->internal_change_flag = false;
		this->just_finish = false;
		this->just_finish_reverse = false;
		this->just_finish_no_reverse = false;


		if (this->running) {
			auto before = this->progress;

			auto f = frame_time / this->duration;
			if (this->reversed) {
				this->progress -= f;
			}
			else {
				this->progress += f;
			}

			this->progress = qpl::clamp_0_1(this->progress);
			if (this->progress == 1.0 && before < 1.0) {
				this->just_finish = this->just_finish_no_reverse = true;
			}
			else if (this->progress == 0.0 && before > 0.0) {
				this->just_finish = this->just_finish_reverse = true;
			}
		}

		this->progress_change_flag = (this->progress != this->progress_before);
		this->progress_before = this->progress;
	}
#ifdef QPL_INTERN_SFML_USE
	void qpl::animation::update(const qsf::event_info& event) {
		this->update(event.frame_time_f());
	}
#endif
	void qpl::animation::go_forwards() {
		this->start();
		this->reversed = false;
		this->internal_change_flag = true;
	}
	void qpl::animation::go_backwards() {
		this->start();
		this->reversed = true;
		this->internal_change_flag = true;
	}
	bool qpl::animation::is_running() const {
		return this->running;
	}
	bool qpl::animation::is_reversed() const {
		return this->reversed;
	}
	bool qpl::animation::changed() const {
		return this->progress_change_flag;
	}
	bool qpl::animation::just_finished() const {
		return this->just_finish;
	}
	bool qpl::animation::just_finished_reverse() const {
		return this->just_finish_reverse;
	}
	bool qpl::animation::just_finished_no_reverse() const {
		return this->just_finish_no_reverse;
	}
	bool qpl::animation::has_progress() const {
		return this->progress;
	}
	void qpl::animation::set_progress(qpl::f64 progress, bool backwards) {
		this->progress = qpl::clamp_0_1(progress);
		if (backwards) {
			this->go_backwards();
		}
		else {
			this->go_forwards();
		}
	}
	qpl::f64 qpl::animation::get_progress() const {
		return this->progress;
	}
	qpl::f64 qpl::animation::get_curve_progress(qpl::f64 curve) const {
		return qpl::curve_slope(this->progress, curve);
	}
	qpl::f64 qpl::animation::get_double_curve_progress(qpl::f64 curve) const {
		return qpl::double_curve_slope(this->progress, curve);
	}

	void qpl::timed_task::update() {
		auto done = this->is_done();
		this->just_finish = !this->done_before && done;
		if (this->just_finish) {
			this->function();
		}
		this->done_before = done;
	}
	bool qpl::timed_task::is_done() const {
		return this->clock.elapsed() > this->wait_duration;
	}
	bool qpl::timed_task::is_running() const {
		return !this->is_done();
	}
	bool qpl::timed_task::just_finished() const {
		return this->just_finish;
	}
	void qpl::timed_task::set_wait_duration(qpl::time time) {
		this->wait_duration = time;
	}
	void qpl::timed_task::set_wait_duration(qpl::f64 seconds) {
		this->wait_duration = qpl::secs(seconds);
	}
	qpl::f64 qpl::timed_task::get_wait_progress() const {
		auto p = this->clock.elapsed_f();
		return p / this->wait_duration.secs_f();
	}

	timed_task_manager qpl::detail::tasks;

	void qpl::timed_task_manager::clear() {
		this->tasks.clear();
	}
	void qpl::timed_task_manager::cleanup() {
		std::vector<timed_task> temp;
		temp.reserve(this->tasks.size());
		for (auto& i : this->tasks) {
			if (i.is_done()) {
				this->finished_tasks.insert(i.name);
			}
			else {
				temp.push_back(i);
			}
		}
		this->tasks = temp;
	}
	bool qpl::timed_task_manager::is_task_finished(const std::string& name) {
		return this->finished_tasks.find(name) != this->finished_tasks.cend();
	}
	bool qpl::timed_task_manager::is_task_running(const std::string& name) {
		for (auto& i : this->tasks) {
			if (i.name == name) {
				return i.is_running();
			}
		}
		return false;
	}
	void qpl::timed_task_manager::add_timed_task(qpl::time time, std::function<void(void)> function, const std::string& name) {
		this->tasks.push_back(qpl::timed_task(time, function, name));
	}
	void qpl::timed_task_manager::add_timed_task(qpl::f64 time, std::function<void(void)> function, const std::string& name) {
		this->tasks.push_back(qpl::timed_task(time, function, name));
	}
	void qpl::timed_task_manager::update() {
		bool any_done = false;
		for (auto& i : this->tasks) {
			i.update();
			if (i.is_done()) {
				any_done = true;
			}
		}
		this->finished_tasks.clear();
		if (any_done) {
			this->cleanup();
		}
	}

	void qpl::start_timed_task(qpl::time time, std::function<void(void)> function) {
		qpl::detail::tasks.add_timed_task(time, function);
	}
	void qpl::start_timed_task(qpl::time time, const std::string& name, std::function<void(void)> function) {
		qpl::detail::tasks.add_timed_task(time, function, name);
	}
	void qpl::start_timed_task(qpl::f64 time, std::function<void(void)> function) {
		qpl::detail::tasks.add_timed_task(time, function);
	}
	void qpl::start_timed_task(qpl::f64 time, const std::string& name, std::function<void(void)> function) {
		qpl::detail::tasks.add_timed_task(time, function, name);
	}
	bool qpl::timed_task_finished(const std::string& name) {
		return qpl::detail::tasks.is_task_finished(name);
	}	
	bool qpl::timed_task_running(const std::string& name) {
		return qpl::detail::tasks.is_task_running(name);
	}
	void qpl::clear_timed_tasks() {
		qpl::detail::tasks.clear();
	}
	void qpl::update_tasks() {
		qpl::detail::tasks.update();
	}
}