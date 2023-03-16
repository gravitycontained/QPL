#pragma once
#ifndef QPL_VARDEF_HPP
#define QPL_VARDEF_HPP

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <cstdint>
#include <tuple>
#include <limits>
#include <string>


namespace qpl {
	using size = std::size_t;
	using isize = std::conditional_t<sizeof(std::size_t) == 8u, std::int64_t, std::int32_t>;

	using i8 = std::int8_t;
	using u8 = std::uint8_t;

	using i16 = std::int16_t;
	using u16 = std::uint16_t;

	using i32 = std::int32_t;
	using u32 = std::uint32_t;


	template<qpl::u32 base, bool sign>
	struct dynamic_integer;
	template<qpl::size bits, bool sign>
	struct integer;
	template<qpl::size bits, bool sign>
	struct x64_integer;
	template<typename E>
	struct big_float_t;

#ifndef QPL_NO_FLOATS
	template<qpl::size exponent_bits, qpl::size mantissa_bits>
	struct floating_point;
#endif

	using ii32 = qpl::integer<32, 1>;
	using uu32 = qpl::integer<32, 0>;

	using i64 = std::int64_t;
	using u64 = std::uint64_t;

	using ii64 = qpl::integer<64, 1>;
	using uu64 = qpl::integer<64, 0>;

	using i96 = qpl::integer<96, 1>;
	using u96 = qpl::integer<96, 1>;

	using i128 = qpl::integer<128, 1>;
	using u128 = qpl::integer<128, 0>;

	using i160 = qpl::integer<160, 1>;
	using u160 = qpl::integer<160, 0>;

	using i192 = qpl::integer<192, 1>;
	using u192 = qpl::integer<192, 0>;

	using i256 = qpl::integer<256, 1>;
	using u256 = qpl::integer<256, 0>;

	using i320 = qpl::integer<320, 1>;
	using u320 = qpl::integer<320, 0>;

	using i512 = qpl::integer<512, 1>;
	using u512 = qpl::integer<512, 0>;

	using i640 = qpl::integer<640, 1>;
	using u640 = qpl::integer<640, 0>;

	using i768 = qpl::integer<768, 1>;
	using u768 = qpl::integer<768, 0>;

	using i1024 = qpl::integer<1024, 1>;
	using u1024 = qpl::integer<1024, 0>;

	using i2048 = qpl::integer<2048, 1>;
	using u2048 = qpl::integer<2048, 0>;

	using i4096 = qpl::integer<4096, 1>;
	using u4096 = qpl::integer<4096, 0>;

	using i8192 = qpl::integer<8192, 1>;
	using u8192 = qpl::integer<8192, 0>;

	using i10240 = qpl::integer<10240, 1>;
	using u10240 = qpl::integer<10240, 0>;

	using i20480 = qpl::integer<20480, 1>;
	using u20480 = qpl::integer<20480, 0>;

	using i32768 = qpl::integer<32768, 1>;
	using u32768 = qpl::integer<32768, 0>;

	using i65536 = qpl::integer<65536, 1>;
	using u65536 = qpl::integer<65536, 0>;

	using i102400 = qpl::integer<102400, 1>;
	using u102400 = qpl::integer<102400, 0>;

	using i204800 = qpl::integer<204800, 1>;
	using u204800 = qpl::integer<204800, 0>;

	using ux128 = qpl::x64_integer<128, 0>;
	using ix128 = qpl::x64_integer<128, 1>;

	using ux192 = qpl::x64_integer<192, 0>;
	using ix192 = qpl::x64_integer<192, 1>;

	using ux256 = qpl::x64_integer<256, 0>;
	using ix256 = qpl::x64_integer<256, 1>;

	using ux320 = qpl::x64_integer<320, 0>;
	using ix320 = qpl::x64_integer<320, 1>;

	using ux512 = qpl::x64_integer<512, 0>;
	using ix512 = qpl::x64_integer<512, 1>;

	using ux768 = qpl::x64_integer<768, 0>;
	using ix768 = qpl::x64_integer<768, 1>;

	using ux1024 = qpl::x64_integer<1024, 0>;
	using ix1024 = qpl::x64_integer<1024, 1>;

