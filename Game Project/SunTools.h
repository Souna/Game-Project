//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      // 
// Copyright ?2016-2017 Prime Zeng                                          // 
//                                                                          // 
// This program is free software: you can redistribute it and/or modify     // 
// it under the terms of the GNU Affero General Public License as           // 
// published by the Free Software Foundation, either version 3 of the       // 
// License, or (at your option) any later version.                          // 
//                                                                          // 
// This program is distributed in the hope that it will be useful,          // 
// but WITHOUT ANY WARRANTY; without even the implied warranty of           // 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            // 
// GNU Affero General Public License for more details.                      // 
//                                                                          // 
// You should have received a copy of the GNU Affero General Public License // 
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    // 
////////////////////////////////////////////////////////////////////////////// 

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
	struct 
	{
		uint32_t length = 0;
		uint32_t offset = 0;
		uint16_t width = 0;
		uint16_t height = 0;
	} bitmap;

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
	auto Split(const std::string& str, std::vector<std::string>& ret, std::string sep, bool allowNull) -> int32_t;
	auto EndsWith(const std::string& str, const std::string& endStr) -> bool;
	auto GetIndexFromTypes(std::string& str) -> int32_t;
};
