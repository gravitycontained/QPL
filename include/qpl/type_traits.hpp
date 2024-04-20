#ifndef QPL_TYPETRAITS_HPP
#define QPL_TYPETRAITS_HPP
#pragma once

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#include <type_traits>
#include <limits>
#include <bit>
#include <cstdint>
#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <span>
#include <limits>
#include <utility>
#include <functional>
#include <typeinfo>
#include <charconv>
#include <iostream>

namespace qpl {


	struct empty_type {};

	struct true_type {};
	struct false_type {};
	struct default_type {};
	using error_type = std::enable_if<false, void>;
	using default_error = error_type;

	template<typename T>
	struct signal;

	template<typename T>
	constexpr auto declval() {
		using pointer = std::decay_t<T>*;
		return *(pointer{});
	}

	template<class T, class... Ignored>
	struct identity {
		using type = typename T;
	};

	template<typename T>
	struct decay_t {
		using type = std::decay<T>::type;
	};

	template<typename T>
	using decay = std::decay_t<T>;

	template<typename T>
	using full_decay = std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>;



	template<typename T, typename U>
	constexpr bool is_same() {
		return std::is_same_v<T, U>;
	}
	template<typename T, typename U>
	constexpr bool is_same_decayed() {
		return std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
	}

	template<typename T, typename U>
	concept is_same_decayed_c = (qpl::is_same_decayed<T, U>());


	template<typename T, typename U, typename... Args>
	constexpr bool all_equal() {
		return qpl::is_same<T, U>() && (qpl::is_same<T, Args>() && ...);
	}
	template<typename T, typename U, typename... Args>
	constexpr bool all_equal_decayed() {
		return qpl::is_same_decayed<T, U>() && (qpl::is_same_decayed<T, Args>() && ...);
	}

	template<class compare, class... Args>
	constexpr bool is_any_type_equal_to() {
		return (qpl::is_same<compare, Args>() || ...);
	}
	template<class compare, class... Args>
	constexpr bool is_any_type_decayed_equal_to() {
		return (qpl::is_same_decayed<compare, Args>() || ...);
	}
	template<class compare, class... Args>
	constexpr bool are_all_types_equal() {
		return (qpl::is_same<compare, Args>() && ...);
	}
	template<class compare, class... Args>
	constexpr bool are_all_types_unique() {
		if constexpr (sizeof...(Args) == 0) {
			return true;
		}
		else {
			return (!qpl::is_any_type_equal_to<compare, Args...>() && qpl::are_all_types_unique<Args...>());
		}
	}


	template<class... Args>
	struct empty_indirection {
		using type = typename identity<Args...>::type;
	};

	namespace detail {
		template<qpl::size Index, qpl::size Size>
		struct constexpr_index {
			constexpr operator qpl::size() { return Index; }
			constexpr static qpl::size size = Size;
			constexpr static qpl::size i = Index;
		};
	}

