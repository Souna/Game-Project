#pragma once

namespace game
{
	template <typename T>
	constexpr T Lerp(T first, T second, float alpha)
	{
		return alpha <= 0.0f ? first
			: alpha >= 1.0f ? second
			: first == second ? first
			: static_cast<T>((1.0f - alpha) * first + alpha * second);
	}
}