#ifndef QPL_TYPETRAITS_HPP
#define QPL_TYPETRAITS_HPP
#pragma once

#include <type_traits>
#include <limits>
#include <tuple>
#include <string>
#include <span>
#include <limits>
#include <utility>
#include <typeinfo>
#include <charconv>
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <set>
#include <map>

namespace qpl {
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
	constexpr bool is_stl_integer() {
		return std::numeric_limits<T>::is_integer;
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
		return CHAR_BIT;
	}

	template<typename T>
	constexpr qpl::size bits_in_type() {
		return sizeof(T) * bits_in_byte();
	}

	template<typename T>
	constexpr qpl::size bytes_in_type() {
		return sizeof(T);
	}

	template<class T, class... Ignored>
	struct identity {
		using type = typename T;
	};

	template<class... Args>
	struct empty_indirection {
		using type = typename identity<Args...>::type;
	};

	template<typename T, typename U>
	constexpr bool is_same() {
		return std::is_same_v<T, U>;
	}
	template<typename T, typename U>
	constexpr bool is_same_decayed() {
		return std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
	}


	template<typename T, typename U, typename... Args>
	constexpr bool all_equal() {
		return qpl::is_same<T, U>() && (qpl::is_same<T, Args>() && ...);
	}
	template<typename T, typename U, typename... Args>
	constexpr bool all_equal_decayed() {
		return qpl::is_same_decayed<T, U>() && (qpl::is_same_decayed<T, Args>() && ...);
	}
	template<class compare, class... Args>
	constexpr bool is_equal_to_any() {
		return (qpl::is_same<compare, Args>() || ...);
	}
	template<class compare, class... Args>
	constexpr bool is_equal_to_any_decayed() {
		return (qpl::is_same_decayed<compare, Args>() || ...);
	}


	template<typename T>
	constexpr bool is_string_type() {
		return qpl::is_equal_to_any_decayed<T, char, const char*, char*, const char[], std::string, std::string_view, wchar_t, const wchar_t*, const wchar_t[], wchar_t*, std::wstring, std::wstring_view>();
	}
	template<typename T>
	constexpr bool is_standard_string_type() {
		return qpl::is_equal_to_any_decayed<T, char, const char*, char*, const char[], std::string, std::string_view>();
	}
	template<typename T>
	constexpr bool is_wstring_type() {
		return qpl::is_equal_to_any_decayed<T, wchar_t, const wchar_t*, wchar_t*, const wchar_t[], std::wstring, std::wstring_view>();
	}

