#pragma once
#include "olcPixelGameEngine.h"
#include "Point.h"
#include "SunNode.h"
#include "SunBitmap.h"

namespace game
{
	/**
	* Represents a single image loaded from a file.
	*/
	class Texture
	{
	public:
		Texture(SunNode source);
		Texture();
		~Texture();

		auto Draw() const -> void;
		auto Shift(Point<int16_t> amount) -> void;

		auto Is_Valid() const -> bool;
		auto Width() const -> int16_t;
		auto Height() const -> int16_t;
		auto Get_Origin() const -> Point<int16_t>;
		auto Get_Dimensions() const -> Point<int16_t>;
		auto Find_Child(SunNode src, std::string) -> SunNode;

	private:
		SunBitmap bitmap_;
		Point<int16_t> origin_;
		Point<int16_t> dimensions_;
	};
}