	template<qpl::size N, typename F>
	constexpr void constexpr_iterate(F&& function) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			(std::forward<F>(function)(qpl::detail::constexpr_index<Ints, N>{}), ...);
		};
		unpack(std::make_index_sequence<N>());
	}

	template<typename T>
	std::string type_name(T value) {
		return std::string(typeid(T).name());
	}
	template<typename T>
	std::string type_name() {
		return std::string(typeid(T).name());
	}


	constexpr qpl::size bits_in_byte() {
#ifdef CHAR_BIT
		return CHAR_BIT;
#else
		return 8u;
#endif
	}

	template<typename T>
	constexpr qpl::size bits_in_type() {
		return sizeof(T) * bits_in_byte();
	}

	template<typename T>
	constexpr qpl::size bytes_in_type() {
		return sizeof(T);
	}

	namespace detail {
		template<typename A, template<typename...> typename B> struct rename_variadic {};

		template<template<typename...> typename A, typename... T, template<typename...> typename B> struct rename_variadic<A<T...>, B> {
			using type = B<T...>;
		};
	}
	template<typename A, template<typename...> typename B>
	using rename_variadic_identity = detail::rename_variadic<A, B>;

	template<typename A, template<typename...> typename B>
	using rename_variadic = qpl::rename_variadic_identity<A, B>::type;


	template<typename T>
	concept is_read_container_c = requires(const T a) {
		a.cbegin();
		a.cend();
	};
	template<typename T>
	constexpr bool is_read_container() {
		return is_read_container_c<T>;
	}
	template<typename T>
	concept is_write_container_c = requires(T a) {
		a.begin();
		a.end();
	};
	template<typename T>
	constexpr bool is_write_container() {
		return is_write_container_c<T>;
	}
	template<typename T>
	concept is_container_c = is_read_container_c<T> || is_write_container_c<T>;

	template<typename T>
	constexpr bool is_container() {
		return is_container_c<T>;
	}

	template<typename T>
	concept is_vector_like_c = requires(T a, const T b) {
		requires is_container<T>();
		a.resize(0u);
		a.data();
		b.size();
	};
	template<typename T>
	constexpr bool is_vector_like() {
		return is_vector_like_c<T>;
	}

	template<typename C> requires (qpl::is_container<C>())
	using container_subtype = std::decay_t<decltype(*(qpl::declval<C>().begin()))>;

	template<typename C> requires (qpl::is_container<C>())
	using container_subtype_with_const = std::remove_reference_t<decltype(*(qpl::declval<C>().begin()))>;

	template<typename C> requires (qpl::is_container<C>())
	using container_subtype_with_reference = std::remove_const_t<decltype(*(qpl::declval<C>().begin()))>;


	template<class T>
	constexpr auto& access_member(T const& val) {
		auto& [member] = val;
		return member;
	}

	template<class T>
	constexpr auto& access_member(T& val) {
		auto& [member] = val;
		return member;
	}


	namespace detail {
		template<qpl::size>
		struct anything {
			template<class T> operator T() const;
		};
		template<class T, qpl::size... Ints>
		concept is_constructible_c = requires {
			T{ anything<Ints>{}... };
		};
	}

	template<class T, qpl::size N = 0u>
	constexpr auto is_constructible() {
		constexpr auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			return qpl::detail::is_constructible_c<T, Ints...>;
		};
		return unpack(std::make_index_sequence<N>{});
	}


	namespace detail {
		template<class T, qpl::size N = 0u, bool found = false>
		constexpr auto find_struct_members_count() {
			constexpr auto constructible = qpl::is_constructible<T, N>();

			if constexpr (found && !constructible) {
				return N - 1;
			}
			else if constexpr (constructible) {
				return find_struct_members_count<T, N + 1, true>();
			}
			else {
				return find_struct_members_count<T, N + 1, found>();
			}
		}
	}
	template<typename T>
	constexpr auto get_struct_members_count() {
		return qpl::detail::find_struct_members_count<T>();
	}

	namespace detail {
		template<class T, qpl::size N>
		constexpr auto get_struct_members_tuple(T& n) {
			if constexpr (N == 0) return std::tuple<>{};
			else if constexpr (N == 1) {
				auto& [v0] = n;
				return std::tie(v0);
			}
			else if constexpr (N == 2) {
				auto& [v0, v1] = n;
				return std::tie(v0, v1);
			}
			else if constexpr (N == 3) {
				auto& [v0, v1, v2] = n;
				return std::tie(v0, v1, v2);
			}
			else if constexpr (N == 4) {
				auto& [v0, v1, v2, v3] = n;
				return std::tie(v0, v1, v2, v3);
			}
			else if constexpr (N == 5) {
				auto& [v0, v1, v2, v3, v4] = n;
				return std::tie(v0, v1, v2, v3, v4);
			}
			else if constexpr (N == 6) {
				auto& [v0, v1, v2, v3, v4, v5] = n;
				return std::tie(v0, v1, v2, v3, v4, v5);
			}
			else if constexpr (N == 7) {
				auto& [v0, v1, v2, v3, v4, v5, v6] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6);
			}
			else if constexpr (N == 8) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7);
			}
			else if constexpr (N == 9) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8);
			}
			else if constexpr (N == 10) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
			}
			else if constexpr (N == 11) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va);
			}
			else if constexpr (N == 12) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb);
			}
			else if constexpr (N == 13) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc);
			}
			else if constexpr (N == 14) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd);
			}
			else if constexpr (N == 15) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve);
			}
			else if constexpr (N == 16) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf);
			}
			else if constexpr (N == 17) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg);
			}
			else if constexpr (N == 18) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh);
			}
			else if constexpr (N == 19) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi);
			}
			else if constexpr (N == 20) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj);
			}
			else if constexpr (N == 21) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk);
			}
			else if constexpr (N == 22) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl);
			}
			else if constexpr (N == 23) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm);
			}
			else if constexpr (N == 24) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn);
			}
			else if constexpr (N == 25) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo);
			}
			else if constexpr (N == 26) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp);
			}
			else if constexpr (N == 27) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq);
			}
			else if constexpr (N == 28) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr);
			}
			else if constexpr (N == 29) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs);
			}
			else if constexpr (N == 30) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt);
			}
			else if constexpr (N == 31) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu);
			}
			else if constexpr (N == 32) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv);
			}
			else if constexpr (N == 33) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw);
			}
			else if constexpr (N == 34) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx);
			}
			else if constexpr (N == 35) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy);
			}
			else if constexpr (N == 36) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz);
			}
			else if constexpr (N == 37) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10);
			}
			else if constexpr (N == 38) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11);
			}
			else if constexpr (N == 39) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12);
			}
			else if constexpr (N == 40) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13);
			}
			else if constexpr (N == 41) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14);
			}
			else if constexpr (N == 42) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15);
			}
			else if constexpr (N == 43) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16);
			}
			else if constexpr (N == 44) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17);
			}
			else if constexpr (N == 45) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18);
			}
			else if constexpr (N == 46) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19);
			}
			else if constexpr (N == 47) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a);
			}
			else if constexpr (N == 48) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b);
			}
			else if constexpr (N == 49) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c);
			}
			else if constexpr (N == 50) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d);
			}
			else if constexpr (N == 51) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e);
			}
			else if constexpr (N == 52) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f);
			}
			else if constexpr (N == 53) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g);
			}
			else if constexpr (N == 54) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h);
			}
			else if constexpr (N == 55) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i);
			}
			else if constexpr (N == 56) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j);
			}
			else if constexpr (N == 57) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k);
			}
			else if constexpr (N == 58) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l);
			}
			else if constexpr (N == 59) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m);
			}
			else if constexpr (N == 60) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n);
			}
			else if constexpr (N == 61) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o);
			}
			else if constexpr (N == 62) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p);
			}
			else if constexpr (N == 63) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q);
			}
			else if constexpr (N == 64) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r);
			}
			else if constexpr (N == 65) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s);
			}
			else if constexpr (N == 66) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t);
			}
			else if constexpr (N == 67) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u);
			}
			else if constexpr (N == 68) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v);
			}
			else if constexpr (N == 69) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w);
			}
			else if constexpr (N == 70) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x);
			}
			else if constexpr (N == 71) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y);
			}
			else if constexpr (N == 72) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z);
			}
			else if constexpr (N == 73) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20);
			}
			else if constexpr (N == 74) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21);
			}
			else if constexpr (N == 75) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22);
			}
			else if constexpr (N == 76) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23);
			}
			else if constexpr (N == 77) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24);
			}
			else if constexpr (N == 78) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25);
			}
			else if constexpr (N == 79) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26);
			}
			else if constexpr (N == 80) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27);
			}
			else if constexpr (N == 81) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28);
			}
			else if constexpr (N == 82) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29);
			}
			else if constexpr (N == 83) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a);
			}
			else if constexpr (N == 84) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b);
			}
			else if constexpr (N == 85) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c);
			}
			else if constexpr (N == 86) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d);
			}
			else if constexpr (N == 87) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e);
			}
			else if constexpr (N == 88) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f);
			}
			else if constexpr (N == 89) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g);
			}
			else if constexpr (N == 90) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h);
			}
			else if constexpr (N == 91) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i);
			}
			else if constexpr (N == 92) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j);
			}
			else if constexpr (N == 93) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k);
			}
			else if constexpr (N == 94) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l);
			}
			else if constexpr (N == 95) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m);
			}
			else if constexpr (N == 96) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n);
			}
			else if constexpr (N == 97) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o);
			}
			else if constexpr (N == 98) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p);
			}
			else if constexpr (N == 99) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q);
			}
			else if constexpr (N == 100) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r);
			}
			else if constexpr (N == 101) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s);
			}
			else if constexpr (N == 102) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t);
			}
			else if constexpr (N == 103) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u);
			}
			else if constexpr (N == 104) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v);
			}
			else if constexpr (N == 105) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w);
			}
			else if constexpr (N == 106) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x);
			}
			else if constexpr (N == 107) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y);
			}
			else if constexpr (N == 108) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z);
			}
			else if constexpr (N == 109) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30);
			}
			else if constexpr (N == 110) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31);
			}
			else if constexpr (N == 111) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32);
			}
			else if constexpr (N == 112) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33);
			}
			else if constexpr (N == 113) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34);
			}
			else if constexpr (N == 114) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35);
			}
			else if constexpr (N == 115) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36);
			}
			else if constexpr (N == 116) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37);
			}
			else if constexpr (N == 117) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38);
			}
			else if constexpr (N == 118) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39);
			}
			else if constexpr (N == 119) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a);
			}
			else if constexpr (N == 120) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b);
			}
			else if constexpr (N == 121) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c);
			}
			else if constexpr (N == 122) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d);
			}
			else if constexpr (N == 123) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e);
			}
			else if constexpr (N == 124) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f);
			}
			else if constexpr (N == 125) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g);
			}
			else if constexpr (N == 126) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g, v3h] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g, v3h);
			}
			else if constexpr (N == 127) {
				auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g, v3h, v3i] = n;
				return std::tie(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx, vy, vz, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v1a, v1b, v1c, v1d, v1e, v1f, v1g, v1h, v1i, v1j, v1k, v1l, v1m, v1n, v1o, v1p, v1q, v1r, v1s, v1t, v1u, v1v, v1w, v1x, v1y, v1z, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v2a, v2b, v2c, v2d, v2e, v2f, v2g, v2h, v2i, v2j, v2k, v2l, v2m, v2n, v2o, v2p, v2q, v2r, v2s, v2t, v2u, v2v, v2w, v2x, v2y, v2z, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v3a, v3b, v3c, v3d, v3e, v3f, v3g, v3h, v3i);
			}
			else {
				static_assert("struct member size too large.");
			}
		}
	}
	template<typename T>
	constexpr auto get_struct_members_tuple(T& value) {
		return qpl::detail::get_struct_members_tuple<T, qpl::get_struct_members_count<T>()>(value);
	}
	template<typename T>
	using get_struct_members_tuple_type = decltype(qpl::detail::get_struct_members_tuple<T, qpl::get_struct_members_count<T>()>(qpl::declval<T>()));

	namespace impl {
		template<typename... Ts>
		constexpr auto tuple_signature(std::tuple<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto tuple_signature(T) {
			return std::false_type{};
		}

		template<typename... Ts>
		constexpr auto pair_signature(std::pair<Ts...>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto pair_signature(T) {
			return std::false_type{};
		}

		template<typename T>
		constexpr auto signal_signature(qpl::signal<T>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto signal_signature(T) {
			return std::false_type{};
		}
	}

	template<typename T>
	constexpr bool is_tuple() {
		return decltype(qpl::impl::tuple_signature(qpl::declval<T>())){};
	}
	template<typename T>
	constexpr bool is_tuple(T tuple) {
		return qpl::is_tuple<T>();
	}

	template<typename T>
	constexpr bool is_signal() {
		return decltype(qpl::impl::signal_signature(qpl::declval<T>())){};
	}
	template<typename T>
	constexpr bool is_signal(T signal) {
		return qpl::is_signal<T>();
	}

	template<typename T>
	concept is_tuple_c = (qpl::is_tuple<T>());

	template<typename T>
	constexpr bool is_pair() {
		return decltype(qpl::impl::pair_signature(qpl::declval<T>())){};
	}
	template<typename T>
	concept is_pair_c = (qpl::is_pair<T>());


	template<typename T> requires(qpl::is_tuple<T>())
	constexpr qpl::size tuple_size() {
		return std::tuple_size_v<T>;
	}
	template<typename T> requires(qpl::is_tuple<T>())
	constexpr qpl::size tuple_size(T tuple) {
		return std::tuple_size_v<T>;
	}
	template<typename... Ts> requires(!(qpl::is_tuple<Ts>() && ...))
	constexpr qpl::size tuple_size() {
		return std::tuple_size_v<std::tuple<Ts...>>;
	}
	template<typename... Ts>
	constexpr qpl::size variadic_size() {
		return std::tuple_size_v<std::tuple<Ts...>>;
	}
	template<typename... Ts>
	constexpr qpl::size variadic_size(Ts&&... args) {
		return qpl::variadic_size<Ts...>();
	}
	template<qpl::size N, typename T> requires(qpl::is_tuple<T>())
	using tuple_type = std::tuple_element_t<N, T>;

	template<qpl::size N, typename... Ts>
	using variadic_type = std::tuple_element_t<N, std::tuple<Ts...>>;

	template<typename... Ts>
	using variadic_type_back = std::tuple_element_t<sizeof...(Ts) - 1, std::tuple<Ts...>>;

	template<typename... Ts>
	using variadic_type_front = std::tuple_element_t<0, std::tuple<Ts...>>;

	template <bool expr, typename T>
	constexpr auto conditional_reference_value(T&& value)
	{
		if constexpr (expr)
			return std::ref(value);
		else
			return std::forward<T>(value);
	}

	template <qpl::size front, qpl::size back = qpl::size_max, bool flip = false, typename... Args>
	constexpr auto conditional_reference_tie(Args&&... values)
	{
		auto&& tuple = std::forward_as_tuple(std::forward<Args>(values)...);

		constexpr auto size = qpl::variadic_size<Args...>();
		auto unpack = [&]<qpl::size... i>(std::index_sequence<i...>) {
			return std::make_tuple(
				conditional_reference_value<
				(
					(front != 0 && i < front) ||
					(back != qpl::size_max && ((size - 1 - i) < back))
					) == flip
					>(std::get<i>(tuple))
				...);
		};
		return unpack(std::make_index_sequence<size>());
	}

	namespace detail {
		template<auto flags, typename... types>
		constexpr auto flags_to_tuple() {
			using flag_tuple = std::tuple<types...>;
			constexpr auto tuple_size = qpl::tuple_size<flag_tuple>();

			constexpr qpl::size SET_BITS = std::popcount(flags);

			auto flags_to_array = [&]() {
				std::array<qpl::size, SET_BITS> result{};
				qpl::size ctr = 0u;
				for (qpl::size i = 0u; i < qpl::bits_in_type<decltype(flags)>() && i < tuple_size; ++i) {
					if (flags & (1 << i)) {
						result[ctr] = i;
						++ctr;
					}
				}
				return result;
			};
			constexpr auto bit_array = flags_to_array();

			auto make_tuple = [&]<typename I, I... indices>(std::index_sequence<indices...>) {
				return std::tuple<std::tuple_element_t<bit_array[indices], flag_tuple>...>{};
			};
			return make_tuple.template operator()<qpl::size>(std::make_index_sequence<std::min(qpl::tuple_size<flag_tuple>(), SET_BITS)>());
		}
	}

	template<auto flags, typename... types>
	using flags_to_tuple = decltype(qpl::detail::flags_to_tuple<flags, types...>());



	namespace detail {

		template<qpl::size N, typename... Ts>
		constexpr auto variadic_type_splice_back() {
			using tuple = std::tuple<Ts...>;
			auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return qpl::declval<std::tuple<std::tuple_element_t<Ints, Tuple>...>>();
			};
			return unpack_tuple.template operator()<tuple>(std::make_index_sequence<qpl::tuple_size<tuple>() - N>());
		}

		template<qpl::size N, typename... Ts>
		constexpr auto variadic_type_splice_front() {
			using tuple = std::tuple<Ts...>;
			auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return std::tuple<std::tuple_element_t<Ints + N, Tuple>...>{};
			};
			return unpack_tuple.template operator() < tuple > (std::make_index_sequence<qpl::tuple_size<tuple>() - N>());
		}
		template<qpl::size front, qpl::size back, typename... Ts>
		constexpr auto variadic_type_splice_front_back() {
			using tuple = std::tuple<Ts...>;
			auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return std::tuple<std::tuple_element_t<Ints + front, Tuple>...>{};
			};
			return unpack_tuple.template operator() < tuple > (std::make_index_sequence<qpl::tuple_size<tuple>() - (front + back)>());
		}
		template<qpl::size offset, qpl::size size, typename... Ts>
		constexpr auto variadic_type_splice() {
			using tuple = std::tuple<Ts...>;
			auto unpack_tuple = [&]<typename Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return std::tuple<std::tuple_element_t<Ints + offset, Tuple>...>{};
			};
			return unpack_tuple.template operator() < tuple > (std::make_index_sequence<size>());
		}
	}

	template<qpl::size N, typename... Ts>
	using variadic_type_splice_back = decltype(qpl::detail::variadic_type_splice_back<N, Ts...>());

	template<qpl::size N, typename... Ts>
	using variadic_type_splice_front = decltype(qpl::detail::variadic_type_splice_front<N, Ts...>());

	template<qpl::size offset, qpl::size size, typename... Ts>
	using variadic_type_splice = decltype(qpl::detail::variadic_type_splice<offset, size, Ts...>());


	template<qpl::size front, qpl::size back, typename... Ts>
	using variadic_type_splice_front_back = decltype(qpl::detail::variadic_type_splice_front_back<front, back, Ts...>());


	template<qpl::size N, typename T> requires(qpl::is_tuple<T>())
	constexpr const auto& tuple_value(const T& tuple) {
		return std::get<N>(std::forward<T>(tuple));
	}
	template<qpl::size N, typename T> requires(qpl::is_tuple<T>())
	constexpr auto& tuple_value(T&& tuple) {
		return std::get<N>(std::forward<T>(tuple));
	}
	template<typename T> requires(qpl::is_tuple<T>())
	constexpr const auto& tuple_value_front(const T& tuple) {
		return qpl::tuple_value<0u>(std::move(tuple));
	}
	template<typename T> requires(qpl::is_tuple<T>())
	constexpr auto& tuple_value_front(T&& tuple) {
		return qpl::tuple_value<0u>(std::move(tuple));
	}
	template<typename T> requires(qpl::is_tuple<T>())
	constexpr auto& tuple_value_back(T&& tuple) {
		return qpl::tuple_value<(qpl::tuple_size<T>() - 1)>(std::move(tuple));
	}


	template<qpl::size N, typename... Args>
	constexpr auto& variadic_value(Args&&... args) {
		return qpl::tuple_value<N>(std::tie(std::forward<Args>(args)...));
	}
	template<typename... Args>
	constexpr auto& variadic_value_front(Args&&... args) {
		return qpl::variadic_value<0>(std::forward<Args>(args)...);
	}
	template<typename... Args>
	constexpr auto& variadic_value_back(Args&&... args) {
		return qpl::variadic_value<qpl::variadic_size<Args...>() - 1>(std::forward<Args>(args)...);
	}

	/*
	template<typename... Ts>
	constexpr auto tuple_value_front(Ts&&... args) {
		return qpl::variadic_value<0u>(std::forward<Ts>(args)...);
	}
	template<typename... Ts>
	constexpr auto tuple_value_back(Ts&&... args) {
		return qpl::variadic_value<qpl::variadic_size<Ts...>() - 1>(std::forward<Ts>(args)...);
	}
	*/

	template<typename T> requires(qpl::is_tuple<T>())
		constexpr auto tuple_reverse(T tuple) {
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
			constexpr auto size = qpl::tuple_size<Tuple>() - 1;
			std::tuple<qpl::tuple_type<size - Ints, Tuple>...> result;
			((std::get<size - Ints>(result) = std::get<Ints>(tuple)), ...);
			return result;
		};

		constexpr auto size = qpl::tuple_size<T>();
		constexpr auto sequence = std::make_index_sequence<size>();
		return unpack.template operator() < T > (sequence);
	}
	template<typename... Ts>
	constexpr auto variadic_reverse(Ts&&... args) {
		return qpl::tuple_reverse(std::make_tuple(args...));
	}

	template<typename... Ts>
	constexpr auto tuple_reverse(Ts&&... args) {
		return qpl::tuple_reverse(std::make_tuple(args...));
	}
	namespace impl {
		template<typename... Args>
		constexpr auto variadic_type_reverse() {
			auto unpack = [&]<typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
				constexpr auto size = qpl::tuple_size<Tuple>() - 1;
				std::tuple<qpl::tuple_type<size - Ints, Tuple>...> result;
				return result;
			};

			constexpr auto sequence = std::make_index_sequence<qpl::tuple_size<std::tuple<Args...>>()>();
			decltype(unpack.template operator() < std::tuple<Args...> > (sequence)) result;
			return result;
		}
	}

	template<typename T>
	constexpr bool is_default_constructible() {
		return std::is_default_constructible_v<T>;
	}
	template<typename ... Ts> requires (sizeof...(Ts) > 1)
	constexpr bool is_default_constructible() {
		return (qpl::is_default_constructible<Ts>() && ...);
	}
	template<typename T> requires (qpl::is_tuple<T>())
	constexpr bool is_default_constructible() {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			return (qpl::is_default_constructible<qpl::tuple_type<Ints, T>>() && ...);
		};
		return unpack(std::make_index_sequence<qpl::tuple_size<T>()>());
	}



	template<typename T, typename U>
	concept is_assignable_c = requires(T a, U b) {
		a = b;
	};
	template<typename T, typename U>
	constexpr bool is_assignable() {
		return qpl::is_assignable_c<T, U>;
	}

	template<typename Tuple, typename F> requires (qpl::is_tuple<Tuple>())
	constexpr auto tuple_unpack(F&& function) {
		if constexpr (qpl::is_default_constructible<Tuple>()) {
			auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
				return std::forward<F>(function)(qpl::decay<qpl::tuple_type<Ints, Tuple>>{}...);
			};
			return unpack(std::make_index_sequence<qpl::tuple_size<Tuple>()>());
		}
		else {
			auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
				return std::forward<F>(function)(qpl::identity<qpl::decay<qpl::tuple_type<Ints, Tuple>>>{}...);
			};
			return unpack(std::make_index_sequence<qpl::tuple_size<Tuple>()>());
		}
	}
	template<typename Tuple, typename F> requires (qpl::is_tuple<Tuple>())
		constexpr auto tuple_unpack(Tuple tuple, F&& function) {
		auto unpack = [&]<qpl::size... Ints>(std::index_sequence<Ints...>) {
			return std::forward<F>(function)(qpl::tuple_value<Ints>(tuple)...);
		};
		return unpack(std::make_index_sequence<qpl::tuple_size<Tuple>()>());
	}


	template<typename T> requires(qpl::is_tuple<T>())
		using tuple_type_reverse = decltype(qpl::tuple_reverse(qpl::declval<T>()));

	template<typename... Ts>
	using variadic_type_reverse = decltype(impl::variadic_type_reverse<Ts...>());

	template<qpl::size N, typename T> requires(qpl::is_tuple<T>())
		constexpr auto tuple_splice_back(T tuple) {
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
			std::tuple<qpl::tuple_type<Ints, Tuple>...> result;
			((std::get<Ints>(result) = std::get<Ints>(tuple)), ...);
			return result;
		};

		constexpr auto size = qpl::tuple_size<T>();
		constexpr auto sequence = std::make_index_sequence<size - N>();
		return unpack.template operator() < T > (sequence);
	}

	template<qpl::size N, typename T> requires(qpl::is_tuple<T>())
		constexpr auto tuple_splice_front(T tuple) {
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
			std::tuple<qpl::tuple_type<Ints + N, Tuple>...> result;
			((std::get<Ints>(result) = std::get<Ints + N>(tuple)), ...);
			return result;
		};

		constexpr auto size = qpl::tuple_size<T>();
		constexpr auto sequence = std::make_index_sequence<size - N>();
		return unpack.template operator() < T > (sequence);
	}

	template<qpl::size start, qpl::size end = qpl::size_max, typename T> requires(qpl::is_tuple<T>())
		constexpr auto tuple_splice(T tuple) {
		auto unpack = [&]<typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
			std::tuple<qpl::tuple_type<Ints + start, Tuple>...> result;
			((std::get<Ints>(result) = std::get<Ints + start>(tuple)), ...);
			return result;
		};

		constexpr auto real_end = (end < (qpl::tuple_size<T>() - 1) ? end : (qpl::tuple_size<T>() - 1));
		constexpr auto new_size = real_end - start + 1;
		constexpr auto sequence = std::make_index_sequence<new_size>();
		return unpack.template operator() < T > (sequence);
	}

	template<qpl::size N, typename... Ts>
	constexpr auto variadic_splice_back(Ts&&... values) {
		return qpl::tuple_splice_back<N>(std::make_tuple(values...));
	}
	template<qpl::size N, typename... Ts>
	constexpr auto variadic_splice_front(Ts&&... values) {
		return qpl::tuple_splice_front<N>(std::make_tuple(values...));
	}
	template<qpl::size start, qpl::size end = qpl::size_max, typename... Ts>
	constexpr auto variadic_splice(Ts&&... values) {
		return qpl::tuple_splice<start, end>(std::make_tuple(values...));
	}

	template<qpl::size N, typename... Ts>
	constexpr auto tuple_splice_back(Ts&&... values) {
		return qpl::tuple_splice_back<N>(std::make_tuple(values...));
	}
	template<qpl::size N, typename... Ts>
	constexpr auto tuple_splice_front(Ts&&... values) {
		return qpl::tuple_splice_front<N>(std::make_tuple(values...));
	}
	template<qpl::size start, qpl::size end = qpl::size_max, typename... Ts>
	constexpr auto tuple_splice(Ts&&... values) {
		return qpl::tuple_splice<start, end>(std::make_tuple(values...));
	}

	template<typename ... types>
	struct inheritance_variadic : types... {};

	template<typename Tuple> requires (qpl::is_tuple<Tuple>())
	using inheritance_tuple = qpl::rename_variadic<Tuple, inheritance_variadic>;

	template<auto flags, typename ... FlagTypes>
	using inheritance_flags = qpl::inheritance_tuple<qpl::flags_to_tuple<flags, FlagTypes...>>;


	namespace impl {
		template<typename T>
		concept is_cout_printable_c = requires(const T t) {
			std::cout << t;
		};
		template<typename T>
		concept is_cin_readable_c = requires(T & t) {
			std::cin >> t;
		};
		template<typename T>
		concept is_wcout_printable_c = requires(const T t) {
			std::wcout << t;
		};
		template<typename T>
		concept is_wcin_readable_c = requires(T & t) {
			std::wcin >> t;
		};
		template<typename T>
		concept is_u32_printable_c = requires(const T t, std::basic_ostringstream<char32_t> stream) {
			stream << t;
		};
		template<typename T>
		concept is_u32_readable_c = requires(T & t, std::basic_istringstream<char32_t> stream) {
			stream >> t;
		};
	}
	template<typename... Args>
	constexpr bool is_cout_printable() {
		return (impl::is_cout_printable_c<Args> && ...);
	}
	template<typename... Args>
	constexpr bool is_cin_readable() {
		return (impl::is_cin_readable_c<Args> && ...);
	}

	template<typename... Args>
	constexpr bool is_wcout_printable() {
		return ((impl::is_wcout_printable_c<Args> || impl::is_cout_printable_c<Args>) && ...);
	}
	template<typename... Args>
	constexpr bool is_wcin_readable() {
		return ((impl::is_wcin_readable_c<Args> || impl::is_cin_readable_c<Args>) && ...);
	}

	template<typename... Args>
	constexpr bool is_u32_printable() {
		return (impl::is_u32_printable_c<Args> && ...) || is_wcout_printable<Args...>();
	}
	template<typename... Args>
	constexpr bool is_u32_readable() {
		return (impl::is_u32_readable_c<Args> && ...) || is_wcout_readable<Args...>();
	}

	template<typename T>
	concept is_cin_readable_c = (qpl::is_cin_readable<T>());
	template<typename T>
	concept is_wcin_readable_c = (qpl::is_wcin_readable<T>());
	template<typename T>
	concept is_u32_readable_c = (qpl::is_u32_readable<T>());
	template<typename T>
	concept is_cout_printable_c = (qpl::is_cout_printable<T>());
	template<typename T>
	concept is_wcout_printable_c = (qpl::is_wcout_printable<T>());
	template<typename T>
	concept is_u32_printable_c = (qpl::is_cout_printable<T>());
	
	struct colored_text_get_position_type;

	namespace impl {
		template<typename T>
		concept is_printable_c = is_cout_printable_c<T> || is_wcout_printable_c<T> || is_u32_printable_c<T>;


		template<typename T>
		constexpr bool is_printable() {
			if constexpr (qpl::is_container_c<T>) {
				return qpl::impl::is_printable<qpl::container_subtype<T>>();
			}
			else if constexpr (qpl::is_same_decayed<T, qpl::colored_text_get_position_type>()) {
				return true;
			}
			else if constexpr (qpl::is_tuple_c<T>) {
				auto check = [&]<typename... Ts>(std::tuple<Ts...>) {
					constexpr auto b = (qpl::impl::is_printable<Ts>() && ...);
					if constexpr (b) {
						return std::true_type{};
					}
					else {
						return std::false_type{};
					}
				};
				return decltype(check(qpl::declval<T>()))::value;
			}
			else if constexpr (qpl::is_pair_c<T>) {
				auto check = [&]<typename... Ts>(std::pair<Ts...>) {
					constexpr auto b = (qpl::impl::is_printable<Ts>() && ...);
					if constexpr (b) {
						return std::true_type{};
					}
					else {
						return std::false_type{};
					}
				};
				return decltype(check(qpl::declval<T>()))::value;
			}
			else {
				return (static_cast<bool>(impl::is_printable_c<T>));
			}
		}

		template<typename T>
		concept is_readable_c = is_cin_readable_c<T> || is_wcin_readable_c<T> || is_u32_readable_c<T>;


		template<typename T>
		constexpr bool is_readable() {
			if constexpr (qpl::is_container<T>()) {
				return is_readable<qpl::container_subtype<T>>();
			}
			else {
				return (static_cast<bool>(impl::is_readable_c<T>));
			}
		}

	}

	template<typename... Args>
	constexpr bool is_printable() {
		return ((qpl::impl::is_printable<Args>()) && ...);
	}
	template<typename... Args>
	constexpr bool is_readable() {
		return ((qpl::impl::is_readable<Args>()) && ...);
	}




	template<qpl::u32 base, bool sign>
	struct dynamic_integer;

	template<qpl::size bits, bool sign>
	struct integer;

