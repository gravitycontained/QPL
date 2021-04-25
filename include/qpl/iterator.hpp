#ifndef QPL_ITERATOR_HPP
#define QPL_ITERATOR_HPP
#pragma once

#include <qpl/vardef.hpp>
#include <qpl/qpldeclspec.hpp>

namespace qpl {
	template<typename T>
	struct iterator {
		T* ptr;

		constexpr iterator(T* ptr) {
			this->ptr = ptr;
		}
		constexpr iterator& operator++() {
			++this->ptr;
			return *this;
		}
		constexpr iterator operator++(int) {
			auto copy = *this;
			++this->ptr;
			return copy;
		}
		constexpr iterator& operator--() {
			--this->ptr;
			return *this;
		}
		constexpr iterator operator--(int) {
			auto copy = *this;
			--this->ptr;
			return copy;
		}
		constexpr iterator& operator+=(qpl::i64 delta) {
			this->ptr += delta;
			return *this;
		}		
		constexpr iterator operator+(qpl::i64 delta) const {
			auto copy = *this;
			copy += delta;
			return copy;
		}
		constexpr iterator& operator-=(qpl::i64 delta) {
			this->ptr -= delta;
			return *this;
		}
		constexpr iterator operator-(qpl::i64 delta) const {
			auto copy = *this;
			copy -= delta;
			return copy;
		}
		constexpr bool operator<(const iterator& other) const {
			return this->ptr < other.ptr;
		}
		constexpr bool operator>(const iterator& other) const {
			return this->ptr > other.ptr;
		}
		constexpr bool operator<=(const iterator& other) const {
			return this->ptr <= other.ptr;
		}
		constexpr bool operator>=(const iterator& other) const {
			return this->ptr >= other.ptr;
		}
		constexpr bool operator==(const iterator& other) const {
			return this->ptr == other.ptr;
		}
		constexpr bool operator!=(const iterator& other) const {
			return this->ptr != other.ptr;
		}
		constexpr T& operator*() {
			return *this->ptr;
		}
		constexpr const T& operator*() const {
			return *this->ptr;
		}
		constexpr T* operator->() {
			return this->ptr;
		}
		constexpr const T* operator->() const {
			return this->ptr;
		}
	};


	template<typename T>
	struct reverse_iterator {
		T* ptr;

		constexpr reverse_iterator(T* ptr) {
			this->ptr = ptr;
		}
		constexpr reverse_iterator& operator++() {
			--this->ptr;
			return *this;
		}
		constexpr reverse_iterator operator++(int) {
			auto copy = *this;
			--this->ptr;
			return copy;
		}
		constexpr reverse_iterator& operator--() {
			++this->ptr;
			return *this;
		}
		constexpr reverse_iterator operator--(int) {
			auto copy = *this;
			++this->ptr;
			return copy;
		}
		constexpr reverse_iterator& operator+=(qpl::i64 delta) {
			this->ptr -= delta;
			return *this;
		}
		constexpr reverse_iterator operator+(qpl::i64 delta) const {
			auto copy = *this;
			copy += delta;
			return copy;
		}
		constexpr reverse_iterator& operator-=(qpl::i64 delta) {
			this->ptr += delta;
			return *this;
		}
		constexpr reverse_iterator operator-(qpl::i64 delta) const {
			auto copy = *this;
			copy -= delta;
			return copy;
		}
		constexpr bool operator<(const reverse_iterator& other) const {
			return this->ptr > other.ptr;
		}
		constexpr bool operator>(const reverse_iterator& other) const {
			return this->ptr < other.ptr;
		}
		constexpr bool operator<=(const reverse_iterator& other) const {
			return this->ptr >= other.ptr;
		}
		constexpr bool operator>=(const reverse_iterator& other) const {
			return this->ptr <= other.ptr;
		}
		constexpr bool operator==(const reverse_iterator& other) const {
			return this->ptr == other.ptr;
		}
		constexpr bool operator!=(const reverse_iterator& other) const {
			return this->ptr != other.ptr;
		}
		constexpr T& operator*() {
			return *this->ptr;
		}
		constexpr const T& operator*() const {
			return *this->ptr;
		}
		constexpr T* operator->() {
			return this->ptr;
		}
		constexpr const T* operator->() const {
			return this->ptr;
		}
	};


