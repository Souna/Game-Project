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

#include "Point.h"
#include "Range.h"
#include "Optional.h"

#include <vector>

#include "SunNode.h"

namespace game
{
	class Seat
	{
	public:
		Seat(SunNode source);

		bool In_Range(Point<int16_t> position) const;
		Point<int16_t> Get_Position() const;

	private:
		Point<int16_t> position_;
	};

	class Ladder
	{
	public:
		Ladder(SunNode source);

		bool Is_Ladder() const;
		bool In_Range(Point<int16_t> position, bool upwards) const;
		bool Fell_Off(int16_t y, bool downwards) const;
		int16_t Get_X() const;

	private:
		int16_t x_;
		int16_t y1_;
		int16_t y2_;
		bool ladder_;
	};

	class MapInfo
	{
	public:
		MapInfo(SunNode src, Range<int16_t> walls, Range<int16_t> borders);
		MapInfo();

		bool Is_Underwater() const;
		std::string Get_BGM() const;
		Range<int16_t> Get_Walls() const;
		Range<int16_t> Get_Borders() const;

		// Find a seat the player's position.
		Optional<const Seat> Find_Seat(Point<int16_t> position) const;
		// Find a ladder at the player's position. upwards = false implies downwards.
		Optional<const Ladder> Find_Ladder(Point<int16_t> position, bool upwards) const;

	private:
		int32_t field_limit_;
		bool cloud_;
		std::string bgm_;
		std::string map_description_;
		std::string map_name_;
		std::string street_name_;
		std::string map_mark_;
		bool swim_;
		bool town_;
		bool hide_minimap_;
		Range<int16_t> map_walls_;
		Range<int16_t> map_borders_;
		std::vector<Seat> seats_;
		std::vector<Ladder> ladders_;
	};
}