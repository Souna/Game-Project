#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <cstdint>

typedef unsigned char byte;
class SunAudio;
class SunBitmap;
class SunNode;
class SunReader;

struct Data
{
	int64_t intreal = 0;
	double doublereal = 0.f;
	std::string str = "";
	int32_t vector[2] = { 0,0 };

	// Data related to the bitmaps.
	struct
	{
		uint32_t length = 0;
		uint32_t offset = 0;
		uint16_t width = 0;
		uint16_t height = 0;
	} bitmap;

	// Data related to audio.
	struct
	{
		uint32_t offset = 0;
		uint32_t length = 0;
		int32_t header = 0;
	} audio;

	bool use = false;
};

namespace sun_tools
{
	auto Split(const std::string& str, std::vector<std::string>& ret, std::string sep, bool allowNull)->int32_t;
	auto EndsWith(const std::string& str, const std::string& endStr) -> bool;
	auto GetIndexFromTypes(std::string& str)->int32_t;
};
