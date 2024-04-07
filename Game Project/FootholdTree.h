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

#include "Foothold.h"
#include "PhysicsObject.h"

#include <unordered_map>
#include "PhysicsObject.h"

namespace game
{
	// The collection of platforms in a maple-map. Used for collision-detection.
	class FootholdTree
	{
	public:
		FootholdTree(SunNode source);
		FootholdTree();

		// Takes an accelerated PhysicsObject and limits its movement based on the platforms in this tree.
		void Limit_Movement(PhysicsObject& to_use) const;
		// Updates a PhysicsObject's foothold id based on it's position.
		void Update_Foothold(PhysicsObject& to_use) const;
		// Determine the point on the ground below the specified position.
		int16_t Get_Y_Below(Point<int16_t> position) const;
		// Returns the leftmost and rightmost platform positions of the map.
		Range<int16_t> Get_Walls() const;
		// Returns the topmost and bottommost platform positions of the map.
		Range<int16_t> Get_Borders() const;

	private:
		uint16_t Get_Foothold_Id_Below(double fx, double fy) const;
		double Get_Wall(uint16_t fhid, bool left, double fy) const;
		double Get_Edge(uint16_t fhid, bool left) const;
		const Foothold& Get_Foothold(uint16_t fhid) const;

		std::unordered_map<uint16_t, Foothold> footholds_;
		std::unordered_multimap<int16_t, uint16_t> footholds_by_x_;

		Foothold null_foothold_;
		Range<int16_t> walls_;
		Range<int16_t> borders_;
	};
}