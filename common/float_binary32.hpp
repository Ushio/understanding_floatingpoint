#pragma once

#include <algorithm>

#define FT_ASSERT(expected) if((expected) == 0) { __debugbreak(); }

const uint32_t MAX_FRACTION = 0x7FFFFF;
const bool PLUS_SIGN_BIT  = false;
const bool MINUS_SIGN_BIT = true;

inline uint32_t as_uint32(float f) {
	return *reinterpret_cast<uint32_t *>(&f);
}
inline float as_float(uint32_t u) {
	return *reinterpret_cast<float *>(&u);
}

inline float binary32_encode(bool signbit, uint8_t expornent, uint32_t significand) {
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

inline float ulp_normalized(float f) {
	int32_t e = get_exponent(f);
	return binary32_encode(PLUS_SIGN_BIT, std::max(e - 23, 0), 0);
}