#ifndef QPL_NO_FLOATS
	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct floating_point;
#endif

	template<typename T>
	struct is_qpl_integer_impl : std::false_type
	{};

	template<qpl::size bits, bool sign>
	struct is_qpl_integer_impl<qpl::integer<bits, sign>> : std::true_type
	{};
	
	template<typename T>
	constexpr bool is_qpl_integer() {
		return is_qpl_integer_impl<T>{};
	}


	template<typename T>
	struct is_qpl_integer_signed_impl : std::false_type
	{};

	template<qpl::size bits>
	struct is_qpl_integer_signed_impl<qpl::integer<bits, true>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_qpl_integer_signed() {
		return is_qpl_integer_signed_impl<T>{};
	}
#ifndef QPL_NO_FLOATS
	template<typename T>
	struct is_qpl_floating_point_impl : std::false_type {};

	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct is_qpl_floating_point_impl<qpl::floating_point<exponent_bits, mantissa_bits>> : std::true_type {};

	template<typename T>
	constexpr bool is_qpl_floating_point() {
		return is_qpl_floating_point_impl<T>{};
	}
#else

	template<typename T>
	constexpr bool is_qpl_floating_point() {
		return false;
	}
#endif

	template<typename T>
	struct is_qpl_dynamic_integer_impl : std::false_type {};

	template<qpl::u32 base, bool sign>
	struct is_qpl_dynamic_integer_impl<qpl::dynamic_integer<base, sign>> : std::true_type {};

	template<typename T>
	constexpr bool is_qpl_dynamic_integer() {
		return is_qpl_dynamic_integer_impl<T>{};
	}

	template<typename T>
	constexpr bool is_qpl_arithmetic() {
		return is_qpl_dynamic_integer<T>() || is_qpl_integer<T>() || is_qpl_floating_point<T>();
	}