	template<typename T>
	constexpr bool is_long_string_type() {
		return qpl::is_equal_to_any_decayed<T, const char*, char*, const char[], std::string, std::string_view, const wchar_t*, const wchar_t[], wchar_t*, std::wstring, std::wstring_view>();
	}
	template<typename T>
	constexpr bool is_long_standard_string_type() {
		return qpl::is_equal_to_any_decayed<T, const char*, char*, const char[], std::string, std::string_view>();
	}
	template<typename T>
	constexpr bool is_long_wstring_type() {
		return qpl::is_equal_to_any_decayed<T, const wchar_t*, wchar_t*, const wchar_t[], std::wstring, std::wstring_view>();
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

	struct true_type {};
	struct false_type {};
	struct default_type {};
	using error_type = std::enable_if<false, void>;
	using default_error = error_type;

	template<bool condition>
	using if_true = std::conditional_t<condition, true_type, false_type>;

	template<class T>
	constexpr bool is_truth_type() {
		return qpl::is_equal_to_any<T, true_type, false_type>();
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
	using conditional = typename std::conditional_t<std::is_same_v<qpl::resolved_conditional<Truth, T, Args...>, qpl::error_type>, qpl::error_type, conditional_impl<Truth, T, Args...>>::type;
	

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

	template<typename T>
	using signed_cast_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 8 }>, qpl::i8,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 16 }>, qpl::i16,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::i32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::i64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using unsigned_cast_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 8 }>, qpl::u8,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 16 }>, qpl::u16,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::u32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::u64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using int_cast_type = qpl::conditional<
		qpl::if_true<qpl::is_arithmetic<std::decay_t<T>>()>,
			qpl::conditional<
				qpl::if_true<qpl::is_integer<T>()>, T,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 32 }>, qpl::i32,
				qpl::if_true<qpl::bits_in_type<T>() == std::size_t{ 64 }>, qpl::i64,
				qpl::default_error>,
		qpl::default_error>;
	
	template<typename T>
	using float_cast_type = qpl::conditional<
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
	using container_subtype = std::decay_t<decltype(*(std::declval<C>().begin()))>;

	template<typename C> requires (qpl::is_container<C>())
	using container_subtype_with_const = std::remove_reference_t<decltype(*(std::declval<C>().begin()))>;

	template<typename C> requires (qpl::is_container<C>())
	using container_subtype_with_reference = std::remove_const_t<decltype(*(std::declval<C>().begin()))>;

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
		return decltype(impl::span_size(std::declval<T>()))::value;
	}
	template<typename T>
	struct span_size_type {
		using type = typename decltype(impl::span_size(std::declval<T>()));
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
	concept has_square_brackets_c = requires(T & x, const T & y) {
		x[qpl::size{}] = qpl::container_subtype<T>{};
		y[qpl::size{}];
	};
	template<typename T>
	constexpr bool has_square_brackets() {
		return has_square_brackets_c<T>;
	}

	template<typename T>
	concept has_at_c = requires(T & x, const T & y) {
		x.at(qpl::size{}) = qpl::container_subtype<T>{};
		y.at(qpl::size{});
	};
	template<typename T>
	constexpr bool has_at() {
		return has_at_c<T>;
	}

	template<typename T>
	concept has_resize_and_access_c = has_resize_c<T> && (has_square_brackets_c<T> || has_at_c<T>);

	template<typename T>
	constexpr bool has_resize_and_access() {
		return has_resize_and_access_c<T>;
	}


	template<typename T>
	concept has_pushback_c = requires(T & x) {
		x.push_back(qpl::container_subtype<T>{});
	};
	template<typename T>
	constexpr bool has_pushback() {
		return has_pushback_c<T>;
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
	concept can_grow_c = has_pushback_c<T> || has_insert_c<T>;

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
			using type = qpl::error_type;
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
		return qpl::is_same_decayed<impl::has_iterator_concept_type<T>::type, std::contiguous_iterator_tag>() && qpl::has_data<T>();
	}
	template<typename T>
	concept is_contiguous_container_c = (is_contiguous_container<T>());



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
		struct has_allocator_type {
			constexpr static bool value = false;
			using type = qpl::error_type;
		};

		template<typename T>
		struct has_allocator_type<T, std::void_t<typename T::allocator_type>> {
			constexpr static bool value = true;
			using type = typename T::allocator_type;
		};

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
	constexpr bool has_allocator_type() {
		return impl::has_allocator_type<T>::value;
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
		auto container_subtype_cast(const container<T, rest...>& c) {
			if constexpr (qpl::is_container<T>()) {
				return container<decltype(container_subtype_cast<U>(T{}))>{};
			}
			else {
				return container<U>{};
			}
		}

		template<typename R, typename... A>
		constexpr R return_type(R(*)(A...)) {
			return R{};
		}
		template<typename C, typename R, typename... A>
		constexpr R return_type(R(C::*)(A...)) {
			return R{};
		}
		template<typename C, typename R, typename... A>
		constexpr R return_type(R(C::*)(A...) const) {
			return R{};
		}
		template<typename C, typename R, typename... A>
		constexpr C method_class_type(R(C::*)(A...)) {
			return C{};
		}
		template<typename C, typename R, typename... A>
		constexpr C method_class_type(R(C::*)(A...) const) {
			return C{};
		}

		template<typename R, typename... A>
		constexpr auto parameter_type(R(*)(A...)) {
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


		template<typename R, typename... A>
		constexpr auto function_type(R(*)(A...)) {
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


		template<typename R, typename... A>
		constexpr auto make_function() {
			return std::declval<R(*)(A...)>();
		}

		template<typename C, typename R, typename... A>
		constexpr auto make_method_help() {
			return std::declval<R(C::*)(A...)>();
		}

		template<typename C, typename F>
		constexpr auto make_method() {
			constexpr auto unpack_tuple = [&]<class C, class Tuple, size_t... Ints>(std::index_sequence<Ints...>) {
				return make_method_help<C, std::tuple_element_t<Ints, Tuple>...>();
			};
			using function_type = decltype(impl::function_type(std::declval<F>()));
			return unpack_tuple.template operator()<C, function_type>(std::make_index_sequence<std::tuple_size_v<function_type>>{});
		}


		template<typename C, typename F, typename = void>
		struct has_method : std::false_type {};
		template<typename C, typename F>
		struct has_method<C, F, std::void_t<decltype(make_method<C, F>())>> : std::true_type {};

	}



	template<typename C, typename T>
	struct container_subtype_cast_t {
		using type = decltype(impl::container_subtype_cast<T>(std::declval<C>()));
	}; 

	template<typename C, typename T>
	using container_subtype_cast = typename container_subtype_cast_t<C, T>::type;


	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_sorted_container() {
		if constexpr (qpl::has_key_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
			if constexpr (qpl::is_same_decayed<C, std::set<qpl::container_key_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
				return true;
			}
		}
		if constexpr (qpl::has_key_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
			if constexpr (qpl::is_same_decayed<C, std::multiset<qpl::container_key_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
				return true;
			}
		}
		if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
			if constexpr (qpl::is_same_decayed<C, std::map<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
				return true;
			}
		}
		if constexpr (qpl::has_key_type<C>() && qpl::has_mapped_type<C>() && qpl::has_key_compare_type<C>() && qpl::has_allocator_type<C>()) {
			if constexpr (qpl::is_same_decayed<C, std::multimap<qpl::container_key_type<C>, qpl::container_mapped_type<C>, qpl::container_key_compare_type<C>, qpl::container_allocator_type<C>>>()) {
				return true;
			}
		}
		return false;
	}

	template<typename C> requires (qpl::is_container<C>())
	constexpr bool is_unsorted_container() {
		return !qpl::is_sorted_container<C>();
	}


	template<typename T, typename U>
	constexpr inline auto type_cast(U&& data) {
		if constexpr (qpl::is_container<U>()) {
			typename qpl::container_subtype_cast_t<U, T>::type result;
			if constexpr (qpl::has_resize_and_access<qpl::container_subtype_cast_t<U, T>::type>()) {
				result.resize(data.size());
				for (qpl::size i = 0u; i < data.size(); ++i) {
					if constexpr (qpl::has_square_brackets<qpl::container_subtype_cast_t<U, T>::type>()) {
						result[i] = qpl::type_cast<T>(data[i]);
					}
					else if constexpr (qpl::has_at<qpl::container_subtype_cast_t<U, T>::type>()) {
						result.at(i) = qpl::type_cast<T>(data.at(i));
					}
					else {
						static_assert("type_cast: T has no [] nor .at()");
					}
				}
			}
			else {
				for (const auto& i : data) {
					if constexpr (qpl::has_pushback<qpl::container_subtype_cast_t<U, T>::type>()) {
						result.push_back(qpl::type_cast<T>(i));
					}
					else if constexpr (qpl::has_insert<qpl::container_subtype_cast_t<U, T>::type>()) {
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
			std::wstring_view sv{ data };
			T value;
			std::from_chars(sv.data(), sv.data() + sv.size(), value);
			return value;
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
	constexpr inline auto signed_size_cast(T&& data) {
		return static_cast<qpl::signed_cast_type<qpl::size>>(data);
	}
	template<typename T>
	constexpr inline auto char_cast(T&& data) {
		return qpl::type_cast<qpl::char_type>(data);
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
		return qpl::type_cast<signed_cast_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto unsigned_cast(T&& value) {
		return qpl::type_cast<unsigned_cast_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto int_cast(T&& value) {
		return qpl::type_cast<int_cast_type<T>>(value);
	}
	template<typename T>
	constexpr inline auto float_cast(T&& value) {
		return qpl::type_cast<float_cast_type<T>>(value);
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

	template<typename... Args>
	using make_function = decltype(impl::make_function<Args...>());

	template<typename C, typename F>
	using make_method = decltype(impl::make_method<C, F>());


	template<typename C, typename F>
	constexpr bool has_method() {
		return impl::has_method<C, F>::value;
	}

	template<typename F>
	using return_type = decltype(impl::return_type(std::declval<F>()));

	template<typename F>
	using parameter_type = decltype(impl::parameter_type(std::declval<F>()));

	template<typename F>
	using method_class_type = decltype(impl::method_class_type(std::declval<F>()));

	template<typename F>
	using function_type = decltype(impl::function_type(std::declval<F>()));


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




	template<typename T, typename U>
	using superior_arithmetic_type = 
		qpl::conditional<
		qpl::if_true<(qpl::is_floating_point<T>() && qpl::is_floating_point<U>())>, qpl::superior_floating_point<T, U>,
		qpl::if_true<(qpl::is_floating_point<T>())>, T,
		qpl::if_true<(qpl::is_floating_point<U>())>, U,
		qpl::if_true<(qpl::type_used_bit_size<T>() < qpl::type_used_bit_size<U>())>, U, T>;



	//template<typename T, typename U>
	//constexpr inline qpl::superior_arithmetic_type<T, U> superior_arithmetic_cast(T&& data) {
	//	return qpl::type_cast<qpl::superior_arithmetic_type<T, U>>(data);
	//}

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

	//template<typename C>
	//struct container_deepest_type_impl {
	//	using type = typename qpl::conditional<
	//		qpl::if_true<qpl::is_container<qpl::container_subtype<C>>()>, container_deepest_type_impl<qpl::container_subtype<C>>::type,
	//		qpl::if_true<qpl::is_container<C>>, qpl::container_subtype<C>,
	//		qpl::default_type, C>;
	//};
	//
	//
	//template<typename C>
	//using container_deepest_type = container_deepest_type_impl<C>::type;
}

#endif