	using ux2048 = qpl::x64_integer<2048, 0>;
	using ix2048 = qpl::x64_integer<2048, 1>;

	using ux4096 = qpl::x64_integer<4096, 0>;
	using ix4096 = qpl::x64_integer<4096, 1>;

	using ux8192 = qpl::x64_integer<8192, 0>;
	using ix8192 = qpl::x64_integer<8192, 1>;

	using ix10240 = qpl::x64_integer<10240, 1>;
	using ux10240 = qpl::x64_integer<10240, 0>;

	using ix20480 = qpl::x64_integer<20480, 1>;
	using ux20480 = qpl::x64_integer<20480, 0>;

	using ix32768 = qpl::x64_integer<65536, 1>;
	using ux32768 = qpl::x64_integer<65536, 0>;

	using ix65536 = qpl::x64_integer<65536, 1>;
	using ux65536 = qpl::x64_integer<65536, 0>;

	using ix102400 = qpl::x64_integer<102400, 1>;
	using ux102400 = qpl::x64_integer<102400, 0>;

	using ix204800 = qpl::x64_integer<204800, 1>;
	using ux204800 = qpl::x64_integer<204800, 0>;


	template<qpl::u32 base>
	using ubase = qpl::dynamic_integer<base, 0>;
	template<qpl::u32 base>
	using ibase = qpl::dynamic_integer<base, 1>;

	using u = qpl::dynamic_integer<10u, 0>;
	using i = qpl::dynamic_integer<10u, 1>;

	using ub = qpl::dynamic_integer<2u, 0>;
	using ib = qpl::dynamic_integer<2u, 1>;

	using uh = qpl::dynamic_integer<16u, 0>;
	using ih = qpl::dynamic_integer<16u, 1>;

	using itypes = std::tuple<qpl::i8, qpl::i16, qpl::i32, qpl::i64>;
	using itype_min = std::tuple_element_t<0, itypes>;
	using itype_max = std::tuple_element_t<std::tuple_size_v<itypes> -1, itypes>;

	using utypes = std::tuple<qpl::u8, qpl::u16, qpl::u32, qpl::u64>;
	using utype_min = std::tuple_element_t<0, utypes>;
	using utype_max = std::tuple_element_t<std::tuple_size_v<utypes> -1, utypes>;

	using f32 = float;
	using f64 = double;

	using u32_string = std::basic_string<qpl::u32>;

#ifndef QPL_NO_FLOATS
	using ff32 = qpl::floating_point<32u, 32u>;
	using ff64 = qpl::floating_point<32u, 64u>;
	using f128 = qpl::floating_point<32u, 128u>;
	using f256 = qpl::floating_point<32u, 256u>;
	using f512 = qpl::floating_point<32u, 512u>;
	using f768 = qpl::floating_point<32u, 768u>;
	using f1024 = qpl::floating_point<32u, 1024u>;
	using f2048 = qpl::floating_point<32u, 2048u>;
	using f2560 = qpl::floating_point<32u, 2560u>;
	using f4096 = qpl::floating_point<32u, 4096u>;
	using f5120 = qpl::floating_point<32u, 5120u>;
	using f8192 = qpl::floating_point<32u, 8192u>;
	using f10240 = qpl::floating_point<32u, 10240u>;
	using f16384 = qpl::floating_point<32u, 16384u>;
	using f32768 = qpl::floating_point<32u, 32768u>;
#endif

	using xf32 = qpl::big_float_t<qpl::i32>;
	using xf64 = qpl::big_float_t<qpl::i64>;
	using xf128 = qpl::big_float_t<qpl::i128>;
	using xf192 = qpl::big_float_t<qpl::i192>;
	using xf256 = qpl::big_float_t<qpl::i256>;

	using ftypes = std::tuple<qpl::f32, qpl::f64>;
	using ftype_min = std::tuple_element_t<0, ftypes>;
	using ftype_max = std::tuple_element_t<std::tuple_size_v<ftypes> -1, ftypes>;

