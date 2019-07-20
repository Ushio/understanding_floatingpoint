#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include "peseudo_random.hpp"
#include "float_binary32.hpp"

int main() {
	static Catch::Session session;
	char* custom_argv[] = {
		"",
		"--break", /* enable break */
		"--durations",
		"yes",
		"--use-colour",
		"auto",

		"",
	};
	session.run(sizeof(custom_argv) / sizeof(custom_argv[0]), custom_argv);
}

TEST_CASE("float_binary32") {
	rt::Xoshiro128StarStar random;
	for (int i = 0; i <= MAX_FRACTION; ++i) {
		bool sign_bit        = random.uniform() < 0.5f ? PLUS_SIGN_BIT : MINUS_SIGN_BIT;
		uint32_t significand = i;
		uint8_t exponent     = random.uniform_integer() % 256;
		float f = binary32_encode(
			sign_bit,
			exponent,
			significand
		);
		REQUIRE(sign_bit    == get_signbit(f));
		REQUIRE(exponent    == get_exponent(f));
		REQUIRE(significand == get_significand(f));
	}

	for (int i = 0; i <= MAX_FRACTION; ++i) {
		float f = binary32_encode(PLUS_SIGN_BIT, 127, i);
		REQUIRE(1.0f <= f);
		REQUIRE(f < 2.0f);
	}
}
