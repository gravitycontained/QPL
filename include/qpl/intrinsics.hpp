#ifndef QPL_INTRINSICS_HPP
#define QPL_INTRINSICS_HPP
#pragma once

#include <intrin.h>
#include <immintrin.h>
#include <mmintrin.h>

#include <tuple>
#include <qpl/vardef.hpp>

namespace qpl {
	namespace intrin {
		template<typename T, typename U>
		constexpr inline std::pair<qpl::u32, qpl::u32> div_mod(T a, U b) {
			qpl::u32 div, mod;
			div = _udiv64(qpl::u64_cast(a), qpl::u32_cast(b), &mod);
			return std::make_pair(div, mod);
		}
	}
}

#endif