	constexpr qpl::i8 i8_min = std::numeric_limits<qpl::i8>::min();
	constexpr qpl::i8 i8_max = std::numeric_limits<qpl::i8>::max();
	constexpr qpl::u8 u8_min = std::numeric_limits<qpl::u8>::min();
	constexpr qpl::u8 u8_max = std::numeric_limits<qpl::u8>::max();

	constexpr qpl::i16 i16_min = std::numeric_limits<qpl::i16>::min();
	constexpr qpl::i16 i16_max = std::numeric_limits<qpl::i16>::max();
	constexpr qpl::u16 u16_min = std::numeric_limits<qpl::u16>::min();
	constexpr qpl::u16 u16_max = std::numeric_limits<qpl::u16>::max();

	constexpr qpl::i32 i32_min = std::numeric_limits<qpl::i32>::min();
	constexpr qpl::i32 i32_max = std::numeric_limits<qpl::i32>::max();
	constexpr qpl::u32 u32_min = std::numeric_limits<qpl::u32>::min();
	constexpr qpl::u32 u32_max = std::numeric_limits<qpl::u32>::max();

	constexpr qpl::i64 i64_min = std::numeric_limits<qpl::i64>::min();
	constexpr qpl::i64 i64_max = std::numeric_limits<qpl::i64>::max();
	constexpr qpl::u64 u64_min = std::numeric_limits<qpl::u64>::min();
	constexpr qpl::u64 u64_max = std::numeric_limits<qpl::u64>::max();

	constexpr qpl::f32 f32_min = std::numeric_limits<qpl::f32>::lowest();
	constexpr qpl::f32 f32_min_zero = std::numeric_limits<qpl::f32>::min();
	constexpr qpl::f32 f32_max = std::numeric_limits<qpl::f32>::max();
	constexpr qpl::f64 f64_min = std::numeric_limits<qpl::f64>::lowest();
	constexpr qpl::f64 f64_min_zero = std::numeric_limits<qpl::f64>::min();
	constexpr qpl::f64 f64_max = std::numeric_limits<qpl::f64>::max();




