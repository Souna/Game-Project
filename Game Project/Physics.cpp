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
#include "Physics.h"

#include <functional>

namespace game
{
	const double GRAvertical_force_ = 0.14;
	const double SWIMGRAvertical_force_ = 0.03;
	const double FRICTION = 0.3;
	const double SLOPEFACTOR = 0.1;
	const double GROUNDSLIP = 3.0;
	const double FLYFRICTION = 0.05;
	const double SWIMFRICTION = 0.08;

	Physics::Physics(SunNode src)
	{
		footholdTree_ = src;
	}

	Physics::Physics() {}

	void Physics::Move_Object(PhysicsObject& phobj) const
	{
		// Determine which platform the object is currently on.
		footholdTree_.Update_Foothold(phobj);

		// Use the appropriate physics for the terrain the object is on.
		switch (phobj.type_)
		{
		case PhysicsObject::Type::NORMAL:
			Move_Normal(phobj);
			footholdTree_.Limit_Movement(phobj);
			break;
		case PhysicsObject::Type::FLYING:
			Move_Flying(phobj);
			footholdTree_.Limit_Movement(phobj);
			break;
		case PhysicsObject::Type::SWIMMING:
			Move_Swimming(phobj);
			footholdTree_.Limit_Movement(phobj);
			break;
		case PhysicsObject::Type::FIXATED:
		default:
			break;
		}

		// Move the object forward.
		phobj.Move();
	}

	void Physics::Move_Normal(PhysicsObject& phobj) const
	{
		phobj.vertical_acceleration_ = 0.0;
		phobj.horizontal_acceleration_ = 0.0;

		if (phobj.on_ground_)
		{
			phobj.vertical_acceleration_ += phobj.vertical_force_;
			phobj.horizontal_acceleration_ += phobj.horizontal_force_;

			if (phobj.horizontal_acceleration_ == 0.0 && phobj.horizontal_speed < 0.1 && phobj.horizontal_speed > -0.1)
			{
				phobj.horizontal_speed = 0.0;
			}
			else
			{
				double inertia = phobj.horizontal_speed / GROUNDSLIP;
				double slopef = phobj.foothold_slope_;

				if (slopef > 0.5)
					slopef = 0.5;
				else if (slopef < -0.5)
					slopef = -0.5;

				phobj.horizontal_acceleration_ -= (FRICTION + SLOPEFACTOR * (1.0 + slopef * -inertia)) * inertia;
			}
		}
		else if (phobj.Is_Flag_Not_Set(PhysicsObject::Flag::NOGRAVITY))
		{
			phobj.vertical_acceleration_ += GRAvertical_force_;
		}

		phobj.horizontal_force_ = 0.0;
		phobj.vertical_force_ = 0.0;

		phobj.horizontal_speed += phobj.horizontal_acceleration_;
		phobj.vertical_speed += phobj.vertical_acceleration_;
	}

	void Physics::Move_Flying(PhysicsObject& phobj) const
	{
		phobj.horizontal_acceleration_ = phobj.horizontal_force_;
		phobj.vertical_acceleration_ = phobj.vertical_force_;
		phobj.horizontal_force_ = 0.0;
		phobj.vertical_force_ = 0.0;

		phobj.horizontal_acceleration_ -= FLYFRICTION * phobj.horizontal_speed;
		phobj.vertical_acceleration_ -= FLYFRICTION * phobj.vertical_speed;

		phobj.horizontal_speed += phobj.horizontal_acceleration_;
		phobj.vertical_speed += phobj.vertical_acceleration_;

		if (phobj.horizontal_acceleration_ == 0.0 && phobj.horizontal_speed < 0.1 && phobj.horizontal_speed > -0.1)
			phobj.horizontal_speed = 0.0;

		if (phobj.vertical_acceleration_ == 0.0 && phobj.vertical_speed < 0.1 && phobj.vertical_speed > -0.1)
			phobj.vertical_speed = 0.0;
	}

	void Physics::Move_Swimming(PhysicsObject& phobj) const
	{
		phobj.horizontal_acceleration_ = phobj.horizontal_force_;
		phobj.vertical_acceleration_ = phobj.vertical_force_;
		phobj.horizontal_force_ = 0.0;
		phobj.vertical_force_ = 0.0;

		phobj.horizontal_acceleration_ -= SWIMFRICTION * phobj.horizontal_speed;
		phobj.vertical_acceleration_ -= SWIMFRICTION * phobj.vertical_speed;

		if (phobj.Is_Flag_Not_Set(PhysicsObject::Flag::NOGRAVITY))
			phobj.vertical_acceleration_ += SWIMGRAvertical_force_;

		phobj.horizontal_speed += phobj.horizontal_acceleration_;
		phobj.vertical_speed += phobj.vertical_acceleration_;

		if (phobj.horizontal_acceleration_ == 0.0 && phobj.horizontal_speed < 0.1 && phobj.horizontal_speed > -0.1)
			phobj.horizontal_speed = 0.0;

		if (phobj.vertical_acceleration_ == 0.0 && phobj.vertical_speed < 0.1 && phobj.vertical_speed > -0.1)
			phobj.vertical_speed = 0.0f;
	}

	Point<int16_t> Physics::Get_Y_Below(Point<int16_t> position) const
	{
		int16_t ground = footholdTree_.Get_Y_Below(position);

		return Point<int16_t>(position.X(), ground - 1);
	}

	const FootholdTree& Physics::Get_FootholdTree() const
	{
		return footholdTree_;
	}
}