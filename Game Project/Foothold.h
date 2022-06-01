#pragma once

#include "Range.h";
#include <cstdint>
#include "SunNode.h"

namespace game
{
	/**
	* A single foothold.
	* TODO: Ability to connect several footholds
	* .__________.___.
	* (x1, y1) (x2, y2) (x3, y3)? idk
	* Utilize previous_ and next_ I guess.
	*/
	class Foothold
	{
	public:
		Foothold(SunNode src, uint16_t id, uint8_t layer, bool anchors);
		Foothold();

		// Returns the individual ID of this foothold.
		uint16_t Id() const;
		// Returns the ID of the foothold to the left, if any.
		uint16_t Previous() const;
		// Returns the ID of the foothold to the right, if any.
		uint16_t Next() const;
		// Returns the layer the platform is part of.
		uint16_t Layer() const;

		// Returns the horizontal component of the foothold.
		// (Both x1 and x2 in a single var).
		const Range<int16_t>& Get_Hori_Component() const;
		// Returns the vertical component of the foothold.
		// Both (y1 and y2 in a single var).
		const Range<int16_t>& Get_Vert_Component() const;

		// Returns left edge of the foothold.
		int16_t Left_Edge() const;
		// Returns right edge of the foothold.
		int16_t Right_Edge() const;
		// Returns top edge of the foothold.
		int16_t Top_Edge() const;
		// Returns bottom edge of the foothold.
		int16_t Bottom_Edge() const;

		// Returns the first horizontal component of the foothold.
		int16_t X1() const;
		// Returns the second horizontal component of the foothold.
		int16_t X2() const;
		// Returns the first vertical component of the foothold.
		int16_t Y1() const;
		// Returns the second vertical component of the foothold.
		int16_t Y2() const;

		// Returns if the foothold is a wall (x1 == x2).
		bool Is_Wall() const;
		// Returns if the foothold is a floor (y1 == y2).
		bool Is_Floor() const;
		// Returns if the foothold is a left edge.
		bool Is_Left_Edge() const;
		// Returns if the foothold is a right edge.
		bool Is_Right_Edge() const;

		// Returns if an x-coordinate is above or below this platform.
		bool Hcontains(int16_t x) const;
		// Returns if a y-coordinate is right or left of this platform.
		bool Vcontains(int16_t y) const;

		// Check whether this foothold blocks an object.
		bool Is_Blocking(const Range<int16_t>& vertical) const;

		// Returns foothold width.
		int16_t Hdelta() const;
		// Returns foothold height.
		int16_t Vdelta() const;

		// Returns foothold width.... looks nicer
		int16_t Width() const;
		// Returns foothold height.... looks nicer lol
		int16_t Height() const;

		// Returns the slope of the foothold (vertical/horizontal).
		double Slope() const;

		// Returns a y-coordinate right below the given x-coordinate.
		double Find_Ground_Below(double x) const;

		// Display draggable foothold anchors.
		bool Show_Draggable_Anchors() const;
		// TODO: Make FH handles into their own class.

	private:
		uint16_t id_;
		uint16_t previous_;
		uint16_t next_;
		uint8_t layer_;

		Range<int16_t> horizontal_range_;
		Range<int16_t> vertical_range_;

		bool show_draggable_anchors_;
	};
}