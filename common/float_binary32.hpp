#pragma once

#define FT_ASSERT(expected) if((expected) == 0) { __debugbreak(); }

const uint32_t MAX_FRACTION = 0x7FFFFF;

inline uint32_t as_uint32(float f) {
	return *reinterpret_cast<uint32_t *>(&f);
}
inline float as_float(uint32_t u) {
	return *reinterpret_cast<float *>(&u);
}

inline float binary32_encode(bool sign_bit, uint8_t expornent, uint32_t significand) {
	FT_ASSERT(significand < 0x800000);
	return as_float((sign_bit ? 0x80000000 : 0) | (static_cast<uint32_t>(expornent) << 23) | significand);
}

inline uint8_t exponent(float f) {
	uint32_t e = as_uint32(f) & 0x7F800000;
	return e >> 23;
}