#ifdef QPL_USE_INTRINSICS
	template<qpl::size bits, bool sign>
	struct x64_integer;

	template<typename T>
	struct is_qpl_x64_integer_impl : std::false_type
	{};

	template<qpl::size bits, bool sign>
	struct is_qpl_x64_integer_impl<qpl::x64_integer<bits, sign>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_qpl_x64_integer() {
		return is_qpl_x64_integer_impl<T>{};
	}
#endif



	template<typename T>
	struct is_qpl_x64_integer_signed_impl : std::false_type
	{};

	template<qpl::size bits>
	struct is_qpl_x64_integer_signed_impl<qpl::x64_integer<bits, true>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_qpl_x64_integer_signed() {
		return is_qpl_x64_integer_signed_impl<T>{};
	}



	template<typename T>
	constexpr bool is_stl_arithmetic() {
		return std::is_arithmetic_v<T>;
	}
#ifdef QPL_USE_INTRINSICS
	template<typename T>
	constexpr bool is_arithmetic() {
		return std::is_arithmetic_v<T> || is_qpl_integer<T>() || is_qpl_floating_point<T>() || is_qpl_x64_integer<T>();
	}
	template<typename T>
	constexpr bool is_integer() {
		return std::numeric_limits<T>::is_integer || qpl::is_qpl_integer<T>() || is_qpl_x64_integer<T>();
	}

	template<typename T>
	constexpr bool is_unsigned() {
		return !std::numeric_limits<T>::is_signed || !is_qpl_integer_signed<T>() || !is_qpl_x64_integer_signed<T>();
	}
	template<typename T>
	constexpr bool is_signed() {
		return std::numeric_limits<T>::is_signed || is_qpl_integer_signed<T>() || is_qpl_x64_integer_signed<T>();
	}
#else
	template<typename T>
	constexpr bool is_arithmetic() {
		return std::is_arithmetic_v<T> || is_qpl_integer<T>() || is_qpl_floating_point<T>();
	}
	template<typename T>
	constexpr bool is_integer() {
		return std::numeric_limits<T>::is_integer || qpl::is_qpl_integer<T>();
	}

	template<typename T>
	constexpr bool is_unsigned() {
		return !std::numeric_limits<T>::is_signed || !is_qpl_integer_signed<T>();
	}
	template<typename T>
	constexpr bool is_signed() {
		return std::numeric_limits<T>::is_signed || is_qpl_integer_signed<T>();
	}


