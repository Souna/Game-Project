#pragma once
#include "olcPixelGameEngine.h"
#include "Point.h"
#include "SunNode.h"
#include "SunBitmap.h"
#include "DrawArgument.h"

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

		auto Draw(const DrawArgument& arguments) const -> void;
		auto Shift(Point<int16_t> amount) -> void;

		auto Is_Valid() const -> bool;
		auto Width() const->int16_t;
		auto Height() const->int16_t;
		auto Get_Origin() const->Point<int16_t>;
		auto Get_Dimensions() const->Point<int16_t>;
		auto Find_Child(SunNode src, std::string)->SunNode;
		auto Add_Bitmap(const SunBitmap& bitmap)->void;
		auto Data_To_Pixel_Array(const std::vector<uint8_t>& data) -> std::unique_ptr<olc::Pixel[]>;

	private:
		std::shared_ptr<olc::Sprite> sprite_ = std::make_shared<olc::Sprite>();
		std::shared_ptr<olc::Decal> decal_;
		SunBitmap bitmap_;
		Point<int16_t> origin_;
		Point<int16_t> dimensions_;
	};
}