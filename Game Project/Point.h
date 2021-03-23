#pragma once

#include <string>
#include <cmath>
#include "SunNode.h"

namespace game
{
	/**
	* A single point on the screen.
	*/
	template <class T>
	class Point
	{
	public:
		// Construct a point from a vector property.
		Point(SunNode source)
		{
			a_ = static_cast<T>(source.X());
			b_ = static_cast<T>(source.Y());
		}

		// Construct a point from the specified coordinates.
		constexpr Point(T first, T second) : a_(first), b_(second) {}

		// Construct a point with coordinates (0, 0).
		constexpr Point() : Point(0, 0) {}

		// Return the x-coordinate of a point.
		constexpr T X() const
		{
			return a_;
		}

		// Return the y-coordinate of a point.
		constexpr T Y() const
		{
			return b_;
		}

		// Return the inner product of a point.
		constexpr T Length() const
		{
			return static_cast<T>(
				std::sqrt(a_ * a_ + b_ * b_)
				);
		}

		// Check whether the point coordinates are equal.
		constexpr bool Straight() const
		{
			return a_ == b_;
		}

		// Return a string representation of the point.
		std::string To_String() const
		{
			return "(" + std::to_string(a_) + "," + std::to_string(b_) + ")";
		}

		// Return the distance to another point.
		constexpr T Distance(Point<T> v) const
		{
			return Point<T>(a_ - v.a_, b_ - v.b_)
				.Length();
		}

		// Set the point's x-coordinate.
		void Set_X(T v)
		{
			a_ = v;
		}

		// Set the point's y-coordinate.
		void Set_Y(T v)
		{
			b_ = v;
		}

		// Shift the point's x-coordinate by the specified amount.
		void Shift_X(T v)
		{
			a_ += v;
		}

		// Shift the point's y-coordinate by the specified amount.
		void Shift_Y(T v)
		{
			b_ += v;
		}

		// Shift the point's coordinates by the specified amounts.
		void Shift(T x, T y)
		{
			a_ += x;
			b_ += y;
		}

		// Shift this point by the amounts defined by another point. Equivalent to operator +=.
		void Shift(Point<T> v)
		{
			a_ += v.a_;
			b_ += v.b_;
		}

		// Take the absolute value of the point.
		Point<T> Absolute()
		{
			return {
				static_cast<T>(std::abs(a_)),
				static_cast<T>(std::abs(b_))
			};
		}

		// Check whether point is equivalent to the specified point.
		constexpr bool operator == (const Point<T>& v) const
		{
			return a_ == v.a_ && b_ == v.b_;
		}

		// Check whether point is not equivalent to the specified point.
		constexpr bool operator != (const Point<T>& v) const
		{
			return !(*this == v);
		}

		// Shift this point by the amounts defined by another point.
		void operator += (Point<T> v)
		{
			a_ += v.a_;
			b_ += v.b_;
		}

		// Shift this point in reverse direction by the amounts defined by another point.
		void operator -= (Point<T> v)
		{
			a_ -= v.a_;
			b_ -= v.b_;
		}

		// Return a point whose coordinates are the negation of this point's coordinates.
		constexpr Point<T> operator - () const
		{
			return { -a_, -b_ };
		}

		// Return a point whose coordinates have been added the specified amount.
		constexpr Point<T> operator + (T v) const
		{
			return { a_ + v, b_ + v };
		}

		// Return a point whose coordinates have been subtracted the specified amount.
		constexpr Point<T> operator - (T v) const
		{
			return { a_ - v, b_ - v };
		}

		// Return a point whose coordinates have been multiplied by the specified amount.
		constexpr Point<T> operator * (T v) const
		{
			return { a_ * v, b_ * v };
		}

		// Return a point whose coordinates have been divided by the specified amount.
		constexpr Point<T> operator / (T v) const
		{
			return { a_ / v, b_ / v };
		}

		// Return a point whose coordinates are the sum of this and another point's coordinates.
		constexpr Point<T> operator + (Point<T> v) const
		{
			return { a_ + v.a_, b_ + v.b_ };
		}

		// Return a point whose coordinates are the difference of this and another point's coordinates.
		constexpr Point<T> operator - (Point<T> v) const
		{
			return { a_ - v.a_, b_ - v.b_ };
		}

		// Return a point whose coordinates are the product of this and another point's coordinates.
		constexpr Point<T> operator * (Point<T> v) const
		{
			return { a_ / v.a_, b_ / v.b_ };
		}

		// Return a point whose coordinates are the division of this and another point's coordinates.
		constexpr Point<T> operator / (Point<T> v) const
		{
			return {
				a_ / (v.a_ == 0 ? 1 : v.a_),
				b_ / (v.b_ == 0 ? 1 : v.b_)
			};
		}

	private:
		T a_;
		T b_;
	};
}