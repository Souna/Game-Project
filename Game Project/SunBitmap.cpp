#include "SunBitmap.h"
#include "SunReader.h"
#include <vector>
#include "zlib.h"
#include <algorithm>
#include "SunNode.h"
#include "Window.h"

uint8_t const table4[0x10] =
{
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
uint8_t const table5[0x20] =
{
	0x00, 0x08, 0x10, 0x19, 0x21, 0x29, 0x31, 0x3A, 0x42, 0x4A, 0x52,
	0x5A, 0x63, 0x6B, 0x73, 0x7B, 0x84, 0x8C, 0x94, 0x9C, 0xA5, 0xAD,
	0xB5, 0xBD, 0xC5, 0xCE, 0xD6, 0xDE, 0xE6, 0xEF, 0xF7, 0xFF
};
uint8_t const table6[0x40] =
{
	0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2D, 0x31, 0x35, 0x39, 0x3D,
	0x41, 0x45, 0x49, 0x4D, 0x51, 0x55, 0x59, 0x5D, 0x61, 0x65, 0x69, 0x6D, 0x71, 0x75, 0x79, 0x7D,
	0x82, 0x86, 0x8A, 0x8E, 0x92, 0x96, 0x9A, 0x9E, 0xA2, 0xA6, 0xAA, 0xAE, 0xB2, 0xB6, 0xBA, 0xBE,
	0xC2, 0xC6, 0xCA, 0xCE, 0xD2, 0xD7, 0xDB, 0xDF, 0xE3, 0xE7, 0xEB, 0xEF, 0xF3, 0xF7, 0xFB, 0xFF
};

template <int N>
void Scale(std::vector<uint8_t> const& input, std::vector<uint8_t>& output, int width, int height)
{
	auto in = reinterpret_cast<uint32_t const*>(input.data());
	auto out = reinterpret_cast<uint32_t*>(output.data());
	auto w = width / N;
	auto h = height / N;
	for (auto y = 0; y < h; ++y)
	{
		for (auto x = 0; x < w; ++x)
		{
			auto p = in[y * w + x];
			for (auto yy = y * N; yy < (y + 1) * N; ++yy)
			{
				for (auto xx = x * N; xx < (x + 1) * N; ++xx)
				{
					out[yy * width + xx] = p;
				}
			}
		}
	}
};

SunBitmap::SunBitmap(int32_t h, int32_t w, uint32_t len, uint32_t o, std::shared_ptr<SunReader> r)
	: height_(h), width_(w), length_(len), offset_(o), reader_(r)
{
}

SunBitmap::~SunBitmap() {}

auto SunBitmap::Data() -> std::vector<uint8_t>&
{
	if (!input_.empty()) return input_;

	auto size = width_ * height_ * 4;
	auto biggest = std::max(size, length_);
	std::vector<uint8_t> output;

	input_.resize(biggest);
	output.resize(biggest);
	buffer_ = reader_->Get_Offset(offset_ + 4);	// Skips image data size and puts us at 78 9C

	const auto* original = reinterpret_cast<uint8_t const*>(buffer_);
	auto decompressed = 0;

	auto decompress = [&]
	{
		z_stream stream = {};
		stream.next_in = input_.data();
		stream.avail_in = length_;
		inflateInit(&stream);
		stream.next_out = output.data();
		stream.avail_out = static_cast<unsigned>(output.size());
		auto error = inflate(&stream, Z_FINISH);
		if (error != Z_BUF_ERROR)
		{
			if (error != Z_DATA_ERROR)
			{
				std::cerr << "zlib error of " << std::dec << error << std::endl;
			}
			return false;
		}
		decompressed = static_cast<int32_t>(stream.total_out);
		inflateEnd(&stream);
		return true;
	};

	std::copy(original, original + length_, input_.begin());

	if (!decompress())
	{
		decompressed = size;
		std::fill(output.begin(), output.begin() + size, 0);
	}
	input_.swap(output);

	struct color4444
	{
		uint8_t b : 4;
		uint8_t g : 4;
		uint8_t r : 4;
		uint8_t a : 4;
	};
	static_assert(sizeof(color4444) == 2, "Your bitpacking sucks");
	struct color8888
	{
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t a;
	};
	static_assert(sizeof(color8888) == 4, "Your bitpacking sucks");
	struct color565
	{
		uint16_t b : 5;
		uint16_t g : 6;
		uint16_t r : 5;
	};
	(sizeof(color565) == 2, "Your bitpacking sucks");
	auto pixels4444 = reinterpret_cast<color4444*>(input_.data());
	auto pixels8888 = reinterpret_cast<color8888*>(input_.data());
	auto pixels565 = reinterpret_cast<color565*>(input_.data());
	auto pixelsout = reinterpret_cast<color8888*>(output.data());
	//Sanity check the sizes
	auto check = decompressed;

	auto pixels = width_ * height_;

	if (check != pixels * 4)
	{
		//std::cerr << "Size mismatch: " << std::dec << width_ << "," << height_ << "," << decompressed << "," << format_ <<
		//	"," << format2_ << std::endl;
		std::cerr << "SHIT";
		//throw std::runtime_error("halp!");
	}

	pxvector_.resize(pixels);

	for (auto i = 0; i < pixels; ++i)
	{
		auto& p = pixels8888[i];
		pixelsout[i] = { p.r, p.g, p.b, p.a };
		pxvector_[i].n = 0;	// <-- See what I can do with this, or if it even matters at all.
		pxvector_[i].r = p.r;
		pxvector_[i].g = p.g;
		pxvector_[i].b = p.b;
		pxvector_[i].a = p.a;
	}

	input_.swap(output);
	output.clear();
	return input_;
}

auto SunBitmap::To_Sprite() -> olc::Sprite*
{
	int pxiterator = 0;
	olc::Sprite* outSprite = new olc::Sprite(width_, height_);
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			outSprite->SetPixel(x, y, pxvector_[pxiterator]);
			pxiterator++;
		}
	}
	return outSprite;
}

auto SunBitmap::To_Decal() -> olc::Decal*
{
	int pxiterator = 0;
	olc::Sprite* conversion_sprite = new olc::Sprite(width_, height_);
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			conversion_sprite->SetPixel(x, y, pxvector_[pxiterator]);
			pxiterator++;
		}
	}
	olc::Decal* out_decal = new olc::Decal(conversion_sprite);
	return out_decal;
}

auto SunBitmap::Set_Width(int32_t width) -> void
{
	this->width_ = width;
}

auto SunBitmap::Content() const -> std::unique_ptr<int8_t[]>
{
	//reader_->Set_Position(offset_ + 1);
	reader_->Set_Position(offset_);
	return reader_->Read_Bytes(length_);
}