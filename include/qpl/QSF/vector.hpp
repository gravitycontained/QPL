#ifndef QPLSF_VECTOR_HPP
#define QPLSF_VECTOR_HPP
#pragma once

#ifdef QPL_USE_SFML || QPL_USE_ALL
#include <SFML/Graphics.hpp>
#include <qpl/vardef.hpp>
#include <qpl/string.hpp>
#include <initializer_list>
#include <iostream>
#include <cmath>

namespace qsf {
	template<typename T>
	struct vector2 {
		T x;
		T y;

		vector2() {
			this->x = this->y = T{};
		}
		template<typename U>
		vector2(const std::initializer_list<U>& list) {
			*this = list;
		}
		template<typename U>
		vector2(const vector2<U>& other) {
			*this = other;
		}
		template<typename U>
		vector2(const sf::Vector2<U>& other) {
			*this = other;
		}
		template<typename U, typename V>
		vector2(U u, V v) {
			this->x = static_cast<T>(u);
			this->y = static_cast<T>(v);
		}
		template<typename U>
		vector2(U u, U v = U{}) {
			this->x = static_cast<T>(u);
			this->y = static_cast<T>(v);
		}
		template<typename U>
		operator sf::Vector2<U>() const {
			return sf::Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
		}

		vector2<T> xx() const {
			return vector2<T>(this->x, this->x);
		}
		vector2<T> yy() const {
			return vector2<T>(this->y, this->y);
		}
		vector2<T> just_x() const {
			return vector2<T>(this->x, T{});
		}
		vector2<T> just_y() const {
			return vector2<T>(T{}, this->y);
		}

		T normal() const {
			return static_cast<T>(std::sqrt(this->x * this->x + this->y * this->y));
		}

		template<typename U>
		vector2<U> t() const {
			return qsf::vector2<U>(*this);
		}

		template<typename U, typename V>
		void move(U x, V y) {
			this->x = static_cast<T>(this->x + x);
			this->y = static_cast<T>(this->y + y);
		}
		template<typename U>
		void move(qsf::vector2<U> position) {
			this->x = static_cast<T>(this->x + position.x);
			this->y = static_cast<T>(this->y + position.y);
		}

		template<typename U, typename V>
		qsf::vector2<T> moved(U x, V y) const {
			auto copy = *this;
			copy.move(x, y);
			return copy;
		}
		template<typename U>
		qsf::vector2<T> moved(qsf::vector2<U> position) const {
			auto copy = *this;
			copy.move(position);
			return copy;
		}

		template<typename U>
		bool operator==(const vector2<U>& other) const {
			return this->x == static_cast<T>(other.x) && this->y == static_cast<T>(other.y);
		}
		template<typename U>
		bool operator!=(const vector2<U>& other) const {
			return this->x != static_cast<T>(other.x) || this->y != static_cast<T>(other.y);
		}
		template<typename U>
		vector2<T>& operator=(const std::initializer_list<U>& list) {
			if (list.size() == 1) {
				this->x = this->y = static_cast<T>(*list.begin());
			}
			else if (list.size() == 2) {
				this->x = static_cast<T>(*list.begin());
				this->y = static_cast<T>(*(list.begin() + 1));
			}
			return *this;
		}
		template<typename U>
		vector2<T>& operator=(const vector2<U>& other) {
			this->x = static_cast<T>(other.x);
			this->y = static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		vector2<T>& operator=(const sf::Vector2<U>& vec) {
			this->x = static_cast<T>(vec.x);
			this->y = static_cast<T>(vec.y);
			return *this;
		}
		bool operator==(const vector2<T>& other) const {
			return this->x == other.x && this->y == other.y;
		}

		template<typename U>
		vector2<T>& operator+=(const vector2<U>& other) {
			this->x += static_cast<T>(other.x);
			this->y += static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		vector2<T> operator+(const vector2<U>& other) const {
			auto copy = *this;
			copy += other;
			return copy;
		}
		template<typename U>
		vector2<T>& operator+=(U u) {
			this->x += static_cast<T>(u);
			this->y += static_cast<T>(u);
			return *this;
		}
		template<typename U>
		vector2<T> operator+(U u) const {
			auto copy = *this;
			copy += u;
			return copy;
		}

		template<typename U>
		vector2<T>& operator-=(const vector2<U>& other) {
			this->x -= static_cast<T>(other.x);
			this->y -= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		vector2<T> operator-(const vector2<U>& other) const {
			auto copy = *this;
			copy -= other;
			return copy;
		}
		template<typename U>
		vector2<T>& operator-=(U u) {
			this->x -= static_cast<T>(u);
			this->y -= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		vector2<T> operator-(U u) const {
			auto copy = *this;
			copy -= u;
			return copy;
		}

		template<typename U>
		vector2<T>& operator/=(const vector2<U>& other) {
			this->x /= static_cast<T>(other.x);
			this->y /= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		vector2<T> operator/(const vector2<U>& other) const {
			auto copy = *this;
			copy /= other;
			return copy;
		}
		template<typename U>
		vector2<T>& operator/=(U u) {
			this->x /= static_cast<T>(u);
			this->y /= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		vector2<T> operator/(U u) const {
			auto copy = *this;
			copy /= u;
			return copy;
		}

		template<typename U>
		vector2<T>& operator*=(const vector2<U>& other) {
			this->x *= static_cast<T>(other.x);
			this->y *= static_cast<T>(other.y);
			return *this;
		}
		template<typename U>
		vector2<T> operator*(const vector2<U>& other) const {
			auto copy = *this;
			copy *= other;
			return copy;
		}
		template<typename U>
		vector2<T>& operator*=(U u) {
			this->x *= static_cast<T>(u);
			this->y *= static_cast<T>(u);
			return *this;
		}
		template<typename U>
		vector2<T> operator*(U u) const {
			auto copy = *this;
			copy *= u;
			return copy;
		}
		std::string string() const {
			return qpl::to_string('(', this->x, ", ", this->y, ')');
		}
		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const qsf::vector2<T>& vec);
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const qsf::vector2<T>& vec) {
		return os << vec.string();
	}

	using vector2f = qsf::vector2<qpl::f32>;
	using vector2d = qsf::vector2<qpl::f64>;
	using vector2i = qsf::vector2<qpl::i32>;
	using vector2u = qsf::vector2<qpl::u32>;


}

#endif
#endif