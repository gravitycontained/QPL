#ifndef QPL_COLOR_HPP
#define QPL_COLOR_HPP
#pragma once

#if defined QPL_INTERN_SFML_USE
#include <SFML/Graphics.hpp>
#endif

#include <qpl/qpldeclspec.hpp>
#include <qpl/defines.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/vector.hpp>
#include <string>
#include <qpl/vardef.hpp>

namespace qpl {

	struct rgb;
	struct rgba;
	struct frgb;

	struct frgba {
		qpl::f32 r = 0.0;
		qpl::f32 g = 0.0;
		qpl::f32 b = 0.0;
		qpl::f32 a = 1.0;

		constexpr frgba() {

		}
		constexpr frgba(qpl::f32 r, qpl::f32 g, qpl::f32 b, qpl::f32 a = 1.0f) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		constexpr frgba(const qpl::frgba& other) {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
			this->a = other.a;
		}
		constexpr frgba(const qpl::rgba& other);
		constexpr frgba(const qpl::rgb& other);
		constexpr frgba(const qpl::frgb& other);

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr frgba(const qpl::vectorN<T, N>& other) : r(), g(), b(), a() {
			*this = other;
		}

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr qpl::frgba& operator=(const qpl::vectorN<T, N>& other) {
			this->r = qpl::f32_cast(other[0]);

			if constexpr (N >= 2) {
				this->g = qpl::f32_cast(other[1]);
			}
			else {
				this->g = 0.0f;
			}
			if constexpr (N >= 3) {
				this->b = qpl::f32_cast(other[2]);
			}
			else {
				this->b = 0.0f;
			}
			if constexpr (N >= 4) {
				this->a = qpl::f32_cast(other[3]);
			}
			else {
				this->a = 0.0f;
			}
			return *this;
		}

		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgba& operator/=(T value) {

			this->r /= value;
			this->g /= value;
			this->b /= value;
			this->a /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::frgba operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgba& operator*=(T value) {

			this->r *= qpl::f32_cast(value);
			this->g *= qpl::f32_cast(value);
			this->b *= qpl::f32_cast(value);
			this->a *= qpl::f32_cast(value);
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::frgba operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::frgba& operator+=(T value) {
			this->r += value;
			this->g += value;
			this->b += value;
			this->a += value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::frgba operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			qpl::frgba& operator-=(T value) {
			this->r -= value;
			this->g -= value;
			this->b -= value;
			this->a -= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::frgba operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL void brighten(qpl::f32 delta);
		QPLDLL qpl::frgba brightened(qpl::f32 delta) const;

		QPLDLL void darken(qpl::f32 delta);
		QPLDLL qpl::frgba darkened(qpl::f32 delta) const;

		QPLDLL void light(qpl::f32 delta);
		QPLDLL qpl::frgba lighted(qpl::f32 delta) const;

		QPLDLL void intensify(qpl::f32 delta);
		QPLDLL qpl::frgba intensified(qpl::f32 delta) const;

		QPLDLL void grayify(qpl::f32 delta);
		QPLDLL qpl::frgba grayified(qpl::f32 delta) const;

		QPLDLL void saturate(qpl::f32 delta);
		QPLDLL qpl::frgba saturated(qpl::f32 delta) const;

		QPLDLL void interpolate(qpl::frgba other, qpl::f32 delta);
		QPLDLL qpl::frgba interpolated(qpl::frgba other, qpl::f32 delta) const;

		QPLDLL constexpr static qpl::frgba interpolation(const std::vector<qpl::frgba>& colors, qpl::f64 strength);
		QPLDLL constexpr static qpl::frgba interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength);

		QPLDLL std::string string(bool rgb = false) const;

		QPLDLL constexpr qpl::frgba& operator*=(const qpl::frgba& other);
		QPLDLL constexpr qpl::frgba operator*(const qpl::frgba& other) const;
		QPLDLL constexpr qpl::frgba& operator/=(const qpl::frgba& other);
		QPLDLL constexpr qpl::frgba operator/(const qpl::frgba& other) const;
		QPLDLL constexpr qpl::frgba& operator-=(const qpl::frgba& other);
		QPLDLL constexpr qpl::frgba operator-(const qpl::frgba& other) const;
		QPLDLL constexpr qpl::frgba& operator+=(const qpl::frgba& other);
		QPLDLL constexpr qpl::frgba operator+(const qpl::frgba& other) const;
		QPLDLL constexpr bool operator==(const qpl::frgba& other) const;
		QPLDLL constexpr bool operator!=(const qpl::frgba& other) const;


		QPLDLL friend std::ostream& operator<<(std::ostream& os, const qpl::frgba& color);
		
		const static qpl::frgba red;
		const static qpl::frgba green;
		const static qpl::frgba blue;
		const static qpl::frgba yellow;
		const static qpl::frgba orange;
		const static qpl::frgba cyan;
		const static qpl::frgba brown;
		const static qpl::frgba magenta;
		const static qpl::frgba white;
		const static qpl::frgba grey;
		const static qpl::frgba black;
		const static qpl::frgba transparent;
		const static qpl::frgba unset;
	};
	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::frgba& color);

	struct frgb {
		qpl::f32 r = 0.0;
		qpl::f32 g = 0.0;
		qpl::f32 b = 0.0;

		constexpr frgb() : r(), g(), b() {

		}
		constexpr frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b) : r(r), g(g), b(b) {

		}
		constexpr frgb(const qpl::frgba& other) {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
		}
		constexpr frgb(const qpl::rgb& other);
		constexpr frgb(const qpl::rgba& other);

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr frgb(const qpl::vectorN<T, N>& other) : r(), g(), b() {
			*this = other;
		}

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr qpl::frgb& operator=(const qpl::vectorN<T, N>& other) {
			this->r = qpl::f32_cast(other[0]);

			if constexpr (N >= 2) {
				this->g = qpl::f32_cast(other[1]);
			}
			else {
				this->g = 0.0f;
			}
			if constexpr (N >= 3) {
				this->b = qpl::f32_cast(other[2]);
			}
			else {
				this->b = 0.0f;
			}
			return *this;
		}

		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb& operator/=(T value) {

			this->r /= value;
			this->g /= value;
			this->b /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb& operator*=(T value) {

			this->r *= qpl::f32_cast(value);
			this->g *= qpl::f32_cast(value);
			this->b *= qpl::f32_cast(value);
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb& operator+=(T value) {
			this->r += value;
			this->g += value;
			this->b += value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		qpl::frgb& operator-=(T value) {
			this->r -= value;
			this->g -= value;
			this->b -= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL void brighten(qpl::f32 delta);
		QPLDLL qpl::frgb brightened(qpl::f32 delta) const;

		QPLDLL void darken(qpl::f32 delta);
		QPLDLL qpl::frgb darkened(qpl::f32 delta) const;

		QPLDLL void light(qpl::f32 delta);
		QPLDLL qpl::frgb lighted(qpl::f32 delta) const;

		QPLDLL void intensify(qpl::f32 delta);
		QPLDLL qpl::frgb intensified(qpl::f32 delta) const;

		QPLDLL void grayify(qpl::f32 delta);
		QPLDLL qpl::frgb grayified(qpl::f32 delta) const;

		QPLDLL void saturate(qpl::f32 delta);
		QPLDLL qpl::frgb saturated(qpl::f32 delta) const;

		QPLDLL void interpolate(qpl::frgb other, qpl::f32 delta);
		QPLDLL qpl::frgb interpolated(qpl::frgb other, qpl::f32 delta) const;

		QPLDLL constexpr static qpl::frgb interpolation(const std::vector<qpl::frgb>& colors, qpl::f64 strength);
		QPLDLL constexpr static qpl::frgb interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength);

		QPLDLL std::string string(bool rgb = false) const;

		QPLDLL constexpr qpl::frgb& operator*=(const qpl::frgb& other);
		QPLDLL constexpr qpl::frgb operator*(const qpl::frgb& other) const;
		QPLDLL constexpr qpl::frgb& operator/=(const qpl::frgb& other);
		QPLDLL constexpr qpl::frgb operator/(const qpl::frgb& other) const;
		QPLDLL constexpr qpl::frgb& operator-=(const qpl::frgb& other);
		QPLDLL constexpr qpl::frgb operator-(const qpl::frgb& other) const;
		QPLDLL constexpr qpl::frgb& operator+=(const qpl::frgb& other);
		QPLDLL constexpr qpl::frgb operator+(const qpl::frgb& other) const;
		QPLDLL constexpr bool operator==(const qpl::frgb& other) const;
		QPLDLL constexpr bool operator!=(const qpl::frgb& other) const;

		QPLDLL friend std::ostream& operator<<(std::ostream& os, const qpl::frgb& color);

		const static qpl::frgb red;
		const static qpl::frgb green;
		const static qpl::frgb blue;
		const static qpl::frgb yellow;
		const static qpl::frgb orange;
		const static qpl::frgb cyan;
		const static qpl::frgb brown;
		const static qpl::frgb magenta;
		const static qpl::frgb white;
		const static qpl::frgb grey;
		const static qpl::frgb black;
	};
	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::frgb& color);


	struct rgba {
		constexpr rgba() {
			*this = qpl::rgba::white;
		}
		constexpr rgba(qpl::u8 r, qpl::u8 g, qpl::u8 b, qpl::u8 a = qpl::u8_max) : r(r), g(g), b(b), a(a) {

		}
		constexpr rgba(qpl::u32 uint) : r(), g(), b(), a() {
			if (uint <= (qpl::u32_max >> 8)) {
				uint <<= 8;
				uint |= 0xFFu;
			}

			this->r = (uint & 0xFF000000u) >> 24;
			this->g = (uint & 0x00FF0000u) >> 16;
			this->b = (uint & 0x0000FF00u) >> 8;
			this->a = (uint & 0x000000FFu);
		}
		constexpr rgba(const rgba& other) : r(), g(), b(), a() {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
			this->a = other.a;
		}
		constexpr rgba(const rgb& other);
		constexpr rgba(const frgb& other) : r(), g(), b(), a() {
			this->r = qpl::u8_cast(qpl::clamp(0.0f, other.r * 255, 255.0f));
			this->g = qpl::u8_cast(qpl::clamp(0.0f, other.g * 255, 255.0f));
			this->b = qpl::u8_cast(qpl::clamp(0.0f, other.b * 255, 255.0f));
			this->a = 255u;
		}
		constexpr rgba(const frgba& other) : r(), g(), b(), a() {
			this->r = qpl::u8_cast(qpl::clamp(0.0f, other.r * 255, 255.0f));
			this->g = qpl::u8_cast(qpl::clamp(0.0f, other.g * 255, 255.0f));
			this->b = qpl::u8_cast(qpl::clamp(0.0f, other.b * 255, 255.0f));
			this->a = qpl::u8_cast(qpl::clamp(0.0f, other.a * 255, 255.0f));
		}

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr rgba(const qpl::vectorN<T, N>& other) {
			*this = other;
		}

		qpl::u8 r = 0;
		qpl::u8 g = 0;
		qpl::u8 b = 0;
		qpl::u8 a = 0;

#if defined QPL_INTERN_SFML_USE
		rgba(sf::Color color) {
			*this = color;
		}
		QPLDLL qpl::rgba& operator=(sf::Color color);
		QPLDLL operator sf::Color() const;
#endif

		QPLDLL constexpr qpl::rgba& operator=(qpl::u32 uint);
		QPLDLL constexpr qpl::rgba& operator=(const qpl::rgb& rgb);
		QPLDLL constexpr qpl::rgba& operator=(const qpl::frgb& rgb);
		QPLDLL constexpr qpl::rgba& operator=(const qpl::frgba& rgb);
		QPLDLL constexpr bool operator==(const rgba& other) const;

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr qpl::rgba& operator=(const qpl::vectorN<T, N>& other) {
			this->r = other[0];

			if constexpr (N >= 2) {
				this->g = other[1];
			}
			else {
				this->g = 0u;
			}
			if constexpr (N >= 3) {
				this->b = other[2];
			}
			else {
				this->b = 0u;
			}
			if constexpr (N >= 4) {
				this->a = other[3];
			}
			else {
				this->a = 0u;
			}
			return *this;
		}

		QPLDLL std::string string() const;
		QPLDLL std::string hex_string(bool prefix = true) const;
		QPLDLL constexpr bool is_unset() const;
		QPLDLL constexpr qpl::u32 uint() const;

		QPLDLL void brighten(qpl::f64 delta);
		QPLDLL qpl::rgba brightened(qpl::f64 delta) const;

		QPLDLL void darken(qpl::f64 delta);
		QPLDLL qpl::rgba darkened(qpl::f64 delta) const;

		QPLDLL void light(qpl::f64 delta);
		QPLDLL qpl::rgba lighted(qpl::f64 delta) const;

		QPLDLL void intensify(qpl::f64 delta);
		QPLDLL qpl::rgba intensified(qpl::f64 delta) const;

		QPLDLL void grayify(qpl::f64 delta);
		QPLDLL qpl::rgba grayified(qpl::f64 delta) const;

		QPLDLL void saturate(qpl::f64 delta);
		QPLDLL qpl::rgba saturated(qpl::f64 delta) const;

		QPLDLL constexpr qpl::rgba& interpolate(qpl::rgba color, qpl::f64 strength = qpl::f64{ 1 });
		QPLDLL constexpr qpl::rgba interpolated(qpl::rgba color, qpl::f64 strength = qpl::f64{ 1 }) const;

		QPLDLL constexpr static qpl::rgba interpolation(const std::vector<qpl::rgba>& colors, qpl::f64 strength);
		template<typename T> requires (qpl::is_container<T>())
		constexpr static qpl::rgba interpolation(const T& colors, qpl::f64 strength) {
			strength = qpl::clamp(0.0, strength, 1.0);
			if (strength == 1.0) {
				return colors.back();
			}

			auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
			auto left_over = ((colors.size() - 1) * strength) - index;

			return colors[index].interpolated(colors[index + 1], left_over);
		}
		QPLDLL constexpr static qpl::rgba grey_shade(qpl::u8 strength = 128) {
			return qpl::rgba(strength, strength, strength);
		}

		QPLDLL constexpr qpl::rgba& invert();
		QPLDLL constexpr qpl::rgba inverted() const;
		QPLDLL constexpr qpl::rgba with_alpha(qpl::u8 alpha) const;
		QPLDLL constexpr qpl::rgba multiplied_alpha(qpl::u8 alpha) const;
		QPLDLL constexpr qpl::rgba multiplied_color(qpl::rgba color) const;

		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba& operator/=(T value) {
			this->r /= value;
			this->g /= value;
			this->b /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba& operator*=(T value) {
			this->r *= value;
			this->g *= value;
			this->b *= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba& operator+=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r + value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g + value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b + value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba& operator-=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r - value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g - value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b - value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::rgba operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL constexpr qpl::rgba& operator*=(qpl::rgba other);
		QPLDLL constexpr qpl::rgba operator*(qpl::rgba other) const;
		QPLDLL constexpr qpl::rgba& operator/=(qpl::rgba other);
		QPLDLL constexpr qpl::rgba operator/(qpl::rgba other) const;
		QPLDLL constexpr qpl::rgba& operator-=(qpl::rgba other);
		QPLDLL constexpr qpl::rgba operator-(qpl::rgba other) const;
		QPLDLL constexpr qpl::rgba& operator+=(qpl::rgba other);
		QPLDLL constexpr qpl::rgba operator+(qpl::rgba other) const;

		QPLDLL friend std::ostream& operator<<(std::ostream& os, const qpl::rgba& color);

		const static qpl::rgba red;
		const static qpl::rgba green;
		const static qpl::rgba blue;
		const static qpl::rgba yellow;
		const static qpl::rgba orange;
		const static qpl::rgba brown;
		const static qpl::rgba cyan;
		const static qpl::rgba magenta;
		const static qpl::rgba white;
		const static qpl::rgba grey;
		const static qpl::rgba black;
		const static qpl::rgba transparent;
		const static qpl::rgba unset;
	};

	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::rgba& color);

	struct rgb {
		constexpr rgb() {
			*this = qpl::rgb::white;
		}
		constexpr rgb(qpl::u8 r, qpl::u8 g, qpl::u8 b) : r(r), g(g), b(b) {

		}
		constexpr rgb(qpl::u32 uint) : r(), g(), b()  {
			if (uint <= (qpl::u32_max >> 8)) {
				uint <<= 8;
				uint |= 0xFFu;
			}

			this->r = (uint & 0x00FF0000u) >> 16;
			this->g = (uint & 0x0000FF00u) >> 8;
			this->b = (uint & 0x000000FFu);
		}
		constexpr rgb(const rgb& other) : r(), g(), b() {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
		}
		constexpr rgb(const rgba& other) : r(), g(), b() {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
		}
		constexpr rgb(const frgb& other) : r(), g(), b() {
			this->r = qpl::u8_cast(qpl::clamp(0.0f, other.r * 255, 255.0f));
			this->g = qpl::u8_cast(qpl::clamp(0.0f, other.g * 255, 255.0f));
			this->b = qpl::u8_cast(qpl::clamp(0.0f, other.b * 255, 255.0f));
		}
		constexpr rgb(const frgba& other) : r(), g(), b() {
			this->r = qpl::u8_cast(qpl::clamp(0.0f, other.r * 255, 255.0f));
			this->g = qpl::u8_cast(qpl::clamp(0.0f, other.g * 255, 255.0f));
			this->b = qpl::u8_cast(qpl::clamp(0.0f, other.b * 255, 255.0f));
		}
		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
		constexpr qpl::rgb(const qpl::vectorN<T, N>& other) {
			*this = other;
		}

		qpl::u8 r = 0;
		qpl::u8 g = 0;
		qpl::u8 b = 0;

#if defined QPL_INTERN_SFML_USE
		rgb(sf::Color color) {
			*this = color;
		}
		QPLDLL qpl::rgb& operator=(sf::Color color);
		QPLDLL operator sf::Color() const;
#endif
		QPLDLL constexpr qpl::rgb& operator=(qpl::u32 uint);
		QPLDLL constexpr qpl::rgb& operator=(const qpl::rgba& rgb);
		QPLDLL constexpr qpl::rgb& operator=(const qpl::frgb& rgb);
		QPLDLL constexpr qpl::rgb& operator=(const qpl::frgba& rgb);

		template<typename T, qpl::size N> requires(qpl::is_arithmetic<T>())
			constexpr qpl::rgb& operator=(const qpl::vectorN<T, N>& other) {
			this->r = other[0];

			if constexpr (N >= 2) {
				this->g = other[1];
			}
			else {
				this->g = 0u;
			}
			if constexpr (N >= 3) {
				this->b = other[2];
			}
			else {
				this->b = 0u;
			}
			return *this;
		}


		QPLDLL constexpr bool operator==(const rgb& other) const;


		QPLDLL std::string string() const;
		QPLDLL std::string hex_string(bool prefix = true) const;
		QPLDLL constexpr qpl::u32 uint() const;

		QPLDLL void brighten(qpl::f64 delta);
		QPLDLL qpl::rgb brightened(qpl::f64 delta) const;

		QPLDLL void darken(qpl::f64 delta);
		QPLDLL qpl::rgb darkened(qpl::f64 delta) const;

		QPLDLL void light(qpl::f64 delta);
		QPLDLL qpl::rgb lighted(qpl::f64 delta) const;

		QPLDLL void intensify(qpl::f64 delta);
		QPLDLL qpl::rgb intensified(qpl::f64 delta) const;

		QPLDLL void grayify(qpl::f64 delta);
		QPLDLL qpl::rgb grayified(qpl::f64 delta) const;

		QPLDLL void saturate(qpl::f64 delta);
		QPLDLL qpl::rgb saturated(qpl::f64 delta) const;

		QPLDLL constexpr qpl::rgb& interpolate(qpl::rgb color, qpl::f64 strength = qpl::f64{ 1 });
		QPLDLL constexpr qpl::rgb interpolated(qpl::rgb color, qpl::f64 strength = qpl::f64{ 1 }) const;

		QPLDLL constexpr static qpl::rgb interpolation(const std::vector<qpl::rgb>& colors, qpl::f64 strength);
		template<typename T> requires (qpl::is_container<T>())
			constexpr static qpl::rgb interpolation(const T& colors, qpl::f64 strength) {
			strength = qpl::clamp(0.0, strength, 1.0);
			if (strength == 1.0) {
				return colors.back();
			}

			auto index = static_cast<qpl::u32>(strength * (colors.size() - 1));
			auto left_over = ((colors.size() - 1) * strength) - index;

			return colors[index].interpolated(colors[index + 1], left_over);
		}
		QPLDLL constexpr static qpl::rgb grey_shade(qpl::u8 strength = 128) {
			return qpl::rgb(strength, strength, strength);
		}

		QPLDLL constexpr qpl::rgb& invert();
		QPLDLL constexpr qpl::rgb inverted() const;
		QPLDLL constexpr qpl::rgb multiplied_color(qpl::rgb color) const;

		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb& operator/=(T value) {
			this->r /= value;
			this->g /= value;
			this->b /= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb operator/(T value) const {
			auto copy = *this;
			copy /= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb& operator*=(T value) {
			this->r *= value;
			this->g *= value;
			this->b *= value;
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb operator*(T value) const {
			auto copy = *this;
			copy *= value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb& operator+=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r + value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g + value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b + value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb operator+(T value) const {
			auto copy = *this;
			copy += value;
			return copy;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb& operator-=(T value) {
			this->r = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->r - value), static_cast<qpl::i16>(qpl::u8_max));
			this->g = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->g - value), static_cast<qpl::i16>(qpl::u8_max));
			this->b = qpl::clamp(qpl::i16{}, static_cast<qpl::i16>(this->b - value), static_cast<qpl::i16>(qpl::u8_max));
			return *this;
		}
		template<typename T> requires (qpl::is_arithmetic<T>())
			constexpr qpl::rgb operator-(T value) const {
			auto copy = *this;
			copy -= value;
			return copy;
		}

		QPLDLL constexpr qpl::rgb& operator*=(qpl::rgb other);
		QPLDLL constexpr qpl::rgb operator*(qpl::rgb other) const;
		QPLDLL constexpr qpl::rgb& operator/=(qpl::rgb other);
		QPLDLL constexpr qpl::rgb operator/(qpl::rgb other) const;
		QPLDLL constexpr qpl::rgb& operator-=(qpl::rgb other);
		QPLDLL constexpr qpl::rgb operator-(qpl::rgb other) const;
		QPLDLL constexpr qpl::rgb& operator+=(qpl::rgb other);
		QPLDLL constexpr qpl::rgb operator+(qpl::rgb other) const;

		QPLDLL friend std::ostream& operator<<(std::ostream& os, const qpl::rgb& color);

		const static qpl::rgb red;
		const static qpl::rgb green;
		const static qpl::rgb blue;
		const static qpl::rgb yellow;
		const static qpl::rgb orange;
		const static qpl::rgb cyan;
		const static qpl::rgb brown;
		const static qpl::rgb magenta;
		const static qpl::rgb white;
		const static qpl::rgb grey;
		const static qpl::rgb black;
	};


	QPLDLL std::ostream& operator<<(std::ostream& os, const qpl::rgb& color);

	QPLDLL qpl::rgb get_random_color();
	QPLDLL qpl::rgba get_random_transparency_color();
	QPLDLL qpl::rgb get_rainbow_color(qpl::f64 f);
	QPLDLL qpl::rgb get_random_rainbow_color();



	constexpr frgba::frgba(const qpl::rgba& other) {
		this->r = other.r / 255.0f;
		this->g = other.g / 255.0f;
		this->b = other.b / 255.0f;
		this->a = other.a / 255.0f;
	}
	constexpr frgba::frgba(const qpl::rgb& other) {
		this->r = other.r / 255.0f;
		this->g = other.g / 255.0f;
		this->b = other.b / 255.0f;
		this->a = 1.0f;
	}
	constexpr frgba::frgba(const qpl::frgb& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
	}

	constexpr frgb::frgb(const qpl::rgba& other) : r(), g(), b() {
		this->r = other.r / 255.0f;
		this->g = other.g / 255.0f;
		this->b = other.b / 255.0f;
	}
	constexpr frgb::frgb(const qpl::rgb& other) : r(), g(), b() {
		this->r = other.r / 255.0f;
		this->g = other.g / 255.0f;
		this->b = other.b / 255.0f;
	}

	constexpr rgba::rgba(const rgb& other) : r(), g(), b(), a() {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = 255u;
	}
}


#if defined QPL_INTERN_VULKAN_USE
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <string>

#include <glm/glm.hpp>

namespace qpl::vk {
	class frgb {
	public:
		constexpr frgb() noexcept : m_rgb(0, 0, 0) {

		}
		constexpr frgb(const frgb& other) noexcept : m_rgb(other.m_rgb) {

		}
		constexpr frgb(const glm::vec3& other) noexcept : m_rgb(other) {

		}
		constexpr frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b) noexcept : m_rgb(r, g, b) {

		}
		QPLDLL frgb& operator=(const qpl::frgb& other);
		QPLDLL frgb& operator=(const glm::vec3& other);
		QPLDLL bool operator==(const frgb& other) const;
		QPLDLL bool operator==(const glm::vec3& other) const;

		QPLDLL operator glm::vec3() const;
		QPLDLL glm::vec3 get() const;

		QPLDLL std::string string() const;


		QPLDLL void brighten(qpl::f32 delta);
		QPLDLL qpl::vk::frgb brightened(qpl::f32 delta) const;

		QPLDLL void darken(qpl::f32 delta);
		QPLDLL qpl::vk::frgb darkened(qpl::f32 delta) const;

		QPLDLL void light(qpl::f32 delta);
		QPLDLL qpl::vk::frgb lighted(qpl::f32 delta) const;

		QPLDLL void intensify(qpl::f32 delta);
		QPLDLL qpl::vk::frgb intensified(qpl::f32 delta) const;

		QPLDLL void grayify(qpl::f32 delta);
		QPLDLL qpl::vk::frgb grayified(qpl::f32 delta) const;

		QPLDLL void saturate(qpl::f32 delta);
		QPLDLL qpl::vk::frgb saturated(qpl::f32 delta) const;


		QPLDLL void black_gray_intense_white(qpl::f32 delta);
		QPLDLL qpl::vk::frgb black_gray_intense_whited(qpl::f32 delta) const;

		QPLDLL void black_intense_white(qpl::f32 delta);
		QPLDLL qpl::vk::frgb black_intense_whited(qpl::f32 delta) const;

		QPLDLL void interpolate(qpl::frgb other, qpl::f32 delta);
		QPLDLL qpl::vk::frgb interpolated(qpl::frgb other, qpl::f32 delta) const;

		const static qpl::frgb red;
		const static qpl::frgb green;
		const static qpl::frgb blue;
		const static qpl::frgb black;
		const static qpl::frgb white;
		const static qpl::frgb yellow;
		const static qpl::frgb magenta;
		const static qpl::frgb cyan;

		const static qpl::frgb orange;
		const static qpl::frgb purple;
		const static qpl::frgb amaranth;
		const static qpl::frgb turquoise;
	private:
		glm::vec3 m_rgb;
	};

	QPLDLL qpl::vk::frgb random_color();


	inline constexpr qpl::vk::frgb qpl::vk::frgb::red = qpl::vk::frgb(1.0f, 0.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::green = qpl::vk::frgb(0.0f, 1.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::blue = qpl::vk::frgb(0.5f, 0.5f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::black = qpl::vk::frgb(0.0f, 0.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::white = qpl::vk::frgb(1.0f, 1.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::yellow = qpl::vk::frgb(1.0f, 1.0f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::magenta = qpl::vk::frgb(1.0f, 0.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::cyan = qpl::vk::frgb(0.0f, 1.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::orange = qpl::vk::frgb(1.0f, 0.5f, 0.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::purple = qpl::vk::frgb(0.5f, 0.0f, 1.0f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::amaranth = qpl::vk::frgb(1.0f, 0.0f, 0.5f);
	inline constexpr qpl::vk::frgb qpl::vk::frgb::turquoise = qpl::vk::frgb(0.0f, 1.0f, 0.5f);
}
#endif


namespace std {
	template <>
	struct hash<qpl::rgba> {
		qpl::u32 operator()(const qpl::rgba& k) const {
			return k.uint();
		}
	};
}



inline constexpr qpl::frgba qpl::frgba::red         = qpl::frgba(1.0f, 0.0f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::green       = qpl::frgba(0.0f, 1.0f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::blue        = qpl::frgba(0.0f, 0.0f, 1.0f);
inline constexpr qpl::frgba qpl::frgba::yellow      = qpl::frgba(1.0f, 1.0f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::orange      = qpl::frgba(1.0f, 0.5f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::cyan        = qpl::frgba(0.0f, 1.0f, 1.0f);
inline constexpr qpl::frgba qpl::frgba::brown       = qpl::frgba(0.5f, 0.25f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::magenta     = qpl::frgba(1.0f, 0.0f, 1.0f);
inline constexpr qpl::frgba qpl::frgba::white       = qpl::frgba(1.0f, 1.0f, 1.0f);
inline constexpr qpl::frgba qpl::frgba::grey        = qpl::frgba(0.5f, 0.5f, 0.5f);
inline constexpr qpl::frgba qpl::frgba::black       = qpl::frgba(0.0f, 0.0f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::transparent = qpl::frgba(1.0f, 1.0f, 1.0f, 0.0f);
inline constexpr qpl::frgba qpl::frgba::unset       = qpl::frgba(-1.0f, -1.0f, -1.0f, -1.0f);

inline constexpr qpl::frgb qpl::frgb::red         = qpl::frgb(1.0f, 0.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::green       = qpl::frgb(0.0f, 1.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::blue        = qpl::frgb(0.0f, 0.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::yellow      = qpl::frgb(1.0f, 1.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::orange      = qpl::frgb(1.0f, 0.5f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::cyan        = qpl::frgb(0.0f, 1.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::brown       = qpl::frgb(0.5f, 0.25f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::magenta     = qpl::frgb(1.0f, 0.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::white       = qpl::frgb(1.0f, 1.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::grey        = qpl::frgb(0.5f, 0.5f, 0.5f);
inline constexpr qpl::frgb qpl::frgb::black       = qpl::frgb(0.0f, 0.0f, 0.0f);



inline constexpr qpl::rgba qpl::rgba::red = qpl::rgba(255, 0, 0);
inline constexpr qpl::rgba qpl::rgba::green = qpl::rgba(0, 255, 0);
inline constexpr qpl::rgba qpl::rgba::blue = qpl::rgba(0, 0, 255);
inline constexpr qpl::rgba qpl::rgba::yellow = qpl::rgba(255, 255, 0);
inline constexpr qpl::rgba qpl::rgba::orange = qpl::rgba(255, 127, 0);
inline constexpr qpl::rgba qpl::rgba::brown = qpl::rgba(150, 77, 0);
inline constexpr qpl::rgba qpl::rgba::cyan = qpl::rgba(0, 255, 255);
inline constexpr qpl::rgba qpl::rgba::magenta = qpl::rgba(255, 0, 255);
inline constexpr qpl::rgba qpl::rgba::white = qpl::rgba(255, 255, 255);
inline constexpr qpl::rgba qpl::rgba::grey = qpl::rgba(127, 127, 127);
inline constexpr qpl::rgba qpl::rgba::black = qpl::rgba(0, 0, 0);
inline constexpr qpl::rgba qpl::rgba::transparent = qpl::rgba(255, 255, 255, 0);
inline constexpr qpl::rgba qpl::rgba::unset = qpl::rgba(0, 0, 0, 0);

inline constexpr qpl::rgb qpl::rgb::red = qpl::rgba::red;
inline constexpr qpl::rgb qpl::rgb::green = qpl::rgba::green;
inline constexpr qpl::rgb qpl::rgb::blue = qpl::rgba::blue;
inline constexpr qpl::rgb qpl::rgb::yellow = qpl::rgba::yellow;
inline constexpr qpl::rgb qpl::rgb::orange = qpl::rgba::orange;
inline constexpr qpl::rgb qpl::rgb::cyan = qpl::rgba::cyan;
inline constexpr qpl::rgb qpl::rgb::brown = qpl::rgba::brown;
inline constexpr qpl::rgb qpl::rgb::magenta = qpl::rgba::magenta;
inline constexpr qpl::rgb qpl::rgb::white = qpl::rgba::white;
inline constexpr qpl::rgb qpl::rgb::grey = qpl::rgba::grey;
inline constexpr qpl::rgb qpl::rgb::black = qpl::rgba::black;




#endif