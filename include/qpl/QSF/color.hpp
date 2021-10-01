#ifndef QPLSF_COLOR_HPP
#define QPLSF_COLOR_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/algorithm.hpp>
#include <string>

namespace qsf {

	struct rgb;
	struct frgb {
		qpl::f32 r = 0.0;
		qpl::f32 g = 0.0;
		qpl::f32 b = 0.0;
		qpl::f32 a = 1.0;

		frgb() {

		}
		frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b, qpl::f32 a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		frgb(const rgb& other) {
			*this = other;
		}
		frgb(const frgb& other) {
			*this = other;
		}


		QPLDLL frgb& operator=(const frgb& other);
		QPLDLL frgb& operator=(const rgb& other);

		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb& operator/=(T value) {

			this->r /= value;
			this->g /= value;
			this->b /= value;
			this->a /= value;
			//this->r = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->r) / value, static_cast<qpl::f32>(1.0f));
			//this->g = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->g) / value, static_cast<qpl::f32>(1.0f));
			//this->b = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->b) / value, static_cast<qpl::f32>(1.0f));
			//this->a = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->a) / value, static_cast<qpl::f32>(1.0f));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb& operator*=(T value) {

			this->r *= value;
			this->g *= value;
			this->b *= value;
			this->a *= value;
			//this->r = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->r) * value, static_cast<qpl::f32>(1.0f));
			//this->g = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->g) * value, static_cast<qpl::f32>(1.0f));
			//this->b = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->b) * value, static_cast<qpl::f32>(1.0f));
			//this->a = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->a) * value, static_cast<qpl::f32>(1.0f));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb& operator+=(T value) {
			this->r += value;
			this->g += value;
			this->b += value;
			this->a += value;
			//this->r = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->r) + value, static_cast<qpl::f32>(1.0f));
			//this->g = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->g) + value, static_cast<qpl::f32>(1.0f));
			//this->b = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->b) + value, static_cast<qpl::f32>(1.0f));
			//this->a = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->a) + value, static_cast<qpl::f32>(1.0f));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb& operator-=(T value) {
			this->r -= value;
			this->g -= value;
			this->b -= value;
			this->a -= value;
			//this->r = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->r) - value, static_cast<qpl::f32>(1.0f));
			//this->g = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->g) - value, static_cast<qpl::f32>(1.0f));
			//this->b = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->b) - value, static_cast<qpl::f32>(1.0f));
			//this->a = qpl::clamp(qpl::f32{}, static_cast<qpl::f32>(this->a) - value, static_cast<qpl::f32>(1.0f));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::frgb operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL std::string string() const;

		QPLDLL qsf::frgb& operator*=(const qsf::frgb& other);
		QPLDLL qsf::frgb operator*(const qsf::frgb& other) const;
		QPLDLL qsf::frgb& operator/=(const qsf::frgb& other);
		QPLDLL qsf::frgb operator/(const qsf::frgb& other) const;
		QPLDLL qsf::frgb& operator-=(const qsf::frgb& other);
		QPLDLL qsf::frgb operator-(const qsf::frgb& other) const;
		QPLDLL qsf::frgb& operator+=(const qsf::frgb& other);
		QPLDLL qsf::frgb operator+(const qsf::frgb& other) const;
		QPLDLL bool operator==(const qsf::frgb& other) const;
		QPLDLL bool operator!=(const qsf::frgb& other) const;

		static const qsf::frgb unset;
	};

