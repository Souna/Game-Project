#pragma once
// A single foothold.
// TODO: Ability to connect several footholds.
// .__________.___.
// (x1, y1) (x2, y2) (x3, y3)? idk
// Utilize m_previous and m_next I guess.

#include "../Template/Range.h";
#include <cstdint>

class Foothold
{
public:
	//Foothold::Foothold(nl::node src, uint16_t id, uint8_t layer);
	Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer);
	Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool handles = false);
	Foothold();


	// Returns the individual ID of this foothold.
	uint16_t id() const;
	// Returns the ID of the foothold to the left, if any.
	uint16_t previous() const;
	// Returns the ID of the foothold to the right, if any.
	uint16_t next() const;
	// Returns the layer the platform is part of.
	uint16_t layer() const;


	// Returns the horizontal component of the foothold.
	// (Both x1 and x2 in a single var).
	const Range<int16_t>& get_horizontal() const;
	// Sets the horizontal component of the foothold.
	// (x1 and x2).
	void set_horizontal(int16_t x1, int16_t x2);
	// Returns the vertical component of the foothold.
	// Both (y1 and y2 in a single var).
	const Range<int16_t>& get_vertical() const;
	// Sets the horizontal component of the foothold.
	// (y1 and y2).
	void set_vertical(int16_t y1, int16_t y2);


	// Returns left edge of the foothold.
	int16_t l() const;
	// Returns right edge of the foothold.
	int16_t r() const;
	// Returns top edge of the foothold.
	int16_t t() const;
	// Returns bottom edge of the foothold.
	int16_t b() const;


	// Returns the first horizontal component of the foothold.
	int16_t x1() const;
	// Returns the second horizontal component of the foothold.
	int16_t x2() const;
	// Returns the first vertical component of the foothold.
	int16_t y1() const;
	// Returns the second vertical component of the foothold.
	int16_t y2() const;


	// Returns if the foothold is a wall (x1 == x2).
	bool is_wall() const;
	// Returns if the foothold is a floor (y1 == y2).
	bool is_floor() const;
	// Returns if the foothold is a left edge.
	bool is_left_edge() const;
	// Returns if the foothold is a right edge.
	bool is_right_edge() const;


	// Returns if an x-coordinate is above or below this platform.
	bool hcontains(int16_t x) const;
	// Returns if a y-coordinate is right or left of this platform.
	bool vcontains(int16_t y) const;


	// Check whether this foothold blocks an object.
	bool is_blocking(const Range<int16_t>& vertical) const;


	// Returns foothold width.
	int16_t hdelta() const;
	// Returns foothold height.
	int16_t vdelta() const;


	// Returns the slope of the foothold (vertical/horizontal).
	double slope() const;

	// Returns a y-coordinate right below the given x-coordinate.
	double ground_below(double x) const;

	// Display foothold handles.
	bool display_handles() const;
	// TODO: Make FH handles into their own class.

private:
	uint16_t m_id;
	uint16_t m_previous;
	uint16_t m_next;
	uint8_t m_layer;

	Range<int16_t> m_horizontal;
	Range<int16_t> m_vertical;

	bool m_displayhandles;
};