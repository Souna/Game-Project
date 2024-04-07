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
#include "FootholdTree.h"

#include "Console.h"

namespace game
{
	FootholdTree::FootholdTree(SunNode src)
	{
		int16_t leftw = 30000;
		int16_t rightw = -30000;
		int16_t botb = -30000;
		int16_t topb = 30000;

		// Get layers.
		for (auto basef : src)
		{
			uint8_t layer;

			try
			{
				layer = static_cast<uint8_t>(std::stoi(basef.second.Name()));
			}
			catch (const std::exception& ex)
			{
				Console::Get().Print(__func__, ex);
				continue;
			}

			// Get foothold id's.
			for (auto midf : basef.second)
			{
				for (auto lastf : midf.second)
				{
					uint16_t id;

					try
					{
						id = static_cast<uint16_t>(std::stoi(lastf.second.Name()));
					}
					catch (const std::exception& ex)
					{
						Console::Get().Print(__func__, ex);
						continue;
					}

					const Foothold& foothold = footholds_.emplace(
						std::piecewise_construct,
						std::forward_as_tuple(id),
						std::forward_as_tuple(lastf.second, id, layer)
					).first->second;

					if (foothold.Left_Edge() < leftw)
						leftw = foothold.Left_Edge();

					if (foothold.Right_Edge() > rightw)
						rightw = foothold.Right_Edge();

					if (foothold.Bottom_Edge() > botb)
						botb = foothold.Bottom_Edge();

					if (foothold.Top_Edge() < topb)
						topb = foothold.Top_Edge();

					if (foothold.Is_Wall())
						continue;

					int16_t start = foothold.Left_Edge();
					int16_t end = foothold.Right_Edge();

					for (int16_t i = start; i <= end; i++)
						footholds_by_x_.emplace(i, id);
				}
			}
		}

		walls_ = { static_cast<int16_t>(leftw + 25), static_cast<int16_t>(rightw - 25) };
		borders_ = { static_cast<int16_t>(topb - 300), static_cast<int16_t>(botb + 100) };
	}

	FootholdTree::FootholdTree() {}

	void FootholdTree::Limit_Movement(PhysicsObject& phobj) const
	{
		if (phobj.Horizontal_Mobile())
		{
			double Current_X = phobj.Current_X();
			double Next_X = phobj.Next_X();

			bool left = phobj.horizontal_speed < 0.0f;
			double wall = Get_Wall(phobj.foothold_id_, left, phobj.Next_Y());
			bool collision = left ? Current_X >= wall && Next_X <= wall : Current_X <= wall && Next_X >= wall;

			if (!collision && phobj.Is_Flag_Set(PhysicsObject::Flag::TURNATEDGES))
			{
				wall = Get_Edge(phobj.foothold_id_, left);
				collision = left ? Current_X >= wall && Next_X <= wall : Current_X <= wall && Next_X >= wall;
			}

			if (collision)
			{
				phobj.Limit_X(wall);
				phobj.Clear_Flag(PhysicsObject::Flag::TURNATEDGES);
			}
		}

		if (phobj.Vertical_Mobile())
		{
			double current_y = phobj.Current_Y();
			double Next_Y = phobj.Next_Y();

			auto ground = Range<double>(
				Get_Foothold(phobj.foothold_id_).Find_Ground_Below(phobj.Current_X()),
				Get_Foothold(phobj.foothold_id_).Find_Ground_Below(phobj.Next_X())
			);

			bool collision = current_y <= ground.First() && Next_Y >= ground.Second();

			if (collision)
			{
				phobj.Limit_Y(ground.Second());

				Limit_Movement(phobj);
			}
			else
			{
				if (Next_Y < borders_.First())
					phobj.Limit_Y(borders_.First());
				else if (Next_Y > borders_.Second())
					phobj.Limit_Y(borders_.Second());
			}
		}
	}

