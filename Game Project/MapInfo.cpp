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
#include "MapInfo.h"

#include "Constants.h"

namespace game
{
	MapInfo::MapInfo(SunNode src, Range<int16_t> walls, Range<int16_t> borders)
	{
		SunNode info = src["info"];

		if (info["VRLeft"].Get_Node_Type() == SunNode::SunPropertyType::INTEGER)
		{
			map_walls_ = Range<int16_t>(info["VRLeft"], info["VRRight"]);
			map_borders_ = Range<int16_t>(info["VRTop"], info["VRBottom"]);
		}
		else
		{
			map_walls_ = walls;
			map_borders_ = borders;
		}

		std::string bgm_path = info["bgm"];
		size_t split = bgm_path.find('/');
		bgm_ = bgm_path.substr(0, split) + ".img/" + bgm_path.substr(split + 1);

		cloud_ = info["cloud_"].Get_Boolean();
		field_limit_ = info["fieldLimit"];
		hide_minimap_ = info["hideMinimap"].Get_Boolean();
		map_mark_ = info["mapMark"];
		swim_ = info["swim"].Get_Boolean();
		town_ = info["town"].Get_Boolean();

		for (auto seat : src["seat"])
			seats_.push_back(seat.second);

		for (auto ladder : src["ladderRope"])
			ladders_.push_back(ladder.second);
	}

	MapInfo::MapInfo() {}

	bool MapInfo::Is_Underwater() const
	{
		return swim_;
	}

	std::string MapInfo::Get_BGM() const
	{
		return bgm_;
	}

	Range<int16_t> MapInfo::Get_Walls() const
	{
		return map_walls_;
	}

	Range<int16_t> MapInfo::Get_Borders() const
	{
		return map_borders_;
	}

	Optional<const Seat> MapInfo::Find_Seat(Point<int16_t> position) const
	{
		for (auto& seat : seats_)
			if (seat.In_Range(position))
				return seat;

		return nullptr;
	}

	Optional<const Ladder> MapInfo::Find_Ladder(Point<int16_t> position, bool upwards) const
	{
		for (auto& ladder : ladders_)
			if (ladder.In_Range(position, upwards))
				return ladder;

		return nullptr;
	}

	Seat::Seat(SunNode src)
	{
		position_ = src;
	}

	bool Seat::In_Range(Point<int16_t> position) const
	{
		auto horizontal = Range<int16_t>::Symmetric(position.X(), 10);
		auto vertical = Range<int16_t>::Symmetric(position.Y(), 10);

		return horizontal.Contains(position_.X()) && vertical.Contains(position_.Y());
	}

	Point<int16_t> Seat::Get_Position() const
	{
		return position_;
	}

	Ladder::Ladder(SunNode src)
	{
		x_ = src["x"];
		y1_ = src["y1"];
		y2_ = src["y2"];
		ladder_ = src["l"].Get_Boolean();
	}

	bool Ladder::Is_Ladder() const
	{
		return ladder_;
	}

	bool Ladder::In_Range(Point<int16_t> position, bool upwards) const
	{
		auto horizontal = Range<int16_t>::Symmetric(position.X(), 10);
		auto vertical = Range<int16_t>(y1_, y2_);

		int16_t y = upwards ?
			position.Y() - 5 :
			position.Y() + 5;

		return horizontal.Contains(x_) && vertical.Contains(y);
	}

	bool Ladder::Fell_Off(int16_t y, bool downwards) const
	{
		int16_t dy = downwards ? y + 5 : y - 5;

		return dy > y2_ || y + 5 < y1_;
	}

	int16_t Ladder::Get_X() const
	{
		return x_;
	}
}