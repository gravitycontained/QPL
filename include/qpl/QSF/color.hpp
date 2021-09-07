#ifndef QPLSF_COLOR_HPP
#define QPLSF_COLOR_HPP
#pragma once

#ifndef QPL_NO_SFML
#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <qpl/vardef.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/algorithm.hpp>
#include <string>

namespace qsf {

	union rgb;
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

	union rgb {
		rgb() {
			*this = rgb::white;
		}
		rgb(sf::Color color) {
			*this = color;
		}
		rgb(qpl::u8 r, qpl::u8 g, qpl::u8 b, qpl::u8 a = qpl::u8_max) {
			this->c.r = r;
			this->c.g = g;
			this->c.b = b;
			this->c.a = a;
		}
		rgb(qpl::u32 hex) {
			*this = hex;
		}
		rgb(const frgb& other) {
			*this = other;
		}		
		rgb(const rgb& other) {
			*this = other;
		}

		struct {
			qpl::u8 r;
			qpl::u8 g;
			qpl::u8 b;
			qpl::u8 a;
		} c;
		qpl::u32 uint;

		QPLDLL qsf::rgb& operator=(const rgb& other);
		QPLDLL qsf::rgb& operator=(const frgb& other);
		QPLDLL qsf::rgb& operator=(sf::Color color);
		QPLDLL qsf::rgb& operator=(qpl::u32 uint);
		QPLDLL bool operator==(const rgb& other) const;

		QPLDLL operator sf::Color() const;

		QPLDLL std::string string() const;
		QPLDLL std::string hex_string() const;
		QPLDLL bool is_unset() const;

		QPLDLL qsf::rgb& interpolate(qsf::rgb color, qpl::f64 strength = qpl::f64{ 1 });
		QPLDLL qsf::rgb interpolated(qsf::rgb color, qpl::f64 strength = qpl::f64{ 1 }) const;

		QPLDLL static qsf::rgb interpolation(const std::vector<qsf::rgb>& colors, qpl::f64 strength);

		QPLDLL qsf::rgb& invert();
		QPLDLL qsf::rgb inverted() const;
		QPLDLL qsf::rgb with_alpha(qpl::u8 alpha) const;

		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb& operator/=(T value) {
			this->c.r /= value;
			this->c.g /= value;
			this->c.b /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb& operator*=(T value) {
			this->c.r *= value;
			this->c.g *= value;
			this->c.b *= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}		
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb& operator+=(T value) {
			this->c.r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.r) + value, static_cast<qpl::i16>(qpl::u8_max));
			this->c.g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.g) + value, static_cast<qpl::i16>(qpl::u8_max));
			this->c.b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.b) + value, static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb& operator-=(T value) {
			this->c.r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.r) - value, static_cast<qpl::i16>(qpl::u8_max));
			this->c.g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.g) - value, static_cast<qpl::i16>(qpl::u8_max));
			this->c.b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->c.b) - value, static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qsf::rgb operator-(T value) const {
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

		static const qsf::rgb red;
		static const qsf::rgb green;
		static const qsf::rgb blue;
		static const qsf::rgb orange;
		static const qsf::rgb yellow;
		static const qsf::rgb cyan;
		static const qsf::rgb magenta;
		static const qsf::rgb white;
		static const qsf::rgb grey;
		static const qsf::rgb black;
		static const qsf::rgb transparent;
		static const qsf::rgb unset;
	};

	QPLDLL qsf::rgb get_random_color();
	QPLDLL qsf::rgb get_random_transparency_color();
	QPLDLL qsf::rgb get_rainbow_color(qpl::f64 f);
}


#endif
#endif