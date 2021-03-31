#pragma once
#include "SunTools.h"
#include <vector>
#include "olcPixelGameEngine.h"

class SunReader;
/**
* A property containing a bitmap image.
*/
class SunBitmap
{
public:
	SunBitmap() = default;
	SunBitmap(int32_t height, int32_t width, uint32_t length, uint32_t offset, std::shared_ptr<SunReader> reader);
	~SunBitmap();

	inline auto Id() const { return offset_; }
	inline auto Get_Width() const { return width_; }
	inline auto Get_Height() const { return height_; }
	inline auto Get_Length() const { return length_; }

	auto Data()->std::vector<uint8_t>&;
	// Converts a SunBitMap to an olc::Sprite*.
	auto To_Sprite()->olc::Sprite*;
	// Converts a SunBitMap to an olc::Sprite* and then converts said sprite
	// into an olc::Decal* before returning. Decals live on the GPU.
	auto To_Decal()->olc::Decal*;
	auto Set_Width(int32_t width) -> void;

	uint32_t height_ = 0;
	uint32_t width_ = 0;
	uint32_t length_ = 0;
	uint32_t offset_ = 0;

	std::shared_ptr<SunReader> reader_ = nullptr;
	std::vector<uint8_t> input_;
	const int8_t* buffer_ = nullptr;

private:
	auto Content() const->std::unique_ptr<int8_t[]>;
	std::vector<olc::Pixel> pxvector_;
};