	template<typename T>
	struct const_iterator {
		const T* ptr;

		constexpr const_iterator(const T* ptr) {
			this->ptr = ptr;
		}
		constexpr const_iterator& operator++() {
			++this->ptr;
			return *this;
		}
		constexpr const_iterator operator++(int) {
			auto copy = *this;
			++this->ptr;
			return copy;
		}
		constexpr const_iterator& operator--() {
			--this->ptr;
			return *this;
		}
		constexpr const_iterator operator--(int) {
			auto copy = *this;
			--this->ptr;
			return copy;
		}
		constexpr const_iterator& operator+=(qpl::i64 delta) {
			this->ptr += delta;
			return *this;
		}
		constexpr const_iterator operator+(qpl::i64 delta) const {
			auto copy = *this;
			copy += delta;
			return copy;
		}
		constexpr const_iterator& operator-=(qpl::i64 delta) {
			this->ptr -= delta;
			return *this;
		}
		constexpr const_iterator operator-(qpl::i64 delta) const {
			auto copy = *this;
			copy -= delta;
			return copy;
		}
		constexpr bool operator<(const const_iterator& other) const {
			return this->ptr < other.ptr;
		}
		constexpr bool operator>(const const_iterator& other) const {
			return this->ptr > other.ptr;
		}
		constexpr bool operator<=(const const_iterator& other) const {
			return this->ptr <= other.ptr;
		}
		constexpr bool operator>=(const const_iterator& other) const {
			return this->ptr >= other.ptr;
		}
		constexpr bool operator==(const const_iterator& other) const {
			return this->ptr == other.ptr;
		}
		constexpr bool operator!=(const const_iterator& other) const {
			return this->ptr != other.ptr;
		}
		constexpr const T& operator*() const {
			return *this->ptr;
		}
		constexpr T* operator->() {
			return this->ptr;
		}
		constexpr const T* operator->() const {
			return this->ptr;
		}
	};
	template<typename T>
	struct const_reverse_iterator {
		const T* ptr;

		constexpr const_reverse_iterator(const T* ptr) {
			this->ptr = ptr;
		}
		constexpr const_reverse_iterator& operator++() {
			--this->ptr;
			return *this;
		}
		constexpr const_reverse_iterator operator++(int) {
			auto copy = *this;
			--this->ptr;
			return copy;
		}
		constexpr const_reverse_iterator& operator--() {
			++this->ptr;
			return *this;
		}
		constexpr const_reverse_iterator operator--(int) {
			auto copy = *this;
			++this->ptr;
			return copy;
		}
		constexpr const_reverse_iterator& operator+=(qpl::i64 delta) {
			this->ptr -= delta;
			return *this;
		}
		constexpr const_reverse_iterator operator+(qpl::i64 delta) const {
			auto copy = *this;
			copy += delta;
			return copy;
		}
		constexpr const_reverse_iterator& operator-=(qpl::i64 delta) {
			this->ptr += delta;
			return *this;
		}
		constexpr const_reverse_iterator operator-(qpl::i64 delta) const {
			auto copy = *this;
			copy -= delta;
			return copy;
		}
		constexpr bool operator<(const const_reverse_iterator& other) const {
			return this->ptr > other.ptr;
		}
		constexpr bool operator>(const const_reverse_iterator& other) const {
			return this->ptr < other.ptr;
		}
		constexpr bool operator<=(const const_reverse_iterator& other) const {
			return this->ptr >= other.ptr;
		}
		constexpr bool operator>=(const const_reverse_iterator& other) const {
			return this->ptr <= other.ptr;
		}
		constexpr bool operator==(const const_reverse_iterator& other) const {
			return this->ptr == other.ptr;
		}
		constexpr bool operator!=(const const_reverse_iterator& other) const {
			return this->ptr != other.ptr;
		}
		constexpr const T& operator*() const {
			return *this->ptr;
		}
		constexpr T* operator->() {
			return this->ptr;
		}
		constexpr const T* operator->() const {
			return this->ptr;
		}
	};
}

#endif