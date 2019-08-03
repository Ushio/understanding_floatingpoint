#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <limits>
#include <cfenv>
#include "peseudo_random.hpp"
#include "float_binary32.hpp"

using namespace binary32;

int main() {
	static Catch::Session session;
	char* custom_argv[] = {
		"",
		"--break", /* enable break */
		"--durations",
		"yes",
		"--use-colour",
		"auto",

		//"any"
		"any"
	};
	session.run(sizeof(custom_argv) / sizeof(custom_argv[0]), custom_argv);
}

TEST_CASE("float_binary32_encode") {
	rt::Xoshiro128StarStar random;
	for (int i = 0; i <= MAX_FRACTION; ++i) {
		bool sign_bit        = random.uniform() < 0.5f;
		uint32_t significand = i;
		uint8_t exponent     = random.uniform_integer() % 256;
		float f = encode(
			sign_bit,
			exponent,
			significand
		);
		REQUIRE(sign_bit    == get_signbit(f));
		REQUIRE(exponent    == get_exponent(f));
		REQUIRE(significand == get_significand(f));
	}

	for (int i = 0; i <= MAX_FRACTION; ++i) {
		float f = encode(PLUS_SIGN_BIT, 127, i);
		REQUIRE(1.0f <= f);
		REQUIRE(f < 2.0f);
	}
}
TEST_CASE("ulp_just_normalized") {
	rt::Xoshiro128StarStar random;
	for (int i = 0; i <= 10000000; ++i) {
		// [0, MAX_FRACTION)
		uint32_t significand = random.uniform_integer() % (MAX_FRACTION + 1);

		// [0, 255)
		uint8_t exponent = random.uniform_integer() % 255;

		// next is +inf
		if (exponent == 254 && significand == MAX_FRACTION) {
			continue;
		}

		float f = encode(
			PLUS_SIGN_BIT,
			exponent,
			significand
		);

		float u = ulp_only_normal_number(f);
		if (can_ulp_store_normal_number(f)) {
			REQUIRE(f + u == std::nextafter(f, std::numeric_limits<float>::max()));
		}
		else {
			REQUIRE(f + u != std::nextafter(f, std::numeric_limits<float>::max()));
		}
	}
}

TEST_CASE("ulp") {
	rt::Xoshiro128StarStar random;
	for (int i = 0; i <= 1000000; ++i) {
		// [0, MAX_FRACTION)
		uint32_t significand = random.uniform_integer() % (MAX_FRACTION + 1);

		// [0, 255)
		uint8_t exponent = random.uniform_integer() % 255;

		// next is +inf
		if (exponent == 254 && significand == MAX_FRACTION) {
			continue;
		}

		float f = encode(
			PLUS_SIGN_BIT,
			exponent,
			significand
		);

		float u = ulp(f);
		REQUIRE(f + u == std::nextafter(f, std::numeric_limits<float>::max()));
	}
}

TEST_CASE("machine_epsilon") {
	REQUIRE(ulp(1.0f) == std::numeric_limits<float>::epsilon());
	REQUIRE(std::ldexp(1.0f, -23) == std::numeric_limits<float>::epsilon());
}

TEST_CASE("special_cases") {
	REQUIRE(-0.0f == +0.0f);
	REQUIRE(+0.0f >= -0.0f);
	REQUIRE(+0.0f <= -0.0f);
	REQUIRE((+0.0f < -0.0f) == false);
	REQUIRE((+0.0f > -0.0f) == false);

	REQUIRE(encode(PLUS_SIGN_BIT, 0, 0) == encode(MINUS_SIGN_BIT, 0, 0));
	REQUIRE(encode(MINUS_SIGN_BIT, 0, 0) == encode(PLUS_SIGN_BIT, 0, 0));

	REQUIRE(as_uint32(encode(PLUS_SIGN_BIT, 0, 0))  != as_uint32(encode(MINUS_SIGN_BIT, 0, 0)));
	REQUIRE(as_uint32(encode(MINUS_SIGN_BIT, 0, 0)) != as_uint32(encode(PLUS_SIGN_BIT, 0, 0)));

	REQUIRE(encode(PLUS_SIGN_BIT, 255, 0)  ==  std::numeric_limits<float>::infinity());
	REQUIRE(encode(MINUS_SIGN_BIT, 255, 0) == -std::numeric_limits<float>::infinity());
	REQUIRE(isnan(encode(MINUS_SIGN_BIT, 255, 1)));
}

TEST_CASE("next_float_up_down") {
	float cases[] = {
		encode(MINUS_SIGN_BIT, 0, 1),
		encode(MINUS_SIGN_BIT, 0, 0),
		encode(PLUS_SIGN_BIT, 0, 0),
		encode(PLUS_SIGN_BIT, 0, 1),
	};

	for (auto f : cases) {
		REQUIRE(next_float_up(f)   == std::nextafter(f,  std::numeric_limits<float>::max()));
		REQUIRE(next_float_down(f) == std::nextafter(f, -std::numeric_limits<float>::max()));
	}

	rt::Xoshiro128StarStar random;
	for (int i = 0; i <= 10000000; ++i) {
		bool sign_bit = random.uniform() < 0.5f;

		// [0, MAX_FRACTION)
		uint32_t significand = random.uniform_integer() % (MAX_FRACTION + 1);

		// [0, 255)
		uint8_t exponent = random.uniform_integer() % 255;

		// next is +inf
		if (exponent == 254 && significand == MAX_FRACTION) {
			continue;
		}

		float f = encode(
			sign_bit,
			exponent,
			significand
		);

		REQUIRE(next_float_up(f)   == std::nextafter(f,  std::numeric_limits<float>::max()));
		REQUIRE(next_float_down(f) == std::nextafter(f, -std::numeric_limits<float>::max()));
	}
}

TEST_CASE("integers") {
	int x = -16777216; /* 2^24 */
	for (float i = x; i <= 16777215.0f /* 2^24 */; i += 1.0f) {
		REQUIRE(i == x);
		++x;
	}
	REQUIRE(x == 16777216);
}
TEST_CASE("fesetround") {
	std::fesetround(FE_UPWARD);
	float x = 16777216.0f;
	REQUIRE(x + 1.0f == 16777218.0f);
}
TEST_CASE("memset") {
	float x;
	memset(&x, 0, sizeof(float));
	REQUIRE(x == encode(PLUS_SIGN_BIT, 0, 0));
	REQUIRE(as_uint32(x) == as_uint32(encode(PLUS_SIGN_BIT, 0, 0)));
}
TEST_CASE("int_as_float") {
	int a = as_uint32(-0.2f);
	int b = as_uint32(-0.1f);
	int c = as_uint32(+0.1f);
	int d = as_uint32(+0.2f);
	REQUIRE(a > b);
	REQUIRE(b < c);
	REQUIRE(c < d);
}

TEST_CASE("FLT_MIN") {
	REQUIRE(FLT_MIN == encode(PLUS_SIGN_BIT, 1, 0));
}
TEST_CASE("any") {
	REQUIRE(FLT_MIN == encode(PLUS_SIGN_BIT, 1, 0));
}