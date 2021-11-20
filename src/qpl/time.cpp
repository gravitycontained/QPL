#include <qpl/algorithm.hpp>
#include <qpl/time.hpp>
#include <qpl/string.hpp>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <thread>

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

	std::string qpl::time::string_non_zero() const {
		std::ostringstream stream;
		bool found = false;

		stream << '[';
		if (this->years()) {
			found = true;
			stream << this->years() << 'y';
		}
		if (this->days_mod()) {
			if (found) {
				stream << " : ";
			}
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)) << this->days_mod() << 'd';
		}
		if (this->hours_mod()) {
			if (found) {
				stream << " : ";
			}
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)) << this->hours_mod() << 'h';
		}
		if (this->mins_mod()) {
			if (found) {
				stream << " : ";
			}
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)) << this->mins_mod() << 'm';
		}
		if (this->secs_mod()) {
			if (found) {
				stream << " : ";
			}
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)) << this->secs_mod() << 's';
		}
		if (this->msecs_mod()) {
			if (found) {
				stream << " : ";
			}
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->msecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)) << this->msecs_mod() << "ms";
		}
		if (this->usecs_mod()) {
			if (found) {
				stream << " : ";
			}
			stream << qpl::string_to_fit(qpl::to_string(this->usecs_mod()), '0', qpl::number_of_digits(this->usecs_in_msec - 1)) << this->usecs_mod() << "us";
		}
		if (this->nsecs_mod() || !found) {
			if (found) {
				stream << " : ";
			}
			stream << qpl::string_to_fit(qpl::to_string(this->nsecs_mod()), '0', qpl::number_of_digits(this->nsecs_in_usec - 1)) << this->nsecs_mod() << "ns";
		}
		stream << ']';
		return stream.str();
	}
	std::string qpl::time::string() const {
		std::ostringstream stream;
		bool found = false;

		stream << '[';
		if (this->years() || found) {
			found = true;
			stream << this->years() << "y : ";
		}
		if (this->days_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)) << this->days_mod() << "d : ";
		}
		if (this->hours_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)) << this->hours_mod() << "h : ";
		}
		if (this->mins_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)) << this->mins_mod() << "m : ";
		}
		if (this->secs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)) << this->secs_mod() << "s : ";
		}
		if (this->msecs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->msecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)) << this->msecs_mod() << "ms : ";
		}
		if (this->usecs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->usecs_mod()), '0', qpl::number_of_digits(this->usecs_in_msec - 1)) << this->usecs_mod() << "us : ";
		}
		stream << qpl::string_to_fit(qpl::to_string(this->nsecs_mod()), '0', qpl::number_of_digits(this->nsecs_in_usec - 1)) << this->nsecs_mod() << "ns]";
		return stream.str();
	}

	std::string qpl::time::string_until_sec() const {
		std::ostringstream stream;
		bool found = false;

		stream << '[';
		if (this->years() || found) {
			found = true;
			stream << this->years() << "y : ";
		}
		if (this->days_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)) << this->days_mod() << "d : ";
		}
		if (this->hours_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)) << this->hours_mod() << "h : ";
		}
		if (this->mins_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)) << this->mins_mod() << "m : ";
		}
		if (this->secs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)) << this->secs_mod() << "s";
		}
		stream << "]";
		return stream.str();
	}

	std::string qpl::time::string_until_ms() const {
		std::ostringstream stream;
		bool found = false;

		stream << '[';
		if (this->years() || found) {
			found = true;
			stream << this->years() << "y : ";
		}
		if (this->days_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)) << this->days_mod() << "d : ";
		}
		if (this->hours_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)) << this->hours_mod() << "h : ";
		}
		if (this->mins_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)) << this->mins_mod() << "m : ";
		}
		if (this->secs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)) << this->secs_mod() << "s : ";
		}
		if (this->msecs_mod() || found) {
			found = true;
			stream << qpl::string_to_fit(qpl::to_string(this->msecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)) << this->msecs_mod() << "ms";
		}
		stream << "]";
		return stream.str();
	}
	std::string qpl::time::string_full() const {
		std::ostringstream stream;

		stream << '[';
		stream << this->years() << "y : ";
		stream << qpl::string_to_fit(qpl::to_string(this->days_mod()), '0', qpl::number_of_digits(this->days_in_year - 1)) << this->days_mod() << "d : ";
		stream << qpl::string_to_fit(qpl::to_string(this->hours_mod()), '0', qpl::number_of_digits(this->hours_in_day - 1)) << this->hours_mod() << "h : ";
		stream << qpl::string_to_fit(qpl::to_string(this->mins_mod()), '0', qpl::number_of_digits(this->mins_in_hour - 1)) << this->mins_mod() << "m : ";
		stream << qpl::string_to_fit(qpl::to_string(this->secs_mod()), '0', qpl::number_of_digits(this->secs_in_min - 1)) << this->secs_mod() << "s : ";
		stream << qpl::string_to_fit(qpl::to_string(this->msecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)) << this->msecs_mod() << "ms : ";
		stream << qpl::string_to_fit(qpl::to_string(this->usecs_mod()), '0', qpl::number_of_digits(this->usecs_in_msec - 1)) << this->usecs_mod() << "us : ";
		stream << qpl::string_to_fit(qpl::to_string(this->nsecs_mod()), '0', qpl::number_of_digits(this->msecs_in_sec - 1)) << this->nsecs_mod() << "ns]";
		return stream.str();
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
		qpl::wait(qpl::secs(seconds));
	}


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

		return qpl::to_string(buffer, qpl::prepended_to_string_to_fit(millis, '0', 3));
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
			if (i.first != sorted.back().first && sorted.size() >= 2u) {
				double p = sorted.back().second / i.second.elapsed_f();
				qpl::println(qpl::to_string(qpl::appended_to_string_to_fit(i.first, ' ', length_max + 1), " - ", qpl::str_spaced(qpl::to_string_precision(7, f * 100), 14), "% time usage : ", i.second.elapsed().string(), " [ ", qpl::to_string_precision(3, p), "x ]"));
			}
			else {
				qpl::println(qpl::to_string(qpl::appended_to_string_to_fit(i.first, ' ', length_max + 1), " - ", qpl::str_spaced(qpl::to_string_precision(7, f * 100), 14), "% time usage : ", i.second.elapsed().string()));
			}
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

	void qpl::animation::set_duration(qpl::f64 duration) {
		this->duration = duration;
	}
	void qpl::animation::stop() {
		this->started = false;
	}
	void qpl::animation::reverse(bool running) {
		if (running) {
			if (this->reversed) {
				this->reset(running);
				this->reverse_progress_state = 0.0;
				this->reversed = false;
			}
			else {
				this->reset(running);
				this->reverse_progress_state = 1.0;
				this->progress_value = 1.0;
				this->reversed = true;
			}
			this->running = true;
		}
		else {
			this->reset();
			this->reverse_progress_state = 0.0;
			this->reversed = true;
		}
	}
	void qpl::animation::go_forwards() {
		if (this->is_running()) {
			if (this->is_reversed()) {
				auto progress = this->get_progress();
				this->reverse_progress_state = 0.0;
				this->timer.reset();
				this->timer.subtract(progress * this->duration);
				this->reversed = !this->reversed;
			}
		}
		else if (this->get_progress() == 0) {
			this->reset(true);
			this->reverse_progress_state = 0.0;
			this->progress_value = 0.0;
			this->reversed = false;
		}
		this->running = true;
	}
	void qpl::animation::go_backwards() {
		if (this->is_running()) {
			if (!this->is_reversed()) {
				this->reverse_progress_state = this->get_progress() * 2;
				this->reversed = !this->reversed;
			}
		}
		else if (this->get_progress() == 1) {
			this->reset(true);
			this->reverse_progress_state = 1.0;
			this->progress_value = 1.0;
			this->reversed = true;
		}
		this->running = true;
	}
	void qpl::animation::swap() {
		if (this->is_reversed()) {
			this->go_forwards();
		}
		else {
			this->go_backwards();
		}
		//if (this->is_running()) {
		//	if (this->is_reversed()) {
		//		auto progress = this->get_progress();
		//		this->reverse_progress_state = 0.0;
		//		this->timer.reset();
		//		this->timer.subtract(progress * this->duration);
		//	}
		//	else {
		//		this->reverse_progress_state = this->get_progress() * 2;
		//	}
		//	this->reversed = !this->reversed;
		//}
		//else {
		//	if (this->get_progress() == 0) {
		//		this->reset(true);
		//		this->reverse_progress_state = 0.0;
		//		this->progress_value = 0.0;
		//		this->reversed = false;
		//	}
		//	else if (this->get_progress() == 1) {
		//		this->reset(true);
		//		this->reverse_progress_state = 1.0;
		//		this->progress_value = 1.0;
		//		this->reversed = true;
		//	}
		//	else {
		//		qpl::println("qsf::animation::swap : bad state : ", this->get_progress(), " ", this->progress_value, " ", this->reverse_progress_state, " ", this->is_reversed());
		//	}
		//}
		//this->running = true;
	}

	void qpl::animation::reset(bool running) {
		this->timer.reset();
		this->running_result = running;
		this->running = running;
		this->started = true;
		this->just_finish = false;
		this->reversed = false;
		this->progress_value = 0.0;
		this->wait_duration = 0.0;
	}
	void qpl::animation::add_time(qpl::f64 seconds) {
		this->timer.add(seconds);
	}
	void qpl::animation::add_time(qpl::time time) {
		this->timer.add(time);
	}
	void qpl::animation::start() {
		this->reset(true);
	}
	void qpl::animation::start_and_reverse() {
		this->reverse();
	}
	void qpl::animation::reset_and_reverse() {
		this->reset();
		this->reverse();
	}
	void qpl::animation::reset_and_start() {
		this->start();
	}
	void qpl::animation::start_in(qpl::f64 time) {
		if (!time) {
			this->start();
			return;
		}
		this->started = true;
		this->wait_duration = time;
		this->wait_timer.reset();
	}
	void qpl::animation::update() {
		if (this->wait_duration) {
			if (this->wait_timer.has_elapsed(this->wait_duration)) {
				this->start();
				this->wait_duration = 0.0;
			}
			return;
		}
		this->just_finish = false;
		if (!this->running) {
			this->running_result = false;
			return;
		}
		if (!this->started) {
			this->running_result = false;
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
			this->running = false;
		}
		this->running_result = this->running || this->just_finish;
	}

	bool qpl::animation::is_reversed() const {
		return this->reversed;
	}
	bool qpl::animation::is_running() const {
		return this->running_result;
	}
	bool qpl::animation::is_running(bool update) {
		if (update) {
			this->update();
		}
		return this->is_running();
	}
	bool qpl::animation::is_started() const {
		return this->started;
	}
	bool qpl::animation::just_finished() const {
		return this->just_finish;
	}
	bool qpl::animation::just_finished(bool update) {
		if (update) {
			this->update();
		}
		return this->just_finished();
	}
	bool qpl::animation::is_done() const {
		if (this->reversed) {
			return this->started && this->get_progress() <= 0.0;
		}
		else {
			return this->started && this->get_progress() >= 1.0;
		}
	}
	bool qpl::animation::is_done(bool update) {
		if (update) {
			this->update();
		}
		return this->is_done();
	}
	bool qpl::animation::is_running_or_done() const {
		return this->is_running() || this->is_done();
	}
	bool qpl::animation::is_running_or_done(bool update) {
		return this->is_running(update) || this->is_done(update);
	}
	bool qpl::animation::is_done_reverse() const {
		return this->is_done() && this->reversed;
	}
	bool qpl::animation::is_done_no_reverse() const {
		return this->is_done() && !this->reversed;
	}
	void qpl::animation::set_progress(qpl::f64 progress) {
		this->timer.reset();
		this->timer.subtract(progress * this->duration);
	}
	qpl::f64 qpl::animation::get_progress() const {
		return qpl::clamp_0_1(this->progress_value);
	}
	qpl::f64 qpl::animation::get_curve_progress(qpl::f64 curve) const {
		return qpl::smooth_curve(this->get_progress(), curve);
	}
	qpl::f64 qpl::animation::get_smooth_progress(qpl::u32 slope) const {
		return qpl::smooth_slope(this->get_progress(), slope);
	}
	qpl::f64 qpl::animation::get_progress(bool update) {
		if (update) {
			this->update();
		}
		return this->get_progress();
	}
	qpl::f64 qpl::animation::get_normalized_progress() const {
		if (this->reversed) {
			return 1 - this->get_progress();
		}
		else {
			return this->get_progress();
		}
	}
	qpl::f64 qpl::animation::get_normalized_progress(bool update) {
		if (update) {
			this->update();
		}
		return this->get_normalized_progress();
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