	void FootholdTree::Update_Foothold(PhysicsObject& phobj) const
	{
		if (phobj.type_ == PhysicsObject::Type::FIXATED && phobj.foothold_id_ > 0)
			return;

		const Foothold& curfh = Get_Foothold(phobj.foothold_id_);
		bool checkslope = false;

		double x = phobj.Current_X();
		double y = phobj.Current_Y();

		if (phobj.on_ground_)
		{
			if (std::floor(x) > curfh.Right_Edge())
				phobj.foothold_id_ = curfh.Next();
			else if (std::ceil(x) < curfh.Left_Edge())
				phobj.foothold_id_ = curfh.Previous();

			if (phobj.foothold_id_ == 0)
				phobj.foothold_id_ = Get_Foothold_Id_Below(x, y);
			else
				checkslope = true;
		}
		else
		{
			phobj.foothold_id_ = Get_Foothold_Id_Below(x, y);
		}

		const Foothold& nextfh = Get_Foothold(phobj.foothold_id_);
		phobj.foothold_slope_ = nextfh.Slope();

		double ground = nextfh.Find_Ground_Below(x);

		if (phobj.vertical_speed == 0.0 && checkslope)
		{
			double vdelta = abs(phobj.foothold_slope_);

			if (phobj.foothold_slope_ < 0.0)
				vdelta *= (ground - y);
			else if (phobj.foothold_slope_ > 0.0)
				vdelta *= (y - ground);

			if (curfh.Slope() != 0.0 || nextfh.Slope() != 0.0)
			{
				if (phobj.horizontal_speed > 0.0 && vdelta <= phobj.horizontal_speed)
					phobj.y = ground;
				else if (phobj.horizontal_speed < 0.0 && vdelta >= phobj.horizontal_speed)
					phobj.y = ground;
			}
		}

		phobj.on_ground_ = phobj.y == ground;

		if (phobj.enable_jump_down_ || phobj.Is_Flag_Set(PhysicsObject::Flag::CHECKBELOW))
		{
			uint16_t belowid = Get_Foothold_Id_Below(x, nextfh.Find_Ground_Below(x) + 1.0);

			if (belowid > 0)
			{
				double nextground = Get_Foothold(belowid).Find_Ground_Below(x);
				phobj.enable_jump_down_ = (nextground - ground) < 600.0;
				phobj.ground_below_ = ground + 1.0;
			}
			else
			{
				phobj.enable_jump_down_ = false;
			}

			phobj.Clear_Flag(PhysicsObject::Flag::CHECKBELOW);
		}

		if (phobj.foothold_layer_ == 0 || phobj.on_ground_)
			phobj.foothold_layer_ = nextfh.Layer();

		if (phobj.foothold_id_ == 0)
		{
			phobj.foothold_id_ = curfh.Id();
			phobj.Limit_X(curfh.X1());
		}
	}

	const Foothold& FootholdTree::Get_Foothold(uint16_t foothold_id_) const
	{
		auto iter = footholds_.find(foothold_id_);

		if (iter == footholds_.end())
			return null_foothold_;

		return iter->second;
	}

	double FootholdTree::Get_Wall(uint16_t curid, bool left, double fy) const
	{
		auto shorty = static_cast<int16_t>(fy);
		Range<int16_t> vertical(shorty - 50, shorty - 1);
		const Foothold& cur = Get_Foothold(curid);

		if (left)
		{
			const Foothold& prev = Get_Foothold(cur.Previous());

			if (prev.Is_Blocking(vertical))
				return cur.Left_Edge();

			const Foothold& prev_prev = Get_Foothold(prev.Previous());

			if (prev_prev.Is_Blocking(vertical))
				return prev.Left_Edge();

			return walls_.First();
		}
		else
		{
			const Foothold& next = Get_Foothold(cur.Next());

			if (next.Is_Blocking(vertical))
				return cur.Right_Edge();

			const Foothold& next_next = Get_Foothold(next.Next());

			if (next_next.Is_Blocking(vertical))
				return next.Right_Edge();

			return walls_.Second();
		}
	}

	double FootholdTree::Get_Edge(uint16_t curid, bool left) const
	{
		const Foothold& fh = Get_Foothold(curid);

		if (left)
		{
			uint16_t previd = fh.Previous();

			if (!previd)
				return fh.Left_Edge();

			const Foothold& prev = Get_Foothold(previd);
			uint16_t prev_previd = prev.Previous();

			if (!prev_previd)
				return prev.Left_Edge();

			return walls_.First();
		}
		else
		{
			uint16_t nextid = fh.Next();

			if (!nextid)
				return fh.Right_Edge();

			const Foothold& next = Get_Foothold(nextid);
			uint16_t next_nextid = next.Next();

			if (!next_nextid)
				return next.Right_Edge();

			return walls_.Second();
		}
	}

	uint16_t FootholdTree::Get_Foothold_Id_Below(double fx, double fy) const
	{
		uint16_t ret = 0;
		double comp = borders_.Second();

		int16_t x = static_cast<int16_t>(fx);
		auto range = footholds_by_x_.equal_range(x);

		for (auto iter = range.first; iter != range.second; ++iter)
		{
			const Foothold& fh = footholds_.at(iter->second);
			double ycomp = fh.Find_Ground_Below(fx);

			if (comp >= ycomp && ycomp >= fy)
			{
				comp = ycomp;
				ret = fh.Id();
			}
		}

		return ret;
	}

	int16_t FootholdTree::Get_Y_Below(Point<int16_t> position) const
	{
		if (uint16_t foothold_id_ = Get_Foothold_Id_Below(position.X(), position.Y()))
		{
			const Foothold& fh = Get_Foothold(foothold_id_);

			return static_cast<int16_t>(fh.Find_Ground_Below(position.X()));
		}
		else
		{
			return borders_.Second();
		}
	}

	Range<int16_t> FootholdTree::Get_Walls() const
	{
		return walls_;
	}

	Range<int16_t> FootholdTree::Get_Borders() const
	{
		return borders_;
	}
}