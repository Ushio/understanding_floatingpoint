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

	inline float next_float_up(float f) {
		if (std::isnan(f)) {
			return f;
		}
		if (f == encode(PLUS_SIGN_BIT, 255, 0)) {
			return f;
		}

		uint32_t u = as_uint32(f);

		// f is -0.0
		if (u == as_uint32(encode(MINUS_SIGN_BIT, 0, 0))) {
			return encode(PLUS_SIGN_BIT, 0, 1);
		}

		if (get_signbit(f) == false /* positive value */) {
			u++;
		}
		else {
			u--;
		}
		return as_float(u);
	}
	inline float next_float_down(float f) {
		if (std::isnan(f)) {
			return f;
		}
		if (f == encode(MINUS_SIGN_BIT, 255, 0)) {
			return f;
		}

		uint32_t u = as_uint32(f);

		// f is +0.0
		if (u == as_uint32(encode(PLUS_SIGN_BIT, 0, 0))) {
			return encode(MINUS_SIGN_BIT, 0, 1);
		}
		
		if (get_signbit(f) == false /* positive value */) {
			u--;
		}
		else {
			u++;
		}
		return as_float(u);
	}
}
