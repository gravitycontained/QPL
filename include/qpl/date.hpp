#ifndef QPL_DATE_HPP
#define QPL_DATE_HPP

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>

#include <string_view>

namespace qpl {

	enum class week_days : int {
		monday = 1,
		tuesday = 2,
		wednesday = 3,
		thursday = 4,
		friday = 5,
		saturday = 6,
		sunday = 7,
	};




	struct date_ms {
		qpl::u16 years = 0;
		qpl::u8 months = 0;
		qpl::u8 days = 0;
		qpl::u8 hours = 0;
		qpl::u8 minutes = 0;
		qpl::u8 seconds = 0;
		qpl::u16 milliseconds = 0;

		date_ms() {

		}

		QPLDLL void set(std::string string);
		QPLDLL std::string string() const;
	};

	date_ms get_current_time();

	struct year_month_day {
		qpl::u16 year = 0;
		qpl::u8 month = 0;
		qpl::u8 day = 0;

		year_month_day() {

		}
		year_month_day(std::string_view str) {
			*this = str;
		}


		QPLDLL week_days get_week_day() const;

		QPLDLL std::string_view get_week_day_string() const;
		QPLDLL bool is_monday() const;
		QPLDLL bool is_tuesday() const;
		QPLDLL bool is_wednesday() const;
		QPLDLL bool is_thursday() const;
		QPLDLL bool is_friday() const;
		QPLDLL bool is_saturday() const;
		QPLDLL bool is_sunday() const;

		QPLDLL year_month_day& operator=(std::string_view date);
		QPLDLL bool operator==(const year_month_day& other) const;
		QPLDLL bool operator>(const year_month_day& other) const;
		QPLDLL bool operator<=(const year_month_day& other) const;
		QPLDLL bool operator<(const year_month_day& other) const;
		QPLDLL bool operator>=(const year_month_day& other) const;

		QPLDLL qpl::size days() const;
		QPLDLL std::string string()const;
	};

	struct hour_minutes {
		qpl::u8 hour = 0;
		qpl::u8 minute = 0;

		hour_minutes() {

		}
		hour_minutes(std::string_view str) {
			*this = str;
		}
		QPLDLL double days() const;
		QPLDLL qpl::size minutes() const;
		QPLDLL hour_minutes& operator=(std::string_view date);
		QPLDLL bool operator==(const hour_minutes& other) const;
		QPLDLL bool operator>(const hour_minutes& other) const;
		QPLDLL bool operator<=(const hour_minutes& other) const;
		QPLDLL bool operator<(const hour_minutes& other) const;
		QPLDLL bool operator>=(const hour_minutes& other) const;
		QPLDLL std::string string()const;
	};

	struct date {
		year_month_day ymd;
		hour_minutes hm;
		date() {

		}
		date(const std::string_view& ymd, const std::string_view& hm) : ymd(ymd), hm(hm) {

		}
		QPLDLL void set(const date_ms& date);
		QPLDLL date& operator=(const date_ms& date);
		QPLDLL date& operator=(const date& other);
		QPLDLL double days() const;

		QPLDLL std::string string() const;
	};


}

#endif