	struct rgb {
		constexpr rgb() {
			*this = rgb::white;
		}
		rgb(sf::Color color) {
			*this = color;
		}
		rgb(const frgb& other) {
			*this = other;
		}
		constexpr rgb(qpl::u8 r, qpl::u8 g, qpl::u8 b, qpl::u8 a = qpl::u8_max) : r(r), g(g), b(b), a(a) {

		}
		constexpr rgb(qpl::u32 uint) : r(), g(), b(), a() {
			if (uint <= (qpl::u32_max >> 8)) {
				uint <<= 8;
				uint |= 0xFFu;
			}

			if (std::is_constant_evaluated()) {
				this->r = (uint & 0xFF000000u) >> 24;
				this->g = (uint & 0x00FF0000u) >> 16;
				this->b = (uint & 0x0000FF00u) >> 8;
				this->a = (uint & 0x000000FFu);
			}
			else {
				qpl::u32 n = ((uint & 0xFF000000u) >> 24) | ((uint & 0x000000FFu) << 24) | ((uint & 0x00FF0000u) >> 8) | ((uint & 0x0000FF00u) << 8);
				memcpy(this, &n, sizeof(qpl::u32));
			}
		}
		constexpr rgb(const rgb& other) : r(), g(), b(), a() {
			if (std::is_constant_evaluated()) {
				this->r = other.r;
				this->g = other.g;
				this->b = other.b;
				this->a = other.a;
			}
			else {
				memcpy(this, &other, sizeof(rgb));
			}
		}

		qpl::u8 r = 0;
		qpl::u8 g = 0;
		qpl::u8 b = 0;
		qpl::u8 a = 0;

		QPLDLL constexpr qsf::rgb& operator=(const rgb& other);
		QPLDLL qsf::rgb& operator=(const frgb& other);
		QPLDLL qsf::rgb& operator=(sf::Color color);
		QPLDLL constexpr qsf::rgb& operator=(qpl::u32 uint);
		QPLDLL constexpr bool operator==(const rgb& other) const;

		QPLDLL operator sf::Color() const;

		QPLDLL std::string string() const;
		QPLDLL std::string hex_string() const;
		QPLDLL constexpr bool is_unset() const;
		QPLDLL constexpr qpl::u32 uint() const;

		QPLDLL qsf::rgb& interpolate(qsf::rgb color, qpl::f64 strength = qpl::f64{ 1 });
		QPLDLL qsf::rgb interpolated(qsf::rgb color, qpl::f64 strength = qpl::f64{ 1 }) const;

		QPLDLL static qsf::rgb interpolation(const std::vector<qsf::rgb>& colors, qpl::f64 strength);
		QPLDLL static qsf::rgb grey_shade(qpl::u8 strength = 128);

		QPLDLL qsf::rgb& invert();
		QPLDLL qsf::rgb inverted() const;
		QPLDLL qsf::rgb with_alpha(qpl::u8 alpha) const;
		QPLDLL qsf::rgb multiplied_alpha(qpl::u8 alpha) const;
		QPLDLL qsf::rgb multiplied_color(qsf::rgb color) const;

		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb& operator/=(T value) {
			this->r /= value;
			this->g /= value;
			this->b /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb& operator*=(T value) {
			this->r *= value;
			this->g *= value;
			this->b *= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}		
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb& operator+=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r + value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g + value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b + value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb& operator-=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r - value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g - value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b - value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qsf::rgb operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL qsf::rgb& operator*=(qsf::rgb other);
		QPLDLL qsf::rgb operator*(qsf::rgb other) const;
		QPLDLL qsf::rgb& operator/=(qsf::rgb other);
		QPLDLL qsf::rgb operator/(qsf::rgb other) const;
		QPLDLL qsf::rgb& operator-=(qsf::rgb other);
		QPLDLL qsf::rgb operator-(qsf::rgb other) const;
		QPLDLL qsf::rgb& operator+=(qsf::rgb other);
		QPLDLL qsf::rgb operator+(qsf::rgb other) const;

		const static qsf::rgb red;
		const static qsf::rgb green;
		const static qsf::rgb blue;
		const static qsf::rgb yellow;
		const static qsf::rgb orange;
		const static qsf::rgb cyan;
		const static qsf::rgb magenta;
		const static qsf::rgb white;
		const static qsf::rgb grey;
		const static qsf::rgb black;
		const static qsf::rgb transparent;
		const static qsf::rgb unset;
	};



	QPLDLL qsf::rgb get_random_color();
	QPLDLL qsf::rgb get_random_transparency_color();
	QPLDLL qsf::rgb get_rainbow_color(qpl::f64 f);
}


#endif
#endif