//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once



#include <cstdint>
#include <string>
#include <unordered_map>
#include "Console.h"
#include "SunNode.h"

namespace game
{
	namespace string_conversion
	{
		template<typename T>
		inline T Or_Default(const std::string& str, T def)
		{
			try
			{
				int32_t intval = std::stoi(str);
				return static_cast<T>(intval);
			}
			catch (const std::exception& ex)
			{
				Console::Get().Print(__func__, ex);
				return def;
			}
		}

		template<typename T>
		inline T Or_Zero(const std::string& str)
		{
			return Or_Default<T>(str, T(0));
		}
	};

	namespace string_format
	{
		// Format a number string so that each 3 decimal points
		// are seperated by a ',' character.
		void Split_Number(std::string& input);

		// Prefix an id with zeroes so that it has the minimum specified length.
		std::string Extend_Id(int32_t id, size_t length);
	};

	namespace bytecode
	{
		// Check if a bit mask contains the specified value.
		bool Compare(int32_t mask, int32_t value);
	}

	namespace SunHelper
	{
		namespace Map
		{
			struct MapInfo
			{
				std::string description;
				std::string name;
				std::string street_name;
				std::string full_name;
			};

			// Returns all relative map info.
			MapInfo Get_Map_Info_By_Id(int32_t mapid);

			// Returns the category of a map.
			std::string Get_Map_Category(int32_t mapid);

			// Returns a list of all life on a map (Mobs and NPCs)
			std::unordered_map<int64_t, std::pair<std::string, std::string>> Get_Life_On_Map(int32_t mapid);

			// Returns the name of the node, under which the argument mapid is in.
			SunNode Get_Map_Node_Name(int32_t mapid);
		}
	}
}