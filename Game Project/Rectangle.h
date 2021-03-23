#pragma once

#include "Point.h"
#include "Range.h"
#include "SunNode.h"

namespace game
{
	template <class T>
	class Rectangle
	{
	public:
		Rectangle(SunNode sourceLeftTop, SunNode sourceRightBottom) : left_top(sourceLeftTop), right_bottom(sourceRightBottom) {}
		Rectangle(SunNode source) : left_top(source["ltop"]), right_bottom(source["rbottom"]) {}

		constexpr Rectangle(Point<T> leftTop, Point<T> rightBottom) : left_top(leftTop), right_bottom(rightBottom) {}
		constexpr Rectangle(T left, T right, T top, T bottom) : left_top(left, top), right_bottom(right, bottom) {}
		constexpr Rectangle() {}

		constexpr T Width() const
		{
			return std::abs(Left() - Right());
		}

		constexpr T Height() const
		{
			return std::abs(Top() - Bottom());
		}

		constexpr T Left() const
		{
			return left_top.X();
		}

		constexpr T Top() const
		{
			return left_top.Y();
		}

		constexpr T Right() const
		{
			return right_bottom.X();
		}

		constexpr T Bottom() const
		{
			return right_bottom.Y();
		}

		constexpr bool Contains(const Point<T>& v) const
		{
			return
				!Straight() &&
				v.X() >= Left() && v.X() <= Right() &&
				v.Y() >= Top() && v.Y() <= Bottom();
		}

		constexpr bool Overlaps(const Rectangle<T>& ar) const
		{
			return
				Get_Horizontal().Overlaps(Range<T>(ar.left(), ar.right())) &&
				Get_Vertical().Overlaps(Range<T>(ar.top(), ar.bottom()));
		}

		constexpr bool Straight() const
		{
			return left_top == right_bottom;
		}

		constexpr bool Empty() const
		{
			return left_top.Straight() && right_bottom.Straight() && Straight();
		}

		constexpr const Point<T>& Get_Left_Top() const
		{
			return left_top;
		}

		constexpr const Point<T>& Get_Right_Bottom() const
		{
			return right_bottom;
		}

		constexpr Range<T> Get_Horizontal() const
		{
			return { Left(), Right() };
		}

		constexpr Range<T> Get_Vertical() const
		{
			return { Top(), Bottom() };
		}

		void Shift(const Point<T>& v)
		{
			left_top = left_top + v;
			right_bottom = right_bottom + v;
		}

	private:
		Point<T> left_top;
		Point<T> right_bottom;
	};
}