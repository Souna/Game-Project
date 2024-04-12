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
#include "Camera.h"

#include "Constants.h"

namespace game
{
	Camera::Camera()
	{
		x_.Set(0.0);
		y_.Set(0.0);

		VIEW_WIDTH_ = constants::Constants::Get().Get_View_Width();
		VIEW_HEIGHT_ = constants::Constants::Get().Get_View_Height();
	}

	void Camera::Update(Point<int16_t> position)
	{
		int32_t new_width = constants::Constants::Get().Get_View_Width();
		int32_t new_height = constants::Constants::Get().Get_View_Height();

		if (VIEW_WIDTH_ != new_width || VIEW_HEIGHT_ != new_height)
		{
			VIEW_WIDTH_ = new_width;
			VIEW_HEIGHT_ = new_height;
		}

		double next_x = x_.Get();
		double hdelta = VIEW_WIDTH_ / 2 - position.X() - next_x;

		if (std::abs(hdelta) >= 5.0)
			next_x += hdelta * (12.0 / VIEW_WIDTH_);

		double next_y = y_.Get();
		double vdelta = VIEW_HEIGHT_ / 2 - position.Y() - next_y;

		if (std::abs(vdelta) >= 5.0)
			next_y += vdelta * (12.0 / VIEW_HEIGHT_);

		if (next_x > horizontal_bounds_.First() || horizontal_bounds_.Length() < VIEW_WIDTH_)
			next_x = horizontal_bounds_.First();
		else if (next_x < horizontal_bounds_.Second() + VIEW_WIDTH_)
			next_x = horizontal_bounds_.Second() + VIEW_WIDTH_;

		if (next_y > vertical_bounds_.First() || vertical_bounds_.Length() < VIEW_HEIGHT_)
			next_y = vertical_bounds_.First();
		else if (next_y < vertical_bounds_.Second() + VIEW_HEIGHT_)
			next_y = vertical_bounds_.Second() + VIEW_HEIGHT_;

		x_ = next_x;
		y_ = next_y;
	}

	void Camera::Set_Position(Point<int16_t> position)
	{
		int32_t new_width = constants::Constants::Get().Get_View_Width();
		int32_t new_height = constants::Constants::Get().Get_View_Height();

		if (VIEW_WIDTH_ != new_width || VIEW_HEIGHT_ != new_height)
		{
			VIEW_WIDTH_ = new_width;
			VIEW_HEIGHT_ = new_height;
		}

		x_.Set(VIEW_WIDTH_ / 2 - position.X());
		y_.Set(VIEW_HEIGHT_ / 2 - position.Y());
	}

	void Camera::Set_View(Range<int16_t> mapwalls, Range<int16_t> mapborders)
	{
		horizontal_bounds_ = -mapwalls;
		vertical_bounds_ = -mapborders;
	}

	Point<int16_t> Camera::Position() const
	{
		auto shortx = static_cast<int16_t>(std::round(x_.Get()));
		auto shorty = static_cast<int16_t>(std::round(y_.Get()));

		return { shortx, shorty };
	}

	Point<int16_t> Camera::Position(float alpha) const
	{
		auto interx = static_cast<int16_t>(std::round(x_.Get(alpha)));
		auto intery = static_cast<int16_t>(std::round(y_.Get(alpha)));

		return { interx, intery };
	}

	Point<double> Camera::Real_Position(float alpha) const
	{
		return { x_.Get(alpha), y_.Get(alpha) };
	}
}