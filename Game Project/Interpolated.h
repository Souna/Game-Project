#pragma once
#include "Lerp.h"

namespace game
{
	template <typename T>
	class Nominal
	{
	public:
		constexpr Nominal() : now_(T()), before_(T()), threshold_(0.0f) {}

		T Get() const
		{
			return now_;
		}

		T Get(float alpha) const
		{
			return alpha >= threshold_ ? now_ : before_;
		}

		T Last() const
		{
			return before_;
		}

		void Set(T value)
		{
			now_ = value;
			before_ = value;
		}

		void Normalize()
		{
			before_ = now_;
		}

		bool Normalized() const
		{
			return before_ == now_;
		}

		void Next(T value, float thrs)
		{
			before_ = now_;
			now_ = value;
			threshold_ = thrs;
		}

		bool operator == (T value) const
		{
			return now_ == value;
		}

		bool operator != (T value) const
		{
			return now_ != value;
		}

		T operator + (T value) const
		{
			return now_ + value;
		}

		T operator - (T value) const
		{
			return now_ - value;
		}

		T operator * (T value) const
		{
			return now_ * value;
		}

		T operator / (T value) const
		{
			return now_ / value;
		}

	private:
		T now_;
		T before_;
		float threshold_;
	};


	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================


	template <typename T>
	class Linear
	{
	public:
		T Get() const
		{
			return now_;
		}

		T Get(float alpha) const
		{
			return Lerp<T>(before_, now_, alpha);
		}

		T Last() const
		{
			return before_;
		}

		void Set(T value)
		{
			now_ = value;
			before_ = value;
		}

		void Normalize()
		{
			before_ = now_;
		}

		bool Normalized() const
		{
			return before_ == now_;
		}

		void operator = (T value)
		{
			before_ = now_;
			now_ = value;
		}

		void operator += (T value)
		{
			before_ = now_;
			now_ += value;
		}

		void operator -= (T value)
		{
			before_ = now_;
			now_ -= value;
		}

		bool operator == (T value) const
		{
			return now_ == value;
		}

		bool operator != (T value) const
		{
			return now_ != value;
		}

		bool operator < (T value) const
		{
			return now_ < value;
		}

		bool operator <= (T value) const
		{
			return now_ <= value;
		}

		bool operator > (T value) const
		{
			return now_ > value;
		}

		bool operator >= (T value) const
		{
			return now_ >= value;
		}

		T operator + (T value) const
		{
			return now_ + value;
		}

		T operator - (T value) const
		{
			return now_ - value;
		}

		T operator * (T value) const
		{
			return now_ * value;
		}

		T operator / (T value) const
		{
			return now_ / value;
		}

		T operator + (Linear<T> value) const
		{
			return now_ + value.get();
		}

		T operator - (Linear<T> value) const
		{
			return now_ - value.get();
		}

		T operator * (Linear<T> value) const
		{
			return now_ * value.get();
		}

		T operator / (Linear<T> value) const
		{
			return now_ / value.get();
		}

	private:
		T now_;
		T before_;
	};
}