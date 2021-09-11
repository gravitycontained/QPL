#include <qpl/date.hpp>
#include <qpl/time.hpp>

namespace qpl {


	void qpl::date_ms::set(std::string string) {
		auto nums = qpl::split_numbers<qpl::u16>(string);

		this->years = qpl::u16_cast(nums[0]);
		this->months = qpl::u8_cast(nums[1]);
		this->days = qpl::u8_cast(nums[2]);
		this->hours = qpl::u8_cast(nums[3]);
		this->minutes = qpl::u8_cast(nums[4]);
		this->seconds = qpl::u8_cast(nums[5]);
		this->milliseconds = qpl::u16_cast(nums[6]);
	}
	std::string qpl::date_ms::string() const {
		std::ostringstream stream;
		stream << qpl::i32_cast(this->years) << "-";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->months), '0', 2) << "-";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->days), '0', 2) << " ";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->hours), '0', 2) << ":";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->minutes), '0', 2) << ":";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->seconds), '0', 2) << ":";
		stream << qpl::prepended_to_string_to_fit(qpl::i32_cast(this->milliseconds), '0', 3);
		return stream.str();
	}
	date_ms qpl::get_current_time() {
		qpl::date_ms result;
		auto str = qpl::get_current_time_string_ms();
		result.set(str);
		return result;
	}

	qpl::week_days qpl::year_month_day::get_week_day() const {
		int rst =
			this->day
			+ ((153 * (this->month + 12 * ((14 - this->month) / 12) - 3) + 2) / 5)
			+ (365 * (this->year + 4800 - ((14 - this->month) / 12)))
			+ ((this->year + 4800 - ((14 - this->month) / 12)) / 4)
			- ((this->year + 4800 - ((14 - this->month) / 12)) / 100)
			+ ((this->year + 4800 - ((14 - this->month) / 12)) / 400)
			- 32045;

		return static_cast<week_days>(rst % 7 + 1);
	}

	std::string_view qpl::year_month_day::get_week_day_string() const {
		switch (this->get_week_day()) {
		case week_days::monday: return "monday"; break;
		case week_days::tuesday: return "tuesday"; break;
		case week_days::wednesday: return "wednesday"; break;
		case week_days::thursday: return "thursday"; break;
		case week_days::friday: return "friday"; break;
		case week_days::saturday: return "saturday"; break;
		case week_days::sunday: return "sunday"; break;
		default: return ""; break;
		}
	}
	bool qpl::year_month_day::is_monday() const {
		return this->get_week_day() == week_days::monday;
	}
	bool qpl::year_month_day::is_tuesday() const {
		return this->get_week_day() == week_days::tuesday;
	}
	bool qpl::year_month_day::is_wednesday() const {
		return this->get_week_day() == week_days::wednesday;
	}
	bool qpl::year_month_day::is_thursday() const {
		return this->get_week_day() == week_days::thursday;
	}
	bool qpl::year_month_day::is_friday() const {
		return this->get_week_day() == week_days::friday;
	}
	bool qpl::year_month_day::is_saturday() const {
		return this->get_week_day() == week_days::saturday;
	}
	bool qpl::year_month_day::is_sunday() const {
		return this->get_week_day() == week_days::sunday;
	}

	qpl::year_month_day& qpl::year_month_day::operator=(std::string_view date) {
		this->year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + (date[3] - '0');
		this->month = (date[5] - '0') * 10 + (date[6] - '0');
		this->day = (date[8] - '0') * 10 + (date[9] - '0');
		return *this;
	}
	bool qpl::year_month_day::operator==(const year_month_day& other) const {
		return this->year == other.year && this->month == other.month && this->day == other.day;
	}
	bool qpl::year_month_day::operator>(const year_month_day& other) const {
		if (this->year != other.year) {
			return this->year > other.year;
		}
		if (this->month != other.month) {
			return this->month > other.month;
		}
		if (this->day != other.day) {
			return this->day > other.day;
		}
		return false;
	}
	bool qpl::year_month_day::operator<=(const year_month_day& other) const {
		return !(*this > other);
	}
	bool qpl::year_month_day::operator<(const year_month_day& other) const {
		if (this->year != other.year) {
			return this->year < other.year;
		}
		if (this->month != other.month) {
			return this->month < other.month;
		}
		if (this->day != other.day) {
			return this->day < other.day;
		}
		return false;
	}
	bool qpl::year_month_day::operator>=(const year_month_day& other) const {
		return !(*this < other);
	}

	qpl::size qpl::year_month_day::days() const {
		return this->year * qpl::size{ 365 } +this->month * qpl::size{ 30 } +this->day;
	}
	std::string qpl::year_month_day::string()const {
		return qpl::to_string(this->year, "-", this->month < 10 ? "0" : "", (int)this->month, "-", this->day < 10 ? "0" : "", (int)this->day);
	}

	double qpl::hour_minutes::days() const {
		return this->minutes() / (24.0 * 60.0);
	}
	qpl::size qpl::hour_minutes::minutes() const {
		return this->hour * qpl::size{ 24 } +this->minute;
	}
	hour_minutes& qpl::hour_minutes::operator=(std::string_view date) {
		this->hour = (date[0] - '0') * 10 + (date[1] - '0');
		this->minute = (date[3] - '0') * 10 + (date[4] - '0');
		return *this;
	}
	bool qpl::hour_minutes::operator==(const hour_minutes& other) const {
		return this->hour == other.hour && this->minute == other.minute;
	}
	bool qpl::hour_minutes::operator>(const hour_minutes& other) const {
		if (this->hour != other.hour) {
			return this->hour > other.hour;
		}
		if (this->minute != other.minute) {
			return this->minute > other.minute;
		}
		return false;
	}
	bool qpl::hour_minutes::operator<=(const hour_minutes& other) const {
		return !(*this > other);
	}
	bool qpl::hour_minutes::operator<(const hour_minutes& other) const {
		if (this->hour != other.hour) {
			return this->hour < other.hour;
		}
		if (this->minute != other.minute) {
			return this->minute < other.minute;
		}
		return false;
	}
	bool qpl::hour_minutes::operator>=(const hour_minutes& other) const {
		return !(*this < other);
	}
	std::string qpl::hour_minutes::string()const {
		return qpl::to_string(this->hour < 10 ? "0" : "", (int)this->hour, ":", this->minute < 10 ? "0" : "", (int)this->minute);
	}

	void qpl::date::set(const date_ms& date) {
		this->ymd.year = date.years;
		this->ymd.month = date.months;
		this->ymd.day = date.days;
		this->hm.hour = date.hours;
		this->hm.minute = date.minutes;
	}
	date& qpl::date::operator=(const date_ms& date) {
		this->set(date);
		return *this;
	}
	date& qpl::date::operator=(const date& other) {
		this->ymd = other.ymd;
		this->hm = other.hm;
		return *this;
	}
	double qpl::date::days() const {
		return this->ymd.days() + this->hm.days();
	}
	std::string qpl::date::string() const {
		return this->ymd.string() + " " + this->hm.string();
	}

}