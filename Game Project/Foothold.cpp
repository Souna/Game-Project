#include "Foothold.h"

namespace game
{
	// This commented constructor implies I have a node parameter supplying all the information I need
	// in relation to footholds on the map.
	//Foothold::Foothold(nl::node src, uint16_t id, uint8_t ly) : m_prev(src["prev"]), next_(src["next"]), horizontal_range_(src["x1"], src["x2"]), vertical_range_(src["y1"], src["y2"]), m_id(id), layer_(ly) {}
	Foothold::Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer) : id_(id), previous_(previous), next_(next), layer_(layer) {};

	Foothold::Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool handles)
		: id_(id), previous_(previous), next_(next), layer_(layer), horizontal_range_(x1, x2), vertical_range_(y1, y2)
	{
		show_draggable_handles_ = handles;
	};
	Foothold::Foothold() : id_(0), previous_(0), next_(0), layer_(0) {}

	uint16_t Foothold::Id() const
	{
		return id_;
	}

	uint16_t Foothold::Previous() const
	{
		return previous_;
	}

	uint16_t Foothold::Next() const
	{
		return next_;
	}

	uint16_t Foothold::Layer() const
	{
		return layer_;
	}

	const Range<int16_t>& Foothold::Get_Hori_Component() const
	{
		return horizontal_range_;
	}

	const Range<int16_t>& Foothold::Get_Vert_Component() const
	{
		return vertical_range_;
	}

	int16_t Foothold::Left_Edge() const
	{
		return horizontal_range_.Smaller();
	}

	int16_t Foothold::Right_Edge() const
	{
		return horizontal_range_.Greater();
	}

	int16_t Foothold::Top_Edge() const
	{
		return vertical_range_.Smaller();
	}

	int16_t Foothold::Bottom_Edge() const
	{
		return vertical_range_.Greater();
	}

	int16_t Foothold::X1() const
	{
		return horizontal_range_.First();
	}

	int16_t Foothold::X2() const
	{
		return horizontal_range_.Second();
	}

	int16_t Foothold::Y1() const
	{
		return vertical_range_.First();
	}

	int16_t Foothold::Y2() const
	{
		return vertical_range_.Second();
	}

	bool Foothold::Is_Wall() const
	{
		// x1 == x2
		return id_ && horizontal_range_.Is_Empty();
	}

	bool Foothold::Is_Floor() const
	{
		// y1 == y2
		return id_ && vertical_range_.Is_Empty();
	}

	bool Foothold::Is_Left_Edge() const
	{
		// If there's nothing to the left.
		return id_ && previous_ == 0;
	}

	bool Foothold::Is_Right_Edge() const
	{
		// If there's nothing to the right.
		return id_ && next_ == 0;
	}

	bool Foothold::Hcontains(int16_t x) const
	{
		return id_ && horizontal_range_.Contains(x);
	}

	bool Foothold::Vcontains(int16_t y) const
	{
		return id_ && vertical_range_.Contains(y);
	}

	bool Foothold::Is_Blocking(const Range<int16_t>& vertical) const
	{
		return Is_Wall() && vertical_range_.Overlaps(vertical);
	}

	int16_t Foothold::Hdelta() const
	{
		// Width.
		return horizontal_range_.Delta();
	}

	int16_t Foothold::Vdelta() const
	{
		// Height.
		return vertical_range_.Delta();
	}

	int16_t Foothold::Width() const
	{
		return Hdelta();
	}

	int16_t Foothold::Height() const
	{
		return Vdelta();
	}

	double Foothold::Slope() const
	{
		// If this foothold x1 == x2, return 0.
		// Else return (rise/run) -> double.
		return Is_Wall() ? 0 : static_cast<double>(Vdelta() / Hdelta());
	}

	double Foothold::Find_Ground_Below(double x) const
	{
		// If this foothold y1 == y2, return 0.
		return Is_Floor() ? Y1() : Slope() * (x - X1()) + Y1();
	}

	// These are not implemented because the horizontal_range_/vertical_range_
	// members are private and I didn't bother to work with that yet.
	// Right now_ the way to assign these values is to just
	// use the arguments passed to the constructor in the initializion list.
	//void Foothold::Set_Hori_Component(int16_t x1, int16_t x2)
	//{
	//	horizontal_range_.first = x1;
	//	horizontal_range_.second = x2;
	//}
	//
	//
	//void Foothold::Set_Vert_Component(int16_t y1, int16_t y2)
	//{
	//	vertical_range_.first = y1;
	//	vertical_range_.second = y2;
	//}

	bool Foothold::Show_Draggable_Handles() const
	{
		return show_draggable_handles_;
	}
}