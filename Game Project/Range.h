#pragma once
#include <functional>
namespace game
{
	// This class is entirely self-contained so there is no need for a .cpp file.
	template <class T>
	class Range
	{
	public:
		// Construct a range from the specified values.
		constexpr Range(const T& first, const T& second) : a_(first), b_(second) {}

		// Construct a range of (0, 0).
		constexpr Range() : Range(0, 0) {}

		// Return the first value of a range.
		constexpr const T& First() const
		{
			return a_;
		}

		// Return the second value of a range.
		constexpr const T& Second() const
		{
			return b_;
		}

		// Return the greater value of a range.
		constexpr const T& Greater() const
		{
			return (a_ > b_) ? a_ : b_;
		}

		// Return the smaller value of a range.
		constexpr const T& Smaller() const
		{
			return (a_ < b_) ? a_ : b_;
		}

		// Return the difference between the values in a range.
		constexpr T Delta() const
		{
			return b_ - a_;
		}

		// Return the absolute difference between the values in a range.
		constexpr T Length() const
		{
			return Greater() - Smaller();
		}

		// Return the mean of both range values.
		constexpr T Center() const
		{
			return (a_ + b_) / 2;
		}

		// Check if both range values are equal. If so, the range is considered an empty range.
		constexpr bool Is_Empty() const
		{
			return a_ == b_;
		}

		// Check if the range contains a value.
		constexpr bool Contains(const T& v) const
		{
			return v >= a_ && v <= b_;
		}

		// Check if the range contains another range.
		constexpr bool Contains(const Range<T>& v) const
		{
			return v.a_ >= a_ && v.b_ <= b_;
		}

		// Check if the ranges overlap.
		constexpr bool Overlaps(const Range<T>& v) const
		{
			return Contains(v.a_) || Contains(v.b_) || v.Contains(a_) || v.Contains(b_);
		}

		// Check whether the range is equivalent to another range.
		constexpr bool operator == (const Range<T>& v) const
		{
			return a_ == v.a_ && b_ == v.b_;
		}

		// Check whether the range is not equivalent to another range.
		constexpr bool operator != (const Range<T>& v) const
		{
			return !(*this == v);
		}

		// Shift this range by the amounts defined by another range.
		constexpr Range<T> operator + (const Range<T>& v) const
		{
			return { a_ + v.a_, b_ + v.b_ };
		}

		// Shift this range by the negative amounts defined by another range.
		constexpr Range<T> operator - (const Range<T>& v) const
		{
			return { a_ - v.a_, b_ - v.b_ };
		}

		// Return the negative of this range.
		constexpr Range<T> operator - () const
		{
			std::negate<T> negator;
			return { negator(a_), negator(b_)};
		}

		// Construct a symmetric range around mid.
		static Range<T> Symmetric(const T& mid, const T& tail)
		{
			return { static_cast<T>(mid - tail), static_cast<T>(mid + tail) };
		}

	private:
		T a_;
		T b_;
	};
}