#endif


	template<typename T>
	constexpr bool is_stl_floating_point() {
		return std::is_floating_point_v<T>;
	}
	template<typename T>
	constexpr bool is_floating_point() {
		return std::is_floating_point_v<T> || is_qpl_floating_point<T>();
	}
	template<typename T>
	concept is_floating_point_c = (qpl::is_floating_point<T>());

	template<typename T>
	constexpr bool is_stl_integer() {
		return std::numeric_limits<T>::is_integer;
	}

	template<typename Tuple, class... Args>
	constexpr qpl::size type_match_count() {
		qpl::size result = 0u;
		qpl::constexpr_iterate<qpl::tuple_size<Tuple>()>([&](auto i) {
			if constexpr (qpl::is_any_type_equal_to<qpl::tuple_type<i, Tuple>, Args...>()) {
				++result;
			}
		});
		return result;
	}

	namespace detail {
		template<typename T>
		constexpr auto std_basic_string(std::basic_string<T>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto std_basic_string(T) {
			return std::false_type{};
		}
		template<typename T>
		constexpr auto std_basic_string_view(std::basic_string_view<T>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto std_basic_string_view(T) {
			return std::false_type{};
		}
	}

	template<typename T>
	constexpr bool is_std_basic_string() {
		return decltype(qpl::detail::std_basic_string(qpl::declval<T>())){};
	}
	template<typename T>
	constexpr bool is_std_basic_string_view() {
		return decltype(qpl::detail::std_basic_string_view(qpl::declval<T>())){};
	}

	template<typename T>
	constexpr bool is_string_type() {
		return qpl::is_std_basic_string<T>() || qpl::is_std_basic_string_view<T>() || qpl::is_any_type_decayed_equal_to<T, char, const char*, char*, const char[], wchar_t, const wchar_t*, const wchar_t[], wchar_t*, char32_t>();
	}



	template<typename T>
	constexpr bool is_standard_string_type() {
		return qpl::is_any_type_decayed_equal_to<T, char, const char*, char*, const char[], std::string, std::string_view>();
	}

	template<typename T>
	constexpr bool is_wstring_type() {
		return qpl::is_any_type_decayed_equal_to<T, wchar_t, const wchar_t*, wchar_t*, const wchar_t[], std::wstring, std::wstring_view>();
	}
	template<typename T>
	constexpr bool is_u32string_type() {
		return qpl::is_any_type_decayed_equal_to<T, char32_t, const char32_t*, char32_t*, const char32_t[], std::basic_string<std::uint32_t>, std::basic_string_view<std::uint32_t>>();
	}

	template<typename T>
	constexpr bool is_long_string_type() {
		return qpl::is_std_basic_string<T>() || qpl::is_std_basic_string_view<T>() || qpl::is_any_type_decayed_equal_to<T, const char*, char*, const char[], const wchar_t*, const wchar_t[], wchar_t*>();
	}

	template<typename T>
	constexpr bool is_long_standard_string_type() {
		return qpl::is_any_type_decayed_equal_to<T, const char*, char*, const char[], std::string, std::string_view>();
	}

	template<typename T>
	constexpr bool is_long_wstring_type() {
		return qpl::is_any_type_decayed_equal_to<T, const wchar_t*, wchar_t*, const wchar_t[], std::wstring, std::wstring_view>();
	}

	template<typename T>
	constexpr bool is_string_type(T value) {
		return is_string_type<T>();
	}
	template<typename T>
	constexpr bool is_standard_string_type(T value) {
		return is_standard_string_type<T>();
	}
	template<typename T>
	constexpr bool is_wstring_type(T value) {
		return is_wstring_type<T>();
	}

	template<typename T>
	constexpr bool is_char_type() {
		return qpl::is_any_type_decayed_equal_to<T, char, wchar_t, char8_t, char16_t, char32_t, qpl::u8, qpl::i8, qpl::u16, qpl::i16, qpl::u32, qpl::i32>();
	}
	template<typename T>
	constexpr bool is_char_pointer_type() {
		return qpl::is_any_type_equal_to<std::remove_const_t<T>,
			char*, char[], const char*, const char[],
			wchar_t*, wchar_t[], const wchar_t*, const wchar_t[],
			char8_t*, char8_t[], const char8_t*, const char8_t[],
			char16_t*, char16_t[], const char16_t*, const char16_t[],
			char32_t*, char32_t[], const char32_t*, const char32_t[]
		>();
	}

	template<bool condition>
	using if_true = std::conditional_t<condition, true_type, false_type>;

	template<class T>
	constexpr bool is_truth_type() {
		return qpl::is_any_type_equal_to<T, true_type, false_type>();
	}
	template<class T>
	constexpr bool is_true() {
		return std::is_same_v<T, true_type>;
	}
	template<class T>
	constexpr bool is_false() {
		return std::is_same_v<T, false_type>;
	}

	template<class U, class T>
	constexpr bool is_derived() {
		return std::is_base_of_v<T, U>;
	}

	template<class Truth, class T, typename... Args>
	struct conditional_impl;

	template<typename... Args>
	struct conditional_indirection {
		using type = typename conditional_impl<Args...>::type;
	};


	template<class Truth, class T, typename... Args>
	struct conditional_impl {
		using type = typename std::enable_if_t<(sizeof...(Args) == 0 && std::is_same_v<Truth, qpl::default_type>) || qpl::is_truth_type<Truth>(),
			std::conditional_t<(sizeof...(Args) >= 2), std::conditional_t<qpl::is_true<Truth>(), std::conditional_t<std::is_same_v<T, qpl::error_type>, T, qpl::identity<T>>, qpl::conditional_indirection<Args...>>,
			std::conditional_t<(sizeof...(Args) == 0), std::conditional_t<std::is_same_v<Truth, qpl::default_type>, std::conditional_t<std::is_same_v<T, qpl::error_type>, T, qpl::identity<T>>, std::enable_if<qpl::is_true<Truth>(), T>>,
			std::conditional_t<qpl::is_true<Truth>(), std::conditional_t<std::is_same_v<T, qpl::error_type>, T, qpl::identity<T>>, qpl::empty_indirection<Args...>>>>>::type;
	};


	template<class Truth, class T, typename... Args>
	using resolved_conditional = typename conditional_impl<Truth, T, Args...>::type;

	template<class Truth, class T, typename... Args>
	using conditional_identity = typename std::conditional_t<std::is_same_v<qpl::resolved_conditional<Truth, T, Args...>, qpl::error_type>, qpl::error_type, conditional_impl<Truth, T, Args...>>;

	template<class Truth, class T, typename... Args>
	using conditional = typename qpl::conditional_identity<Truth, T, Args...>::type;
	


	template<typename T>
	qpl::size constexpr type_used_bit_size() {
		if constexpr (qpl::is_qpl_integer<T>()) {
			return T::used_bit_size();
		}
		else if constexpr (qpl::is_stl_integer<T>()) {
			return std::numeric_limits<T>::digits;
		}
		else {
			return qpl::bits_in_type<T>();
		}
	}

	constexpr bool char_is_signed() {
		return (CHAR_MIN != 0);
	}	
	constexpr bool char_is_unsigned() {
		return (CHAR_MIN == 0);
	}
	constexpr bool wchar_is_signed() {
		return (WCHAR_MIN != 0);
	}
	constexpr bool wchar_is_unsigned() {
		return (WCHAR_MIN == 0);
	}

	namespace {
		constexpr qpl::u32 endian_u32 = 0x01020304ui32;
		constexpr qpl::u8 endian_u8 = (const qpl::u8&)endian_u32; //todo replace once std::endian is released
	}

	constexpr bool endian_little() {
		return (endian_u8 == 0x04);
	}
	constexpr bool endian_middle() {
		return (endian_u8 == 0x02);
	}
	constexpr bool endian_big() {
		return (endian_u8 == 0x01);
	}

	using char_type = qpl::conditional<
		qpl::if_true<qpl::char_is_signed()>, qpl::i8,
		qpl::if_true<qpl::char_is_unsigned()>, qpl::u8>;

	using wchar_type = wchar_t;

	template<typename T>
	using signed_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 8 }>, qpl::i8,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 16 }>, qpl::i16,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::i32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::i64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using unsigned_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 8 }>, qpl::u8,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 16 }>, qpl::u16,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::u32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::u64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using int_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::is_integer<T>()>, T,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::i32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::i64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using float_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::is_floating_point<T>()>, T,
				qpl::if_true<qpl::bits_in_type<T>() <= std::size_t{ 32 }>, qpl::f32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::f64,
				qpl::default_error>,
		qpl::default_error>;


	template<typename T, typename U>
	constexpr bool equals_type(U&& value = U{}) {
		return std::is_same_v<std::decay_t<U>, std::decay_t<T>>;
	}

	namespace impl {
		template<typename C>
		auto container_deepest_subtype() {
			if constexpr (!qpl::is_container<C>()) {
				return qpl::declval<C>();
			}
			else if constexpr (qpl::is_container<qpl::container_subtype<C>>()) {
				return container_deepest_subtype<qpl::container_subtype<C>>();
			}
			else {
				return qpl::declval<qpl::container_subtype<C>>();
			}
		}
	}

	template<typename C>
	using container_deepest_subtype = decltype(impl::container_deepest_subtype<C>());



	namespace impl {
		template<typename T, qpl::size N>
		struct span_size_type {
			constexpr static qpl::size value = N;
		};
		template<typename T, qpl::size N>
		constexpr auto span_size(const std::span<T, N>& span) {
			return span_size_type<T, N>{};
		}
	}
	template<typename T>
	constexpr auto span_size() {
		return decltype(impl::span_size(qpl::declval<T>()))::value;
	}
	template<typename T>
	struct span_size_type {
		using type = typename decltype(impl::span_size(qpl::declval<T>()));
	};

	template<typename T>
	constexpr bool is_span() {
		//return (qpl::is_same<T, std::span<qpl::container_subtype_with_const<T>, qpl::span_size<T>()>>());
		using span_t = std::span<qpl::container_subtype_with_const<T>, qpl::span_size<T>()>;
		return qpl::is_same<T, span_t>();
	}
	template<typename T>
	concept is_span_c = is_span<T>();


	template<typename T>
	concept has_size_c = requires(const T x) {
		x.size();
	};
	template<typename T>
	constexpr bool has_size() {
		return has_size_c<T>;
	}

	template<typename T>
	concept has_data_c = requires(T x) {
		{ x.data() } -> std::same_as<qpl::container_subtype<T>*>;
	};
	template<typename T>
	constexpr bool has_data() {
		return has_data_c<T>;
	}

	template<typename T>
	concept has_resize_c = requires(T & x) {
		x.resize(qpl::size{});
		x.size();
	};
	template<typename T>
	constexpr bool has_resize() {
		return has_resize_c<T>;
	};

	template<typename T>
	concept has_reserve_c = requires(T & x) {
		x.reserve(qpl::size{});
	};
	template<typename T>
	constexpr bool has_reserve() {
		return has_reserve_c<T>;
	};

	template<typename T>
	concept has_push_back_c = requires(T& x) {
		x.push_back(qpl::declval < qpl::container_subtype<T>>());
	};
	template<typename T>
	constexpr bool has_push_back() {
		return has_push_back_c<T>;
	}
	template<typename T>
	concept has_emplace_back_c = requires(T & x) {
		x.emplace_back(std::move(qpl::declval<qpl::container_subtype<T>>()));
	};
	template<typename T>
	constexpr bool has_emplace_back() {
		return has_emplace_back_c<T>;
	}
	template<typename T>
	concept has_pop_back_c = requires(T& x) {
		x.pop_back();
	};
	template<typename T>
	constexpr bool has_pop_back() {
		return has_pop_back_c<T>;
	}
	template<typename T>
	concept has_push_front_c = requires(T& x) {
		x.push_front(qpl::container_subtype<T>{});
	};
	template<typename T>
	constexpr bool has_push_front() {
		return has_push_front_c<T>;
	}
	template<typename T>
	concept has_pop_front_c = requires(T & x) {
		x.pop_front();
	};
	template<typename T>
	constexpr bool has_pop_front() {
		return has_pop_front_c<T>;
	}
	template<typename T>
	concept has_insert_c = requires(T & x) {
		x.insert(qpl::container_subtype<T>{});
	};
	template<typename T>
	constexpr bool has_insert() {
		return has_insert_c<T>;
	}

	template<typename T>
	concept can_grow_c = has_push_back_c<T> || has_insert_c<T>;

	template<typename T>
	constexpr bool can_grow() {
		return can_grow_c<T>;
	}

	template<typename T>
	concept has_erase_c = requires(T & x) {
		x.erase(qpl::container_subtype<T>{});
	};
	template<typename T>
	constexpr bool has_erase() {
		return has_erase_c<T>;
	}

	template<typename T>
	concept has_less_c = requires(const T x) {
		{ x < T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_less() {
		return has_less_c<T>;
	};

	template<typename T>
	concept has_greater_c = requires(const T x) {
		{ x > T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_greater() {
		return has_greater_c<T>;
	};
	template<typename T>
	concept has_equal_c = requires(const T x) {
		{ x == T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_equal() {
		return has_equal_c<T>;
	};
	template<typename T>
	concept has_not_equal_c = requires(const T x) {
		{ x != T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_not_equal() {
		return has_not_equal_c<T>;
	};
	template<typename T>
	concept has_less_equal_c = requires(const T x) {
		{ x <= T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_less_equal() {
		return has_less_equal_c<T>;
	};
	template<typename T>
	concept has_grater_equal_c = requires(const T x) {
		{ x >= T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool has_grater_equal() {
		return has_grater_equal_c<T>;
	};

	template<typename T, typename U>
	concept is_equal_comparable_c = requires(const T x, const U y) {
		{ x == y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_equal_comparable() {
		return is_equal_comparable_c<T, U>;
	};
	template<typename T, typename U>
	concept is_not_equal_comparable_c = requires(const T x, const U y) {
		{ x != y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_not_equal_comparable() {
		return is_not_equal_comparable_c<T, U>;
	};

	template<typename T, typename U>
	concept is_greater_comparable_c = requires(const T x, const U y) {
		{ x > y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_greater_comparable() {
		return is_greater_comparable_c<T, U>;
	};
	template<typename T, typename U>
	concept is_less_comparable_c = requires(const T x, const U y) {
		{ x < y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_less_comparable() {
		return is_less_comparable_c<T, U>;
	};
	template<typename T, typename U>
	concept is_less_equal_comparable_c = requires(const T x, const U y) {
		{ x <= y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_less_equal_comparable() {
		return is_less_equal_comparable_c<T, U>;
	};
	template<typename T, typename U>
	concept is_greater_equal_comparable_c = requires(const T x, const U y) {
		{ x >= y } -> std::same_as<bool>;
	};
	template<typename T, typename U>
	constexpr bool is_greater_equal_comparable() {
		return is_greater_equal_comparable_c<T, U>;
	};

	namespace impl {
		template<typename T, typename F = void>
		struct has_iterator_concept_type {
			constexpr static bool value = false;
			using type = typename qpl::error_type;
		};

		template<typename T>
		struct has_iterator_concept_type<T, std::void_t<typename T::iterator::iterator_concept>> {
			constexpr static bool value = true;
			using type = typename T::iterator::iterator_concept;
		};
	}
	template<typename T>
	constexpr bool has_iterator_concept_type() {
		return impl::has_iterator_concept_type<T>::value;
	}


	template<typename T>
	constexpr bool is_contiguous_container() {
		return qpl::is_same_decayed<typename impl::has_iterator_concept_type<T>::type, std::contiguous_iterator_tag>() && qpl::has_data<T>();
	}

	template<typename T>
	concept is_contiguous_container_c = (is_contiguous_container<T>());


	namespace detail {
		template<typename T>
		constexpr auto get_char_type() {
			if constexpr (qpl::is_container<T>()) {
				return qpl::detail::get_char_type<qpl::container_subtype<T>>();
			}
			else {
				if constexpr (qpl::is_any_type_decayed_equal_to<T, char, char*, char[], const char*, const char[]>()) {
					return char{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::u8, qpl::u8*, qpl::u8[], const qpl::u8*, const qpl::u8[]>()) {
					return u8{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::i8, qpl::i8*, qpl::i8[], const qpl::i8*, const qpl::i8[]>()) {
					return i8{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, wchar_t, wchar_t*, wchar_t[], const wchar_t*, const wchar_t[]>()) {
					return wchar_t{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::u16, qpl::u16*, qpl::u16[], const qpl::u16*, const qpl::u16[]>()) {
					return u16{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::i16, qpl::i16*, qpl::i16[], const qpl::i16*, const qpl::i16[]>()) {
					return i16{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, char32_t, char32_t*, char32_t[], const char32_t*, const char32_t[]>()) {
					return char32_t{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::u32, qpl::u32*, qpl::u32[], const qpl::u32*, const qpl::u32[]>()) {
					return u32{};
				}
				else if constexpr (qpl::is_any_type_decayed_equal_to<T, qpl::i32, qpl::i32*, qpl::i32[], const qpl::i32*, const qpl::i32[]>()) {
					return i32{};
				}
				else {
					return qpl::empty_type{};
				}
			}
		}
	}

	template<typename T>
	using string_char_type = decltype(qpl::detail::get_char_type<T>());




	namespace impl {
		template<typename T, typename F = void>
		struct has_key_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_key_type<T, std::void_t<typename T::key_type>> {
			constexpr static bool value = true;
			using type = typename T::key_type;
		};

		template<typename T, typename F = void>
		struct has_value_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_value_type<T, std::void_t<typename T::value_type>> {
			constexpr static bool value = true;
			using type = typename T::value_type;
		};

		template<typename T, typename F = void>
		struct has_mapped_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_mapped_type<T, std::void_t<typename T::mapped_type>> {
			constexpr static bool value = true;
			using type = typename T::mapped_type;
		};

		template<typename T, typename F = void>
		struct has_key_compare_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_key_compare_type<T, std::void_t<typename T::key_compare>> {
			constexpr static bool value = true;
			using type = typename T::key_compare;
		};

		template<typename T, typename F = void>
		struct has_key_equal_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_key_equal_type<T, std::void_t<typename T::key_equal>> {
			constexpr static bool value = true;
			using type = typename T::key_equal;
		};
		template<typename T, typename F = void>
		struct has_allocator_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_allocator_type<T, std::void_t<typename T::allocator_type>> {
			constexpr static bool value = true;
			using type = typename T::allocator_type;
		};

		template<typename T, typename F = void>
		struct has_hasher_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_hasher_type<T, std::void_t<typename T::hasher>> {
			constexpr static bool value = true;
			using type = typename T::hasher;
		};

		template<qpl::size N, bool B>
		struct std_array_size_info {
			constexpr static qpl::size size = N;
			constexpr static bool value = B;
		};

		template<typename T>
		constexpr auto std_array_size(T&& f) {
			return std_array_size_info<0u, false>{};
		}
		template<typename T, qpl::size N>
		constexpr auto std_array_size(std::array<T, N>&& f) {
			return std_array_size_info<N, true>{};
		}
	}

	template<typename T>
	constexpr bool has_key_type() {
		return impl::has_key_type<T>::value;
	}

	template<typename T>
	constexpr bool has_value_type() {
		return impl::has_value_type<T>::value;
	}

	template<typename T>
	constexpr bool has_mapped_type() {
		return impl::has_mapped_type<T>::value;
	}

	template<typename T>
	constexpr bool has_key_compare_type() {
		return impl::has_key_compare_type<T>::value;
	}
	template<typename T>
	constexpr bool has_key_equal_type() {
		return impl::has_key_equal_type<T>::value;
	}
	template<typename T>
	constexpr bool has_hasher_type() {
		return impl::has_hasher_type<T>::value;
	}
	template<typename T>
	constexpr bool has_allocator_type() {
		return impl::has_allocator_type<T>::value;
	}
	template<typename T>
	constexpr bool has_std_array_size() {
		return decltype(qpl::impl::std_array_size(T{}))::value;
	}

	template<typename C> requires (qpl::is_container<C>())
		using container_key_type = impl::has_key_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_value_type = impl::has_value_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_mapped_type = impl::has_mapped_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_allocator_type = impl::has_allocator_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_key_compare_type = impl::has_key_compare_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_key_equal_type = impl::has_key_equal_type<C>::type;

	template<typename C> requires (qpl::is_container<C>())
		using container_hasher_type = impl::has_hasher_type<C>::type;

	template<typename T>
	constexpr qpl::size std_array_size() {
		return decltype(qpl::impl::std_array_size(T{}))::size;
	}


	template<typename T>
	concept is_sortable_c = std::sortable<T>;

	template<typename T>
	constexpr bool is_sortable() {
		return true || is_sortable_c<T>;
	}
	 
	template<typename T>
	concept is_iterator_c = requires(T a, const T& b) {
		a.operator*();
		++a;
		{ b == T{} } -> std::same_as<bool>;
		{ b != T{} } -> std::same_as<bool>;
	};
	template<typename T>
	constexpr bool is_iterator() {
		return is_iterator_c<T>;
	}


	template<typename C>
	concept has_map_signature_c = (qpl::is_container<C>() && qpl::has_value_type<C>() && qpl::has_mapped_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>());

	template<typename C>
	constexpr bool has_map_signature() {
		return has_map_signature_c<C>;
	}


	namespace impl {
		template <typename U, template <typename, typename...> class container, typename T, typename... rest>
		auto container_change_subtype(const container<T, rest...>& c) {
			if constexpr (qpl::is_long_string_type<T>()) {
				return qpl::declval<container<U>>();
			}
			else if constexpr (qpl::is_container<T>()) {
				return qpl::declval<container<decltype(container_subtype_cast<U>(T{})) >> ();
			}
			else {
				return qpl::declval<container<U>>();
			}
		}

		template<typename R, typename... A>
		constexpr R return_type(R(*)(A...)) {
			return qpl::declval<R>();
		}
		template<typename R, typename... A>
		constexpr R return_type(std::function<R(A...)>) {
			return qpl::declval<R>();
		}
		template<typename C, typename R, typename... A>
		constexpr R return_type(R(C::*)(A...)) {
			return qpl::declval<R>();
		}
		template<typename C, typename R, typename... A>
		constexpr R return_type(R(C::*)(A...) const) {
			return qpl::declval<R>();
		}
		template<typename F>
		constexpr auto return_type(F) {
			return return_type(std::function{ F{} });
		}

		template<typename C, typename R, typename... A>
		constexpr C method_class_type(R(C::*)(A...)) {
			return qpl::declval<C>();
		}
		template<typename C, typename R, typename... A>
		constexpr C method_class_type(R(C::*)(A...) const) {
			return qpl::declval<C>();
		}

		template<typename R, typename... A>
		constexpr auto parameter_type(R(*)(A...)) {
			return std::tuple<A...>{};
		}
		template<typename R, typename... A>
		constexpr auto parameter_type(std::function<R(A...)>) {
			return std::tuple<A...>{};
		}
		template<typename C, typename R, typename... A>
		constexpr auto parameter_type(R(C::*)(A...)) {
			return std::tuple<A...>{};
		}
		template<typename C, typename R, typename... A>
		constexpr auto parameter_type(R(C::*)(A...) const) {
			return std::tuple<A...>{};
		}
		template<typename F>
		constexpr auto parameter_type(F) {
			return parameter_type(std::function{ F{} });
		}

		template<typename R, typename... A>
		constexpr auto function_type(R(*)(A...)) {
			return std::tuple<R, A...>{};
		}
		template<typename R, typename... A>
		constexpr auto function_type(std::function<R(A...)>) {
			return std::tuple<R, A...>{};
		}
		template<typename C, typename R, typename... A>
		constexpr auto function_type(R(C::*)(A...)) {
			return std::tuple<R, A...>{};
		}
		template<typename C, typename R, typename... A>
		constexpr auto function_type(R(C::*)(A...) const) {
			return std::tuple<R, A...>{};
		}
		template<typename F>
		constexpr auto function_type(F) {
			return function_type(std::function{ F{} });
		}


		template<typename R, typename... A>
		constexpr auto make_function() {
			return qpl::declval<R(*)(A...)>();
		}

		template<typename C, typename R, typename... A>
		constexpr auto make_method_help() {
			return qpl::declval<R(C::*)(A...)>();
		}

		template<typename C, typename F>
		constexpr auto make_method() {
			auto unpack_tuple = [&]<class C, class Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return make_method_help<C, std::tuple_element_t<Ints, Tuple>...>();
			};
			using function_type = decltype(impl::function_type(qpl::declval<F>()));
			return unpack_tuple.template operator()<C, function_type>(std::make_index_sequence<qpl::tuple_size<function_type>()>{});
		}

		template<typename C, typename F, typename = void>
		struct has_method : std::false_type {};
		template<typename C, typename F>
		struct has_method<C, F, std::void_t<decltype(make_method<C, F>())>> : std::true_type {};
	}

	template<typename C, typename T>
	struct container_change_subtype_t {
		using type = decltype(impl::container_change_subtype<T>(qpl::declval<C>()));
	}; 

	template<typename C, typename T>
	using container_change_subtype = typename container_change_subtype_t<C, T>::type;

	template<typename C> 
	constexpr bool is_std_set_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::set<qpl::container_key_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_multiset_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::multiset<qpl::container_key_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_unordered_set_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_hasher_type<C>() && qpl::has_key_equal_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::unordered_set<qpl::container_key_type<C>, qpl::container_hasher_type<C>, qpl::container_key_equal_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_unordered_multiset_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_hasher_type<C>() && qpl::has_key_equal_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::unordered_multiset<qpl::container_key_type<C>, qpl::container_hasher_type<C>, qpl::container_key_equal_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_map_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::map<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_multimap_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::multimap<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_unordered_map_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_hasher_type<C>() && qpl::has_key_equal_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::unordered_map<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_hasher_type<C>, qpl::container_key_equal_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_unordered_multimap_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_hasher_type<C>() && qpl::has_key_equal_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::unordered_multimap<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_hasher_type<C>, qpl::container_key_equal_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_vector_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_value_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::vector<qpl::container_value_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_list_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_value_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::list<qpl::container_value_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_forward_list_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_value_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::forward_list<qpl::container_value_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_deque_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_value_type<C>() && qpl::has_allocator_type<C>()) {
				if constexpr (qpl::is_same_decayed<C, std::deque<qpl::container_value_type<C>, qpl::container_allocator_type<C>>>()) {
					return true;
				}
			}
		}
		return false;
	}
	template<typename C>
	constexpr bool is_std_array_type() {
		if constexpr (qpl::is_container<C>()) {
			if constexpr (qpl::has_value_type<C>() && qpl::has_std_array_size<C>()) {
				if constexpr (qpl::is_same<C, std::array<qpl::container_value_type<C>, qpl::std_array_size<C>()>>()) {
					return true;
				}
			}
		}
		return false;
	}

	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_sorted_container() {
		return qpl::is_std_set_type<C>() || qpl::is_std_multiset_type<C>() || qpl::is_std_map_type<C>() || qpl::is_std_multimap_type<C>();
	}

	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_unsorted_container() {
		return !qpl::is_sorted_container<C>();
	}

	template<typename C> requires (qpl::is_container<C>())
		constexpr bool is_std_container() {
		return qpl::is_std_set_type<C>() || qpl::is_std_multiset_type<C>() || 
			qpl::is_std_unordered_set_type<C>() || qpl::is_std_unordered_multiset_type<C>() ||
			qpl::is_std_map_type<C>() || qpl::is_std_multimap_type<C>() || 
			qpl::is_std_unordered_map_type<C>() || qpl::is_std_unordered_multimap_type<C>() ||
			qpl::is_std_vector_type<C>() || 
			qpl::is_std_list_type<C>() ||
			qpl::is_std_forward_list_type<C>() ||
			qpl::is_std_array_type<C>() || 
			qpl::is_std_deque_type<C>();
	}

	namespace impl {
		template<typename T>
		constexpr auto mapped_type() {
			if constexpr (qpl::has_mapped_type<T>()) {
				return qpl::declval<qpl::container_mapped_type<T>>();
			}
			else {
				return qpl::size{};
			}
		}
		template<typename T>
		constexpr auto value_type() {
			if constexpr (qpl::has_key_type<T>()) {
				return qpl::declval<qpl::container_key_type<T>>();
			}
			else {
				return qpl::declval<qpl::container_subtype<T>>();
			}
		}
	}


	template<typename T>
	concept has_square_brackets_c = requires(T & x, const T & y) {
		x[qpl::impl::mapped_type<T>()] = qpl::impl::value_type<T>();
		y[qpl::impl::mapped_type<T>()];
	};
	template<typename T>
	constexpr bool has_square_brackets() {
		return has_square_brackets_c<T>;
	}

	template<typename T>
	concept has_square_brackets_read_c = requires(const T & n) {
		n[qpl::impl::mapped_type<T>()];
	};
	template<typename T>
	constexpr bool has_square_brackets_read() {
		return has_square_brackets_read_c<T>;
	}
	template<typename T>
	concept has_square_brackets_write_c = requires(T & n) {
		n[qpl::impl::mapped_type<T>()] = qpl::impl::value_type<T>();
	};
	template<typename T>
	constexpr bool has_square_brackets_write() {
		return has_square_brackets_write_c<T>;
	}
	template<typename T>
	concept has_at_c = requires(T & x, const T & y) {
		x.at(qpl::impl::mapped_type<T>()) = qpl::impl::value_type<T>();
		y.at(qpl::impl::mapped_type<T>());
	};
	template<typename T>
	constexpr bool has_at() {
		return has_at_c<T>;
	}
	template<typename T>
	concept has_at_read_c = requires(const T & n) {
		n.at(qpl::impl::mapped_type<T>());
	};
	template<typename T>
	constexpr bool has_at_read() {
		return has_at_read_c<T>;
	}
	template<typename T>
	concept has_at_write_c = requires(T & n) {
		n.at(qpl::impl::mapped_type<T>()) = qpl::impl::value_type<T>();
	};
	template<typename T>
	constexpr bool has_at_write() {
		return has_at_write_c<T>;
	}

	template<typename T>
	concept has_resize_and_access_c = has_resize_c<T> && (has_square_brackets_c<T> || has_at_c<T>);

	template<typename T>
	constexpr bool has_resize_and_access() {
		return has_resize_and_access_c<T>;
	}

	namespace detail {


		template<typename T, typename U>
		concept castable_test_default = requires(T v, U x) {
			v = static_cast<T>(x);
		};
		template<typename T>
		concept castable_test_stream = requires(T x, std::wstringstream stream) {
			stream << x;
			stream >> x;
		};
		template<typename T>
		concept castable_test_from_chars = requires(T x, std::string_view sv) {
			std::from_chars(sv.data(), sv.data() + sv.size(), x);
		};
	}

	template<typename T, typename U>
	constexpr bool type_castable() {
		if constexpr (qpl::is_container<U>() && !qpl::is_long_string_type<U>()) {
			if constexpr (qpl::has_resize_and_access<qpl::container_change_subtype<U, T>>()) {
				if constexpr (qpl::has_square_brackets<qpl::container_change_subtype<U, T>>()) {
					return qpl::type_castable<T, decltype(qpl::declval<U>()[0])>();
				}
				else if constexpr (qpl::has_at<qpl::container_change_subtype<U, T>>()) {
					return qpl::type_castable<T, decltype(qpl::declval<U>().at(0))>();
				}
				else {
					return false;
				}
			}
			else {
				if constexpr (qpl::has_push_back<qpl::container_change_subtype<U, T>>()) {
					return qpl::type_castable<T, decltype(*qpl::declval<U>().begin())>();
				}
				else if constexpr (qpl::has_insert<qpl::container_change_subtype<U, T>>()) {
					return qpl::type_castable<T, decltype(*qpl::declval<U>().begin())>();
				}
				else {
					return false;
				}
			}
		}
		else if constexpr (qpl::is_long_standard_string_type<U>()) {

			return detail::castable_test_from_chars<T>;
		}
		else if constexpr (qpl::is_long_wstring_type<U>()) {
			return detail::castable_test_stream<T>;
		}
		else if constexpr (qpl::is_long_standard_string_type<T>()) {
			return qpl::is_printable<T>();
		}
		else if constexpr (qpl::is_long_wstring_type<T>()) {
			return qpl::is_printable<T>();
		}
		else {
			return detail::castable_test_default<T, U>;
		}
	}

	template<typename T, typename Tuple>
	constexpr bool type_castable_tuple() {
		auto unpack = [&]<typename T, typename Tuple, qpl::size... Ints>(std::index_sequence<Ints...>) {
			return ((qpl::type_castable<T, std::tuple_element_t<Ints, Tuple>>()) && ...);
		};

		return unpack.template operator() < T, Tuple > (std::make_index_sequence<qpl::tuple_size<Tuple>()>());
	}

	template<typename T, typename U> requires (qpl::type_castable<T, U>())
	constexpr inline auto type_cast(U&& data) {
		if constexpr (qpl::is_container<U>() && !qpl::is_long_string_type<U>()) {
			typename qpl::container_change_subtype<U, T> result;
			if constexpr (qpl::has_resize_and_access<qpl::container_change_subtype<U, T>>()) {
				result.resize(data.size());
				for (qpl::size i = 0u; i < data.size(); ++i) {
					if constexpr (qpl::has_square_brackets<qpl::container_change_subtype<U, T>>()) {
						result[i] = qpl::type_cast<T>(data[i]);
					}
					else if constexpr (qpl::has_at<qpl::container_change_subtype<U, T>>()) {
						result.at(i) = qpl::type_cast<T>(data.at(i));
					}
					else {
						static_assert("type_cast: T has no [] nor .at()");
					}
				}
			}
			else {
				for (const auto& i : data) {
					if constexpr (qpl::has_push_back<qpl::container_change_subtype<U, T>>()) {
						result.push_back(qpl::type_cast<T>(i));
					}
					else if constexpr (qpl::has_insert<qpl::container_change_subtype<U, T>>()) {
						result.insert(qpl::type_cast<T>(i));
					}
					else {
						static_assert("type_cast: T has no pushback nor insert");
					}
				}
			}
			return result;
		}
		else if constexpr (qpl::is_long_standard_string_type<U>()) {
			std::string_view sv{ data };
			T value;
			std::from_chars(sv.data(), sv.data() + sv.size(), value);
			return value;
		}
		else if constexpr (qpl::is_long_wstring_type<U>()) {
			std::wstringstream stream;
			stream << data;
			T value;
			stream >> value;
			
			return value;
		}
		else if constexpr (qpl::is_long_standard_string_type<T>()) {
			std::ostringstream stream;
			stream << data;
			return stream.str();
		}
		else if constexpr (qpl::is_long_wstring_type<T>()) {
			std::wostringstream stream;
			stream << data;
			return stream.str();
		}
		else {
			return static_cast<T>(data);
		}
	}
	template<typename T>
	constexpr inline auto u8_cast(T&& data) {
		return qpl::type_cast<qpl::u8>(data);
	}
	template<typename T>
	constexpr inline auto i8_cast(T&& data) {
		return qpl::type_cast<qpl::i8>(data);
	}

	template<typename T>
	constexpr inline auto u16_cast(T&& data) {
		return qpl::type_cast<qpl::u16>(data);
	}
	template<typename T>
	constexpr inline auto i16_cast(T&& data) {
		return qpl::type_cast<qpl::i16>(data);
	}

	template<typename T>
	constexpr inline auto u32_cast(T&& data) {
		return qpl::type_cast<qpl::u32>(data);
	}

	template<typename T>
	constexpr inline auto i32_cast(T&& data) {
		return qpl::type_cast<qpl::i32>(data);
	}

	template<typename T>
	constexpr inline auto u64_cast(T&& data) {
		return qpl::type_cast<qpl::u64>(data);
	}
	template<typename T>
	constexpr inline auto i64_cast(T&& data) {
		return qpl::type_cast<qpl::i64>(data);
	}


	template<typename T>
	constexpr inline auto size_cast(T&& data) {
		return qpl::type_cast<qpl::size>(data);
	}
	template<typename T>
	constexpr inline auto isize_cast(T&& data) {
		return qpl::type_cast<qpl::isize>(data);
	}
	template<typename T>
	constexpr inline auto signed_size_cast(T&& data) {
		return static_cast<qpl::signed_type<qpl::size>>(data);
	}
	template<typename T>
	constexpr inline auto char_cast(T&& data) {
		return qpl::type_cast<qpl::char_type>(data);
	}
	template<typename T>
	constexpr inline auto wchar_cast(T&& data) {
		return qpl::type_cast<qpl::wchar_type>(data);
	}
	template<typename T>
	constexpr inline auto f32_cast(T&& data) {
		return qpl::type_cast<qpl::f32>(data);
	}
	template<typename T>
	constexpr inline auto f64_cast(T&& data) {
		return qpl::type_cast<qpl::f64>(data);
	}
	template<typename T>
	constexpr inline auto bool_cast(T&& data) {
		return qpl::type_cast<bool>(data);
	}

	template<typename T>
	constexpr inline auto signed_cast(T&& value) {
		return qpl::type_cast<signed_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto unsigned_cast(T&& value) {
		return qpl::type_cast<unsigned_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto int_cast(T&& value) {
		return qpl::type_cast<int_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto float_cast(T&& value) {
		return qpl::type_cast<float_type<T>>(value);
	}

	template<typename F>
	constexpr bool is_function(F&& function) {
		return std::is_function_v<F>;
	}
	template<typename F>
	constexpr bool is_function() {
		return std::is_function_v<F>;
	}
	template<typename F>
	constexpr bool is_method(F&& function) {
		return std::is_member_function_pointer_v<F>;
	}
	template<typename F>
	constexpr bool is_method() {
		return std::is_member_function_pointer_v<F>;
	}

	template<typename F>
	concept is_callable_c = requires(F f) {
		std::function{ f };
	};

	template<typename F>
	constexpr bool is_callable() {
		return is_callable_c<F>;
	}

	template<typename... Args>
	using make_function = decltype(impl::make_function<Args...>());

	template<typename C, typename F>
	using make_method = decltype(impl::make_method<C, F>());


	template<typename C, typename F>
	constexpr bool has_method() {
		return impl::has_method<C, F>::value;
	}

	template<typename F>
	struct return_type_t {
		using type = decltype(impl::return_type(qpl::declval<F>()));
	};
	template<typename F>
	struct parameter_type_t {
		using type = decltype(impl::parameter_type(qpl::declval<F>()));
	};
	template<typename F>
	struct method_class_type_t {
		using type = decltype(impl::method_class_type(qpl::declval<F>()));
	};
	template<typename F>
	struct function_type_t {
		using type = decltype(impl::function_type(qpl::declval<F>()));
	};

	template<typename F>
	using return_type = decltype(impl::return_type(qpl::declval<F>()));

	template<typename F>
	using parameter_type = decltype(impl::parameter_type(qpl::declval<F>()));

	template<typename F>
	using method_class_type = decltype(impl::method_class_type(qpl::declval<F>()));

	template<typename F>
	using function_type = decltype(impl::function_type(qpl::declval<F>()));

	template<typename F>
	constexpr qpl::size return_size(F) {
		return qpl::tuple_size<qpl::return_type<F>>();
	}
	template<typename F>
	constexpr qpl::size parameter_size(F) {
		return qpl::tuple_size<qpl::parameter_type<F>>();
	}
	template<typename F>
	constexpr qpl::size parameter_size() {
		return qpl::tuple_size<qpl::parameter_type<F>>();
	}
	template<typename F>
	constexpr qpl::size function_size(F) {
		return qpl::tuple_size<qpl::function_type<F>>();
	}
	template<typename F>
	constexpr qpl::size function_size() {
		return qpl::tuple_size<qpl::function_type<F>>();
	}


	template<class F>
	constexpr qpl::size mantissa_bit_size() {
		if constexpr (qpl::is_stl_floating_point<F>()) {
			if constexpr (qpl::is_same<F, float>()) {
				return qpl::f32_mantissa_size();
			}
			else if constexpr (qpl::is_same<F, double>()) {
				return qpl::f64_mantissa_size();
			}
		}
		else if constexpr (qpl::is_qpl_floating_point<F>()) {
			return F::mantissa_bit_size();
		}
		else {
			return 0;
		}
	}
	template<class F>
	constexpr qpl::size exponent_bit_size() {
		if constexpr (qpl::is_stl_floating_point<F>()) {
			if constexpr (qpl::is_same<F, float>()) {
				return qpl::f32_exponent_size();
			}
			else if constexpr (qpl::is_same<F, double>()) {
				return qpl::f64_exponent_size();
			}
		}
		else if constexpr (qpl::is_qpl_floating_point<F>()) {
			return F::exponent_bit_size();
		}
		else {
			return 0;
		}
	}

	template<class i1, class i2>
	using superior_floating_point =
		qpl::conditional<
		qpl::if_true<(qpl::mantissa_bit_size<i2>() > qpl::mantissa_bit_size<i1>())>, i2,
		qpl::if_true<(qpl::mantissa_bit_size<i1>() > qpl::mantissa_bit_size<i2>())>, i1,
		qpl::if_true<(qpl::exponent_bit_size<i2>() > qpl::exponent_bit_size<i1>())>, i2, i1>;


	template<class i1, class i2>
	using superior_integer =
		qpl::conditional<qpl::if_true<(i2::used_bit_size() > i1::used_bit_size())>, i2, i1>;

	template<class i1, class i2>
	using superior_dynamic_integer =
		qpl::conditional<qpl::if_true<i2::optimal_base()>, i2, i1>;


	namespace detail {
		//struct superior_arithmetic_type;
		template<typename T, typename U>
		using superior_arithmetic_type = typename qpl::conditional<
			qpl::if_true<(qpl::is_floating_point<T>() && qpl::is_floating_point<U>())>, qpl::superior_floating_point<T, U>,
			qpl::if_true<(qpl::is_floating_point<T>())>, T,
			qpl::if_true<(qpl::is_floating_point<U>())>, U,
			qpl::if_true<(qpl::type_used_bit_size<T>() < qpl::type_used_bit_size<U>())>, U, T>;


		template<typename T1>
		constexpr auto superior_arithmetic_types() {
			return qpl::declval<T1>();
		}
		template<typename T1, typename T2>
		constexpr auto superior_arithmetic_types() {
			return qpl::declval<superior_arithmetic_type<T1, T2>>();
		}
		template<typename T1, typename T2, typename T3, typename ... Ts>
		constexpr auto superior_arithmetic_types() {
			return superior_arithmetic_types<decltype(superior_arithmetic_types<T1, T2>()), decltype(superior_arithmetic_types<T3, Ts...>())>();
		}
	}

	template<typename ... Ts>
	using superior_arithmetic_type = decltype(detail::superior_arithmetic_types<Ts...>());

	template<typename T, typename U>
	using promote_superior_type_classic =
		qpl::conditional<qpl::if_true<(qpl::bytes_in_type<T>() == 8u || qpl::bytes_in_type<U>() == 8u)>, qpl::f64,
		qpl::conditional<qpl::if_true<(qpl::is_floating_point<qpl::superior_arithmetic_type<T, U>>())>, qpl::superior_arithmetic_type<T, U>,
		qpl::conditional<qpl::if_true<(qpl::type_used_bit_size<qpl::superior_arithmetic_type<T, U>>() < qpl::type_used_bit_size<qpl::i16>())>, std::conditional_t<qpl::is_signed<qpl::superior_arithmetic_type<T, U>>(), qpl::i16, qpl::u16>,
		qpl::conditional<qpl::if_true<(qpl::type_used_bit_size<qpl::superior_arithmetic_type<T, U>>() < qpl::type_used_bit_size<qpl::i32>())>, std::conditional_t<qpl::is_signed<qpl::superior_arithmetic_type<T, U>>(), qpl::i32, qpl::u32>,
		qpl::conditional<qpl::if_true<(qpl::type_used_bit_size<qpl::superior_arithmetic_type<T, U>>() < qpl::type_used_bit_size<qpl::i64>())>, std::conditional_t<qpl::is_signed<qpl::superior_arithmetic_type<T, U>>(), qpl::i64, qpl::u64>,
		qpl::default_type, qpl::f64>>>>>;

	template<typename T, typename U, typename R = promote_superior_type_classic<T, U>>
	constexpr R safe_addition(T a, U b) {
		return static_cast<R>(a) + static_cast<R>(b);
	}
	template<typename T, typename U, typename R = promote_superior_type_classic<T, U>>
	constexpr R safe_multiplication(T a, U b) {
		return static_cast<R>(a) * static_cast<R>(b);
	}

}

#endif