	constexpr qpl::f64 i8_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::f64 i8_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::f64 u8_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::f64 u8_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::f64 i16_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::f64 i16_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::f64 u16_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::f64 u16_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::f64 i32_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i32>::min());
	constexpr qpl::f64 i32_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i32>::max());
	constexpr qpl::f64 u32_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u32>::min());
	constexpr qpl::f64 u32_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u32>::max());

	constexpr qpl::f64 i64_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i64>::min());
	constexpr qpl::f64 i64_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::i64>::max());
	constexpr qpl::f64 u64_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u64>::min());
	constexpr qpl::f64 u64_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::u64>::max());

	constexpr qpl::f64 f32_min_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::f32>::min());
	constexpr qpl::f64 f32_max_f64 = static_cast<qpl::f64>(std::numeric_limits<qpl::f32>::max());



	constexpr qpl::f32 i8_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::f32 i8_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::f32 u8_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::f32 u8_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::f32 i16_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::f32 i16_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::f32 u16_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::f32 u16_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::f32 i32_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i32>::min());
	constexpr qpl::f32 i32_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i32>::max());
	constexpr qpl::f32 u32_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u32>::min());
	constexpr qpl::f32 u32_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u32>::max());

	constexpr qpl::f32 i64_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i64>::min());
	constexpr qpl::f32 i64_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::i64>::max());
	constexpr qpl::f32 u64_min_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u64>::min());
	constexpr qpl::f32 u64_max_f32 = static_cast<qpl::f32>(std::numeric_limits<qpl::u64>::max());





	constexpr qpl::u64 i8_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::u64 i8_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::u64 u8_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::u64 u8_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::u64 i16_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::u64 i16_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::u64 u16_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::u64 u16_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::u64 i32_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i32>::min());
	constexpr qpl::u64 i32_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::i32>::max());
	constexpr qpl::u64 u32_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u32>::min());
	constexpr qpl::u64 u32_max_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::u32>::max());

	constexpr qpl::u64 f32_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::f32>::min());
	constexpr qpl::u64 f64_min_u64 = static_cast<qpl::u64>(std::numeric_limits<qpl::f64>::min());



	constexpr qpl::i64 i8_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::i64 i8_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::i64 u8_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::i64 u8_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::i64 i16_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::i64 i16_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::i64 u16_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::i64 u16_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::i64 i32_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i32>::min());
	constexpr qpl::i64 i32_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::i32>::max());
	constexpr qpl::i64 u32_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u32>::min());
	constexpr qpl::i64 u32_max_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::u32>::max());

	constexpr qpl::i64 f32_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::f32>::min());
	constexpr qpl::i64 f64_min_i64 = static_cast<qpl::i64>(std::numeric_limits<qpl::f64>::min());



	constexpr qpl::u32 i8_min_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::u32 i8_max_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::u32 u8_min_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::u32 u8_max_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::u32 i16_min_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::u32 i16_max_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::u32 u16_min_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::u32 u16_max_u32 = static_cast<qpl::u32>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::i32 i8_min_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::i32 i8_max_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::i32 u8_min_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::i32 u8_max_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::i32 i16_min_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::i16>::min());
	constexpr qpl::i32 i16_max_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::i16>::max());
	constexpr qpl::i32 u16_min_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::u16>::min());
	constexpr qpl::i32 u16_max_i32 = static_cast<qpl::i32>(std::numeric_limits<qpl::u16>::max());

	constexpr qpl::u16 i8_min_u16 = static_cast<qpl::u16>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::u16 i8_max_u16 = static_cast<qpl::u16>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::u16 u8_min_u16 = static_cast<qpl::u16>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::u16 u8_max_u16 = static_cast<qpl::u16>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::i16 i8_min_i16 = static_cast<qpl::i16>(std::numeric_limits<qpl::i8>::min());
	constexpr qpl::i16 i8_max_i16 = static_cast<qpl::i16>(std::numeric_limits<qpl::i8>::max());
	constexpr qpl::i16 u8_min_i16 = static_cast<qpl::i16>(std::numeric_limits<qpl::u8>::min());
	constexpr qpl::i16 u8_max_i16 = static_cast<qpl::i16>(std::numeric_limits<qpl::u8>::max());

	constexpr qpl::size size_min = std::numeric_limits<qpl::size>::min();
	constexpr qpl::size size_max = std::numeric_limits<qpl::size>::max();

	constexpr qpl::isize isize_min = std::numeric_limits<qpl::isize>::min();
	constexpr qpl::isize isize_max = std::numeric_limits<qpl::isize>::max();

	constexpr qpl::f64 f64_epsilon = std::numeric_limits<qpl::f64>::epsilon();
	constexpr qpl::f64 f32_epsilon = std::numeric_limits<qpl::f32>::epsilon();

	constexpr qpl::size f64_digits = std::numeric_limits<qpl::f64>::max_digits10;
	constexpr qpl::size f32_digits = std::numeric_limits<qpl::f32>::max_digits10;

	constexpr qpl::f64 pi = 3.141592653589793238462;
	constexpr qpl::f32 pi_32 = static_cast<qpl::f32>(qpl::pi);
	constexpr qpl::f64 sqrt1_2 = 0.707106781186547524401;
	constexpr qpl::f32 sqrt1_2_32 = static_cast<qpl::f32>(qpl::sqrt1_2);
	constexpr qpl::f64 e = 2.718281828459045235360;
	constexpr qpl::f32 e_32 = static_cast<qpl::f32>(qpl::e);
	constexpr qpl::f64 ln2 = 0.69314718055994530941;
	constexpr qpl::f32 log2_32 = static_cast<qpl::f32>(qpl::ln2);
	constexpr qpl::f64 ln10 = 2.30258509299404568402;
	constexpr qpl::f32 log10_32 = static_cast<qpl::f32>(qpl::ln10);

	constexpr qpl::f64 f64_lower_bound10 = 9.99999999999999822;

	constexpr qpl::size f32_mantissa_size() {
		return 23;
	}
	constexpr qpl::size f32_exponent_size() {
		return 8;
	}
	constexpr qpl::size f64_mantissa_size() {
		return 52;
	}
	constexpr qpl::size f64_exponent_size() {
		return 11;
	}
}

#endif