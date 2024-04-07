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
#include "Misc.h"

#include "SunNode.h"
#include "SunFile.h"

namespace game
{
	namespace string_format
	{
		void Split_Number(std::string& input)
		{
			for (size_t i = input.size(); i > 3; i -= 3)
				input.insert(i - 3, 1, ',');
		}

		std::string Extend_Id(int32_t id, size_t length)
		{
			std::string strid = std::to_string(id);

			if (strid.size() < length)
				strid.insert(0, length - strid.size(), '0');

			return strid;
		}
	}

	namespace bytecode
	{
		bool Compare(int32_t mask, int32_t value)
		{
			return (mask & value) != 0;
		}
	}

	namespace SunHelper
	{
		namespace Map
		{
			MapInfo Get_Map_Info_By_Id(int32_t mapid)
			{
				std::string map_category = Get_Map_Category(mapid);
				SunNode map_info = sun_file::String["Map.img"][map_category][mapid];

				return {
					map_info["mapDesc"],
					map_info["mapName"],
					map_info["streetName"],
					map_info["streetName"] + " : " + map_info["mapName"]
				};
			}

			std::string Get_Map_Category(int32_t mapid)
			{
				if (mapid < 100000000)
					return "Dev";

				if (mapid < 200000000)
					return "";

				if (mapid < 300000000)
					return "";

				if (mapid < 540000000)
					return "";

				if (mapid < 600000000)
					return "";

				return "etc";
			}

			std::unordered_map<int64_t, std::pair<std::string, std::string>> Get_Life_On_Map(int32_t mapid)
			{
				std::unordered_map<int64_t, std::pair<std::string, std::string>> map_life;

				SunNode portal = Get_Map_Node_Name(mapid);

				//for (SunNode life : portal["life"])
				//{
				//	int64_t life_id = life["id"];
				//	std::string life_type = life["type"];

				//	if (life_type == "m")
				//	{
				//		// Mob
				//		SunNode life_name = sun_file::String["Mob.img"][life_id]["name"];

				//		std::string life_id_str = string_format::Extend_Id(life_id, 7);
				//		//SunNode life_level = sun_file::Mob[life_id_str + ".img"]["info"]["level"];

				//		//if (life_name && life_level)
				//		//	map_life[life_id] = { life_type, life_name + "(Lv. " + life_level + ")" };
				//	}
				//	else if (life_type == "n")
				//	{
				//		// NPC
				//		if (SunNode life_name = sun_file::String["Npc.img"][life_id]["name"])
				//			map_life[life_id] = { life_type, life_name };
				//	}
				//}

				return map_life;
			}

			SunNode Get_Map_Node_Name(int32_t mapid)
			{
				std::string prefix = std::to_string(mapid / 100000000);
				std::string mapid_str = string_format::Extend_Id(mapid, 9);

				return sun_file::Map["Map"]["Map" + prefix][mapid_str + ".img"];
			}
		}
	}
}