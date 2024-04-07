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

#include "Animation.h"
#include "Rectangle.h"

#include <cstdint>
#include <map>

namespace game
{
	class Portal
	{
	public:
		enum Type
		{
			SPAWN,
			INVISIBLE,
			REGULAR,
			TOUCH,
			TYPE4,
			TYPE5,
			WARP,
			SCRIPTED,
			SCRIPTED_INVISIBLE,
			SCRIPTED_TOUCH,
			HIDDEN,
			SCRIPTED_HIDDEN,
			SPRING1,
			SPRING2,
			TYPE14
		};

		static Type Type_By_Id(int32_t id)
		{
			return static_cast<Type>(id);
		}

		struct WarpInfo
		{
			int32_t mapid;
			std::string to_name;
			std::string name;
			bool intra_map;
			bool valid;

			WarpInfo(int32_t m, bool i, std::string tn, std::string n) : mapid(m), intra_map(i), to_name(tn), name(n)
			{
				valid = mapid < 999999999;
			}

			WarpInfo() : WarpInfo(999999999, false, {}, {}) {}
		};

		Portal(const Animation* animation, Type type, std::string name, bool intramap, Point<int16_t> position, int32_t tomap, std::string toname);
		Portal();

		void Update(Point<int16_t> playerpos);
		void Draw(Point<int16_t> viewpos, float alpha) const;

		std::string Get_Name() const;
		Type Get_Type() const;
		Point<int16_t> Get_Position() const;
		Rectangle<int16_t> Bounds() const;

		WarpInfo Get_Warp_Info() const;

	private:
		const Animation* animation_;
		Type type_;
		std::string name_;
		Point<int16_t> position_;
		WarpInfo warp_info_;
		bool touched_;
	};
}