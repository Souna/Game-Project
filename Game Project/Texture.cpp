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
		}
	}

	Texture::Texture() {}

	Texture::~Texture() {}

	auto Texture::Draw() const -> void
	{
		size_t id = bitmap_.Id();
		if (id == 0) return;

		bitmap_.Data();
		olc::Sprite* sprite = bitmap_.To_Sprite();
		Window::Get().DrawSprite(origin_.X(), origin_.Y(), sprite);
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