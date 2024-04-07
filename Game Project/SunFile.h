#pragma once
#include <memory>
#include "Error.h"
#include "Constants.h"
#include "SunNode.h"
#include <string>
#include <array>

namespace sun_file
{
	extern SunNode Map, String;
	extern std::string devFilePath;

	// List of all of the game files needed. If any of these files
	// are not present, the initialization will return an error and
	// the game will not run.
	constexpr std::array<const char*, constants::NUM_FILES> filenames =
	{
		"Map.sun",
		"String.sun"
	};

	// Initialize files before we can load them.
	// Does an integrity check on all files, as well.
	// If any files are missing we return an error.
	auto Init()->Error;

	// Load all SunFiles.
	auto Load_All_Files(const std::string& file_path) -> void; // -> trailing return type
}