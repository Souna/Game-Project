#include "Foothold.h"

// This constructor implies I have a node parameter supplying all the information I need
// in relation to footholds on the map.
//Foothold::Foothold(nl::node src, uint16_t id, uint8_t ly) : m_prev(src["prev"]), m_next(src["next"]), m_horizontal(src["x1"], src["x2"]), m_vertical(src["y1"], src["y2"]), m_id(id), m_layer(ly) {}

Foothold::Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer) : m_id(id), m_previous(previous), m_next(next), m_layer(layer) {};

Foothold::Foothold(uint16_t id, uint16_t previous, uint16_t next, uint8_t layer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool handles)
	: m_id(id), m_previous(previous), m_next(next), m_layer(layer), m_horizontal(x1, x2), m_vertical(y1, y2)
{
	m_displayhandles = handles;
};
Foothold::Foothold() : m_id(0), m_previous(0), m_next(0), m_layer(0) {}

uint16_t Foothold::id() const
{
	return m_id;
}

uint16_t Foothold::previous() const
{
	return m_previous;
}

uint16_t Foothold::next() const
{
	return m_next;
}

uint16_t Foothold::layer() const
{
	return m_layer;
}

const Range<int16_t>& Foothold::get_horizontal() const
{
	return m_horizontal;
}

const Range<int16_t>& Foothold::get_vertical() const
{
	return m_vertical;
}

int16_t Foothold::l() const
{
	return m_horizontal.smaller();
}

int16_t Foothold::r() const
{
	return m_horizontal.greater();
}

int16_t Foothold::t() const
{
	return m_vertical.smaller();
}

int16_t Foothold::b() const
{
	return m_vertical.greater();
}

int16_t Foothold::x1() const
{
	return m_horizontal.first();
}

int16_t Foothold::x2() const
{
	return m_horizontal.second();
}

int16_t Foothold::y1() const
{
	return m_vertical.first();
}

int16_t Foothold::y2() const
{
	return m_vertical.second();
}

bool Foothold::is_wall() const
{
	// x1 == x2
	return m_id && m_horizontal.empty();
}

bool Foothold::is_floor() const
{
	// y1 == y2
	return m_id && m_vertical.empty();
}

bool Foothold::is_left_edge() const
{
	// If there's nothing to the left.
	return m_id && m_previous == 0;
}

bool Foothold::is_right_edge() const
{
	// If there's nothing to the right.
	return m_id && m_next == 0;
}

bool Foothold::hcontains(int16_t x) const
{
	return m_id && m_horizontal.contains(x);
}

bool Foothold::vcontains(int16_t y) const
{
	return m_id && m_vertical.contains(y);
}

bool Foothold::is_blocking(const Range<int16_t>& vertical) const
{
	return is_wall() && m_vertical.overlaps(vertical);
}

int16_t Foothold::hdelta() const
{
	// Width.
	return m_horizontal.delta();
}

int16_t Foothold::vdelta() const
{
	// Height.
	return m_vertical.delta();
}

double Foothold::slope() const
{
	// If this foothold x1 == x2, return 0.
	// Else return rise/run -> double.
	return is_wall() ? 0 : static_cast<double>(vdelta() / hdelta());
}

double Foothold::ground_below(double x) const
{
	// If this foothold y1 == y2, return 0.
	return is_floor() ? y1() : slope() * (x - x1()) + y1();
}

// These are not implemented because the m_horizontal/m_vertical
// members are private and I didn't bother to work with that yet.
// Right now the way to assign these values is to just
// use the arguments passed to the constructor in the initializion list.
//void Foothold::set_horizontal(int16_t x1, int16_t x2)
//{
//	m_horizontal.first = x1;
//	m_horizontal.second = x2;
//}
//
//
//void Foothold::set_vertical(int16_t y1, int16_t y2)
//{
//	m_vertical.first = y1;
//	m_vertical.second = y2;
//}

bool Foothold::display_handles() const
{
	return m_displayhandles;
}