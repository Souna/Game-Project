#include "Texture.h"
#include "Window.h"
#include "olcPixelGameEngine.h"

namespace game
{
	Texture::Texture(SunNode canvas_node)
	{
		if (canvas_node.Get_Node_Type() == SunNode::SunPropertyType::BITMAP)
		{
			origin_ = canvas_node["origin"];
			std::string source = canvas_node["source"];

			auto foundSource = Find_Child(canvas_node, source);
			auto foundChild = foundSource;
			if (foundChild)
				canvas_node = foundSource;
			bitmap_ = canvas_node;
			dimensions_ = Point<int16_t>(bitmap_.Get_Width(), bitmap_.Get_Height());
			Add_Bitmap(bitmap_);
			decal_ = std::make_shared<olc::Decal>(sprite_.get());
		}
	}

	Texture::Texture() {}

	Texture::~Texture() {}

	auto Texture::Draw(const DrawArgument& arguments) const -> void
	{
		size_t id = bitmap_.Id();
		if (id == 0) return;

		Rectangle<int16_t> rectangle = arguments.get_rectangle(origin_, dimensions_);
		Color color = arguments.get_color();
		float angle = arguments.get_angle();

		if (color.invisible())
			return;

		olc::vf2d position = { static_cast<float>(rectangle.Left()), static_cast<float>(rectangle.Top()) };
		olc::vf2d scale = { static_cast<float>(rectangle.Width()) / static_cast<float>(bitmap_.Get_Width()),
							static_cast<float>(rectangle.Height()) / static_cast<float>(bitmap_.Get_Height()) };

		Window::Get().DrawRotatedDecal(position, decal_.get(), angle);
	}

	auto Texture::Shift(Point<int16_t> amount) -> void
	{
		origin_ -= amount;
	}

	auto Texture::Is_Valid() const -> bool
	{
		return bitmap_.Id() > 0;
	}

	auto Texture::Width() const -> int16_t
	{
		return dimensions_.X();
	}

	auto Texture::Height() const -> int16_t
	{
		return dimensions_.Y();
	}

	auto Texture::Get_Origin() const -> Point<int16_t>
	{
		return origin_;
	}

	auto Texture::Get_Dimensions() const -> Point<int16_t>
	{
		return dimensions_;
	}

	auto Texture::Find_Child(SunNode src, std::string link) -> SunNode
	{
		if (!link.empty())
		{
			SunNode parent_node = src.Root();

			return parent_node.Resolve((link.substr(link.find('/') + 1)).c_str());
		}

		return SunNode();
	}

	auto Texture::Add_Bitmap(const SunBitmap& bitmap) -> void
	{
		sprite_->width = bitmap.Get_Width();
		sprite_->height = bitmap.Get_Height();
		auto data = bitmap_.Data();
		auto pixelArray = Data_To_Pixel_Array(data);
		sprite_->pColData = pixelArray.release();
	}

	auto Texture::Data_To_Pixel_Array(const std::vector<uint8_t>& data) -> std::unique_ptr<olc::Pixel[]>
	{
		auto pixelCount = data.size() / 4;
		auto pixels = std::make_unique<olc::Pixel[]>(pixelCount);

		for (size_t i = 0; i < pixelCount; ++i)
		{
			size_t baseIndex = i * 4;
			uint8_t red = data[baseIndex];
			uint8_t green = data[baseIndex + 1];
			uint8_t blue = data[baseIndex + 2];
			uint8_t alpha = data[baseIndex + 3];
			pixels[i] = olc::Pixel(red, green, blue, alpha);
		}

		return pixels;
	}
}