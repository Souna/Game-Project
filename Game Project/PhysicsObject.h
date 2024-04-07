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

#include "Constants.h"
#include "Interpolated.h"
#include "Point.h"

namespace game
{
	// Struct that contains all properties for movement calculations.
	struct MovingObject
	{
		Linear<double> x;
		Linear<double> y;
		double horizontal_speed = 0.0;
		double vertical_speed = 0.0;

		void Normalize()
		{
			x.Normalize();
			y.Normalize();
		}

		void Move()
		{
			x += horizontal_speed;
			y += vertical_speed;
		}

		void Set_X(double d)
		{
			x.Set(d);
		}

		void Set_Y(double d)
		{
			y.Set(d);
		}

		void Limit_X(double d)
		{
			x = d;
			horizontal_speed = 0.0;
		}

		void Limit_Y(double d)
		{
			y = d;
			vertical_speed = 0.0;
		}

		void Move_X_Until(double d, uint16_t delay)
		{
			if (delay)
			{
				double hdelta = d - x.Get();
				horizontal_speed = constants::TIMESTEP * hdelta / delay;
			}
		}

		void Move_Y_Until(double d, uint16_t delay)
		{
			if (delay)
			{
				double vdelta = d - y.Get();
				vertical_speed = constants::TIMESTEP * vdelta / delay;
			}
		}

		bool Horizontal_Mobile() const
		{
			return horizontal_speed != 0.0;
		}

		bool Vertical_Mobile() const
		{
			return vertical_speed != 0.0;
		}

		bool Mobile() const
		{
			return Horizontal_Mobile() || Vertical_Mobile();
		}

		double Current_X() const
		{
			return x.Get();
		}

		double Current_Y() const
		{
			return y.Get();
		}

		double Next_X() const
		{
			return x + horizontal_speed;
		}

		double Next_Y() const
		{
			return y + vertical_speed;
		}

		int16_t Get_X() const
		{
			double rounded = std::round(x.Get());
			return static_cast<int16_t>(rounded);
		}

		int16_t Get_Y() const
		{
			double rounded = std::round(y.Get());
			return static_cast<int16_t>(rounded);
		}

		int16_t Get_Last_X() const
		{
			double rounded = std::round(x.Last());
			return static_cast<int16_t>(rounded);
		}

		int16_t Get_Last_Y() const
		{
			double rounded = std::round(y.Last());
			return static_cast<int16_t>(rounded);
		}

		Point<int16_t> Get_Position() const
		{
			return { Get_X(), Get_Y() };
		}

		int16_t Get_Absolute_X(double view_x, float alpha) const
		{
			double interx = x.Normalized() ? std::round(x.Get()) : x.Get(alpha);

			return static_cast<int16_t>(
				std::round(interx + view_x)
				);
		}

		int16_t Get_Absolute_Y(double view_y, float alpha) const
		{
			double intery = y.Normalized() ? std::round(y.Get()) : y.Get(alpha);

			return static_cast<int16_t>(
				std::round(intery + view_y)
				);
		}

		Point<int16_t> Get_Absolute_Position(double view_x, double view_y, float alpha) const
		{
			return { Get_Absolute_X(view_x, alpha), Get_Absolute_Y(view_y, alpha) };
		}
	};

	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================

		// Struct that contains all properties necessary for physics calculations.
	struct PhysicsObject : public MovingObject
	{
		// Determines which physics engine to use.
		enum Type
		{
			NORMAL,
			ICE,
			SWIMMING,
			FLYING,
			FIXATED
		};

		enum Flag
		{
			NOGRAVITY = 0x0001,
			TURNATEDGES = 0x0002,
			CHECKBELOW = 0x0004
		};

		Type type_ = NORMAL;
		int32_t flags_ = 0;
		uint16_t foothold_id_ = 0;
		double foothold_slope_ = 0.0;
		int8_t foothold_layer_ = 0;
		double ground_below_ = 0.0;
		bool on_ground_ = true;
		bool enable_jump_down_ = false;

		double horizontal_force_ = 0.0;
		double vertical_force_ = 0.0;
		double horizontal_acceleration_ = 0.0;
		double vertical_acceleration_ = 0.0;

		bool Is_Flag_Set(Flag f)
		{
			return (flags_ & f) != 0;
		}

		bool Is_Flag_Not_Set(Flag f)
		{
			return !Is_Flag_Set(f);
		}

		void Set_Flag(Flag f)
		{
			flags_ |= f;
		}

		void Clear_Flag(Flag f)
		{
			flags_ &= ~f;
		}

		void Clear_All_Flags()
		{
			flags_ = 0;
		}
	};
}