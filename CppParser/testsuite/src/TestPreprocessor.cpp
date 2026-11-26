//
// TestPreprocessor.cpp
//
// The program that UtilityTest starts in place of a preprocessor. It copies
// the file named by its last argument to the file named by its first
// argument, as a preprocessor writes the file it is given to the output file
// that its options name.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <span>
#include <system_error>


int main(int argc, char** argv)
{
	// The program name, the output file and the file to preprocess.
	constexpr std::size_t MIN_ARGUMENTS = 3;
	const std::span<char*> arguments(argv, static_cast<std::size_t>(argc));
	if (arguments.size() < MIN_ARGUMENTS) return EXIT_FAILURE;

	std::error_code error;
	std::filesystem::copy_file(arguments.back(), arguments[1], std::filesystem::copy_options::overwrite_existing, error);
	return error ? EXIT_FAILURE : EXIT_SUCCESS;
}
