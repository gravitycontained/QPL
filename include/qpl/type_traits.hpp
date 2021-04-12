#ifndef QPL_TYPETRAITS_HPP
#define QPL_TYPETRAITS_HPP
#pragma once

#include <type_traits>
#include <limits>
#include <tuple>
#include <string>
#include <limits>
#include <utility>
#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>

#define QPLCONCEPT(x) std::enable_if_t<x>* = nullptr

namespace qpl {
	template<qpl::u32 base, bool sign>
	struct dynamic_integer;

	template<qpl::size bits, bool sign>
	struct integer;


	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct floating_point;
	
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


	template<typename T>
	struct is_qpl_floating_point_impl : std::false_type
	{};

	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct is_qpl_floating_point_impl<qpl::floating_point<exponent_bits, mantissa_bits>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_qpl_floating_point() {
		return is_qpl_floating_point_impl<T>{};
	}

	template<typename T>
	struct is_qpl_dynamic_integer_impl : std::false_type
	{};

	template<qpl::u32 base, bool sign>
	struct is_qpl_dynamic_integer_impl<qpl::dynamic_integer<base, sign>> : std::true_type
	{};

	template<typename T>
	constexpr bool is_qpl_dynamic_integer() {
		return is_qpl_dynamic_integer_impl<T>{};
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


	constexpr qpl::u64 bits_in_byte() {
		return CHAR_BIT;
	}

	template<typename T>
	constexpr qpl::u64 bits_in_type() {
		return sizeof(T) * bits_in_byte();
	}

	template<typename T>
	constexpr qpl::u64 bytes_in_type() {
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
	template<typename T, typename U, typename... Args>
	constexpr bool all_equal() {
		return qpl::is_same<T, U>() && (qpl::is_same<T, Args>() && ...);
	}
	template<typename T, typename U>
	constexpr bool is_same_decayed() {
		return std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
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


	template<typename T>
	constexpr inline auto type_cast(T&& data) {
		return static_cast<T>(data);
	}
	template<typename T>
	constexpr inline qpl::u8 u8_cast(T&& data) {
		return static_cast<qpl::u8>(data);
	}
	template<typename T>
	constexpr inline qpl::i8 i8_cast(T&& data) {
		return static_cast<qpl::i8>(data);
	}

	template<typename T>
	constexpr inline qpl::u16 u16_cast(T&& data) {
		return static_cast<qpl::u16>(data);
	}
	template<typename T>
	constexpr inline qpl::i16 i16_cast(T&& data) {
		return static_cast<qpl::i16>(data);
	}

	template<typename T>
	constexpr inline qpl::u32 u32_cast(T&& data) {
		return static_cast<qpl::u32>(data);
	}

	template<typename T>
	constexpr inline qpl::i32 i32_cast(T&& data) {
		return static_cast<qpl::i32>(data);
	}

	template<typename T>
	constexpr inline qpl::u64 u64_cast(T&& data) {
		return static_cast<qpl::u64>(data);
	}
	template<typename T>
	constexpr inline qpl::i64 i64_cast(T&& data) {
		return static_cast<qpl::i64>(data);
	}

	template<typename T>
	constexpr inline qpl::u128 u128_cast(T&& data) {
		return static_cast<qpl::u128>(data);
	}
	template<typename T>
	constexpr inline qpl::i128 i128_cast(T&& data) {
		return static_cast<qpl::i128>(data);
	}

	template<typename T>
	constexpr inline qpl::u256 u256_cast(T&& data) {
		return static_cast<qpl::u256>(data);
	}
	template<typename T>
	constexpr inline qpl::i256 i256_cast(T&& data) {
		return static_cast<qpl::i256>(data);
	}

	template<typename T>
	constexpr inline qpl::u512 u512_cast(T&& data) {
		return static_cast<qpl::u512>(data);
	}
	template<typename T>
	constexpr inline qpl::i512 i512_cast(T&& data) {
		return static_cast<qpl::i512>(data);
	}


	template<typename T>
	inline qpl::u u_cast(T&& data) {
		return static_cast<qpl::u>(data);
	}
	template<typename T>
	inline qpl::i i_cast(T&& data) {
		return static_cast<qpl::i>(data);
	}
	template<typename T>
	inline qpl::uh uh_cast(T&& data) {
		return static_cast<qpl::uh>(data);
	}
	template<typename T>
	inline qpl::ih ih_cast(T&& data) {
		return static_cast<qpl::ih>(data);
	}
	template<typename T>
	inline qpl::ub ub_cast(T&& data) {
		return static_cast<qpl::ub>(data);
	}
	template<typename T>
	inline qpl::ib ib_cast(T&& data) {
		return static_cast<qpl::ib>(data);
	}
	template<qpl::u32 base, typename T>
	inline qpl::ubase<base> ubase_cast(T&& data) {
		return static_cast<qpl::ubase<base>>(data);
	}
	template<qpl::u32 base, typename T>
	inline qpl::ibase<base> ibase_cast(T&& data) {
		return static_cast<qpl::ibase<base>>(data);
	}

	template<typename T>
	constexpr inline qpl::size size_cast(T&& data) {
		return static_cast<qpl::size>(data);
	}
	template<typename T>
	constexpr inline qpl::f32 f32_cast(T&& data) {
		return static_cast<qpl::f32>(data);
	}
	template<typename T>
	constexpr inline qpl::f64 f64_cast(T&& data) {
		return static_cast<qpl::f64>(data);
	}
	template<typename T>
	constexpr inline bool bool_cast(T&& data) {
		return static_cast<bool>(data);
	}




	template<typename T>
	auto signed_cast(T&& value) {
		return static_cast<signed_cast_type<T>>(value);
	}
	template<typename T>
	auto unsigned_cast(T&& value) {
		return static_cast<unsigned_cast_type<T>>(value);
	}
	template<typename T>
	auto int_cast(T&& value) {
		return static_cast<int_cast_type<T>>(value);
	}
	template<typename T>
	auto float_cast(T&& value) {
		return static_cast<float_cast_type<T>>(value);
	}

	template<typename T, typename U>
	constexpr bool equals_type(U&& value = U{}) {
		return std::is_same_v<std::decay_t<U>, std::decay_t<T>>;
	}

	template<typename T>
	concept is_container = requires(T a, const T b) {
		a.begin();
		a.end();
		b.cbegin();
		b.cend();
	};
	template<typename T>
	concept is_vector_like = requires(T a, const T b) {
		requires is_container<T>;
		a.resize(0u);
		a.data();
		b.size();
	};


	template<typename T>
	concept has_size = requires(const T x) {
		x.size();
	};


	namespace impl {

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


	template<typename C>
	using container_subtype = qpl::conditional<
		qpl::if_true<qpl::is_container<C>>, 
		std::decay_t<decltype(*(std::declval<C>().begin()))>, 
		qpl::default_error>;

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