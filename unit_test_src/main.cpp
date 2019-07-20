#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>

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

}