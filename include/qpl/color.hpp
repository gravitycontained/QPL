#ifndef QPL_COLOR_HPP
#define QPL_COLOR_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
#include <SFML/Graphics.hpp>
#endif

#include <qpl/qpldeclspec.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/algorithm.hpp>
#include <string>
#include <qpl/vardef.hpp>

namespace qpl {

	struct rgb;
	struct frgb {
		qpl::f32 r = 0.0;
		qpl::f32 g = 0.0;
		qpl::f32 b = 0.0;
		qpl::f32 a = 1.0;

		constexpr frgb() {

		}
		constexpr frgb(qpl::f32 r, qpl::f32 g, qpl::f32 b, qpl::f32 a = 1.0f) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		constexpr frgb(const rgb& other) {
			*this = other;
		}
		constexpr frgb(const frgb& other) {
			*this = other;
		}


		QPLDLL constexpr frgb& operator=(const frgb& other);
		QPLDLL constexpr frgb& operator=(const rgb& other);

		template<typename T> requires (qpl::is_arithmetic<T>())
		constexpr qpl::frgb& operator/=(T value) {

			this->r /= value;
			this->g /= value;
			this->b /= value;
			this->a /= value;
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
			this->a *= qpl::f32_cast(value);
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
			this->a += value;
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
			this->a -= value;
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
		QPLDLL constexpr static qpl::frgb interpolation(const std::vector<qpl::rgb>& colors, qpl::f64 strength);

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
		
		const static qpl::frgb red;
		const static qpl::frgb green;
		const static qpl::frgb blue;
		const static qpl::frgb yellow;
		const static qpl::frgb orange;
		const static qpl::frgb cyan;
		const static qpl::frgb magenta;
		const static qpl::frgb white;
		const static qpl::frgb grey;
		const static qpl::frgb black;
		const static qpl::frgb transparent;
		const static qpl::frgb unset;
	};

	struct rgb {
		constexpr rgb() {
			*this = rgb::white;
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

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
		rgb(sf::Color color) {
			*this = color;
		}
		QPLDLL qpl::rgb& operator=(sf::Color color);
		QPLDLL operator sf::Color() const;
#endif
		QPLDLL constexpr qpl::rgb& operator=(const rgb& other);
		QPLDLL constexpr qpl::rgb& operator=(const frgb& other);
		QPLDLL constexpr qpl::rgb& operator=(qpl::u32 uint);
		QPLDLL constexpr bool operator==(const rgb& other) const;


		QPLDLL std::string string() const;
		QPLDLL std::string hex_string(bool prefix = true) const;
		QPLDLL constexpr bool is_unset() const;
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
		QPLDLL constexpr qpl::rgb with_alpha(qpl::u8 alpha) const;
		QPLDLL constexpr qpl::rgb multiplied_alpha(qpl::u8 alpha) const;
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

		const static qpl::rgb red;
		const static qpl::rgb green;
		const static qpl::rgb blue;
		const static qpl::rgb yellow;
		const static qpl::rgb orange;
		const static qpl::rgb cyan;
		const static qpl::rgb magenta;
		const static qpl::rgb white;
		const static qpl::rgb grey;
		const static qpl::rgb black;
		const static qpl::rgb transparent;
		const static qpl::rgb unset;
	};



	QPLDLL qpl::rgb get_random_color();
	QPLDLL qpl::rgb get_random_transparency_color();
	QPLDLL qpl::rgb get_rainbow_color(qpl::f64 f);
	QPLDLL qpl::rgb get_random_rainbow_color();
}


#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
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
	struct hash<qpl::rgb> {
		qpl::u32 operator()(const qpl::rgb& k) const {
			return k.uint();
		}
	};
}



inline constexpr qpl::frgb qpl::frgb::red = qpl::frgb(1.0f, 0.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::green = qpl::frgb(0.0f, 1.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::blue = qpl::frgb(0.0f, 0.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::yellow = qpl::frgb(1.0f, 1.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::orange = qpl::frgb(1.0f, 0.5f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::cyan = qpl::frgb(0.0f, 1.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::magenta = qpl::frgb(1.0f, 0.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::white = qpl::frgb(1.0f, 1.0f, 1.0f);
inline constexpr qpl::frgb qpl::frgb::grey = qpl::frgb(0.5f, 0.5f, 0.5f);
inline constexpr qpl::frgb qpl::frgb::black = qpl::frgb(0.0f, 0.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::transparent = qpl::frgb(1.0f, 1.0f, 1.0f, 0.0f);
inline constexpr qpl::frgb qpl::frgb::unset = qpl::frgb(-1.0f, -1.0f, -1.0f, -1.0f);


inline constexpr qpl::rgb qpl::rgb::red = qpl::rgb(255, 0, 0);
inline constexpr qpl::rgb qpl::rgb::green = qpl::rgb(0, 255, 0);
inline constexpr qpl::rgb qpl::rgb::blue = qpl::rgb(0, 0, 255);
inline constexpr qpl::rgb qpl::rgb::yellow = qpl::rgb(255, 255, 0);
inline constexpr qpl::rgb qpl::rgb::orange = qpl::rgb(255, 127, 0);
inline constexpr qpl::rgb qpl::rgb::cyan = qpl::rgb(0, 255, 255);
inline constexpr qpl::rgb qpl::rgb::magenta = qpl::rgb(255, 0, 255);
inline constexpr qpl::rgb qpl::rgb::white = qpl::rgb(255, 255, 255);
inline constexpr qpl::rgb qpl::rgb::grey = qpl::rgb(127, 127, 127);
inline constexpr qpl::rgb qpl::rgb::black = qpl::rgb(0, 0, 0);
inline constexpr qpl::rgb qpl::rgb::transparent = qpl::rgb(255, 255, 255, 0);
inline constexpr qpl::rgb qpl::rgb::unset = qpl::rgb(0, 0, 0, 0);




#endif