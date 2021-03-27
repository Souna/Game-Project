#include "Texture.h"
#include "Window.h"
#include "olcPixelGameEngine.h"

namespace game
{
	Texture::Texture(SunNode src)
	{
		if (src.Get_Node_Type() == SunNode::NodeType::BITMAP)
		{
			origin_ = src["origin"];
			std::string source = src["source"];

			auto foundSource = Find_Child(src, source);
			auto foundChild = foundSource;
			if (foundChild)
				src = foundSource;
			bitmap_ = src;
			dimensions_ = Point<int16_t>(bitmap_.Get_Width(), bitmap_.Get_Height());
		}
	}

	Texture::Texture() {}

	Texture::~Texture() {}

	auto Texture::Draw() const -> void
	{
		size_t id = bitmap_.Id();

		if (id == 0) return;

		//Window::Get().DrawSprite({ 0,0 }, bitmap_);
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
}