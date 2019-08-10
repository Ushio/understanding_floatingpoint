#pragma once

#include <algorithm>

#define FT_ASSERT(expected) if((expected) == 0) { __debugbreak(); }

namespace binary32 {
	const uint32_t MAX_FRACTION = 0x7FFFFF;
	const bool PLUS_SIGN_BIT  = false;
	const bool MINUS_SIGN_BIT = true;

	inline uint32_t as_uint32(float f) {
		return *reinterpret_cast<uint32_t *>(&f);
	}
	inline float as_float(uint32_t u) {
		return *reinterpret_cast<float *>(&u);
	}

	inline float encode(bool signbit, uint8_t expornent, uint32_t significand) {
		FT_ASSERT(significand < 0x800000);
		return as_float((signbit ? 0x80000000 : 0) | (static_cast<uint32_t>(expornent) << 23) | significand);
	}

	inline uint8_t get_exponent(float f) {
		uint32_t e = as_uint32(f) & 0x7F800000;
		return e >> 23;
	}

	inline uint32_t get_significand(float f) {
		return as_uint32(f) & 0x7FFFFF;
	}

	inline float set_significand(float f, uint32_t significand) {
		FT_ASSERT(significand < 0x800000);
		return as_float((as_uint32(f) & 0xFF800000) | significand);
	}

	inline bool get_signbit(float f) {
		return (as_uint32(f) & 0x80000000) != 0;
	}

	inline bool can_ulp_store_normal_number(float f) {
		return 23 < get_exponent(f);
	}

	inline float ulp_only_normal_number(float f) {
		int32_t e = get_exponent(f);
		return encode(PLUS_SIGN_BIT, std::max(e - 23, 0), 0);
	}

	inline float ulp(float f) {
		int32_t e = get_exponent(f);
		if (23 < e) {
			return encode(PLUS_SIGN_BIT, e - 23, 0);
		}
		return encode(PLUS_SIGN_BIT, 0, 1 << std::max(e - 1, 0));
	}

	inline int32_t to_ordered(float f) {
		uint32_t b = as_uint32(f);

		uint32_t s = b & 0x80000000; // sign bit
		int32_t  x = b & 0x7FFFFFFF; // expornent and significand

		return s ? -x : x;
	}

	inline float from_ordered(int32_t ordered) {
		if (ordered < 0) {
			uint32_t x = -ordered;
			return as_float(x | 0x80000000);
		}
		return as_float(ordered);
	}
	inline float next_float(float x, int move) {
		return from_ordered(to_ordered(x) + move);
	}

	template <class T>
	float flulp_error(float x, T r) {
		T e = (T(x) - r) / T(ulp(float(r)));
		return std::fabs(float(e));
	}
}
