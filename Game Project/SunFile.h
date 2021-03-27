#pragma once
#include <memory>
#include "Error.h"
#include "SunNode.h"
#include <string>
#include <array>

namespace sun_file
{
	constexpr uint8_t NUM_FILES = 1;

	// SunFiles are referred to as 'nodes' because they themselves can be used like nodes.
	// Probably because of the master SunDirectory lmao idk
	// Base is our testing grounds and we're treating it as a SunNode =))))))))
	extern SunNode map;

	constexpr std::array<const char*, NUM_FILES> filenames = { "map.sun" };

	// Initialize files before_ we can load them.
	auto Init()->Error;

	// Load all SunFiles.
	auto Load_All_Files(const std::string& file_path) -> void; // -> trailing return type?
}