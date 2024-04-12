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

#include "Interpolated.h"
#include "Point.h"
#include "Range.h"

#include <cstdint>

namespace game
{
	// View on stage which follows the player object.
	class Camera
	{
	public:
		// Initialize everything to 0, we need the player's spawnpoint first to properly set the position.
		Camera();

		// Update the view with the current player position. (Or any other target)
		void Update(Point<int16_t> position);
		// Set the position, changing the view immediately.
		void Set_Position(Point<int16_t> position);
		// Updates the view's boundaries. Determined by mapinfo or footholds.
		void Set_View(Range<int16_t> hborders, Range<int16_t> vborders);
		// Return the current position.
		Point<int16_t> Position() const;
		// Return the interpolated position.
		Point<int16_t> Position(float alpha) const;
		// Return the interpolated position.
		Point<double> Real_Position(float alpha) const;

	private:
		// Movement variables.
		Linear<double> x_;
		Linear<double> y_;

		// View limits.
		Range<int16_t> horizontal_bounds_;
		Range<int16_t> vertical_bounds_;

		int16_t VIEW_WIDTH_;
		int16_t VIEW_HEIGHT_;
	};
}