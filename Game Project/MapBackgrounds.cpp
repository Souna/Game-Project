#include "MapBackgrounds.h"
#include "Window.h"
#include "SunBitmap.h"
#include "SunFile.h"

namespace game
{
	// ["Map.sun"]["Map"]["Map1"]["back"]["0"] <-- this is what src is
	Background::Background(SunNode src) : animated_(false)
	{
		view_width_ = constants::Constants::Get().Get_View_Width();
		view_height_ = constants::Constants::Get().Get_View_Width();
		width_offset_ = view_width_ / 2;
		height_offset_ = view_height_ / 2;

		// Just selects ["Map.sun"]["Back"], where all the background images are stored.
		SunNode back_src = sun_file::map["Back"];
		// Checks the animated property of the current background number in the parameter.
		animated_ = src["animated"].Get_Boolean();

		// The actual image from the file.
		animation_ = back_src[src["set"]][animated_ ? "animated" : "still"][src["no"]];

	}

	void Background::Update()
	{
	}

	void Background::Draw()
	{
		//Window::Get().DrawDecal({ 0,0 }, dbackground_ /*{0.25f,0.19f}*/);
		//Window::Get().DrawSprite({ 0, 0 }, sbackground_);
	}

	void Background::Set_Type(Type type)
	{

	}


	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================


	MapBackgrounds::MapBackgrounds(const char* path)
	{
		//backgrounds_vector_.push_back(path);
	}

	MapBackgrounds::MapBackgrounds(SunNode src)
	{
		//["Map.sun"]["Map"]["Map1"]["back"]
		std::map<std::string, SunNode>::iterator iter;

		//for (iter = src.Begin(); iter != src.End(); ++iter)
		//{
		//	// Grab each image node sequentially regardless of their name.
		//	// Later on backgrounds will be stored with their names zero-based and ascending.
		//	// At that point we will change the method for iterating through the nodes here.
		//	backgrounds_vector_.push_back(src.Get_Child(iter->first.c_str()));
		//}

		int16_t bg_number = 0;
		SunNode back = src[std::to_string(bg_number)]; //["Map.sun"]["Map"]["Map1"]["back"][bg_number]
		while (back.Children_Size() > 0)	// alpha/animated/bS/cx/cy... etc
		{
			bool front = back["front"].Get_Boolean();

			if (front)
				foregrounds_vector_.push_back(back);
			else
				backgrounds_vector_.push_back(back);

			bg_number++;
			back = src[std::to_string(bg_number)];
		}
		black = false;
	}

	MapBackgrounds::MapBackgrounds()
	{}

	void MapBackgrounds::Draw_Backgrounds()
	{
		for (auto& background : backgrounds_vector_)
		{
			background.Draw();
		}
	}

	void MapBackgrounds::Draw_Foregrounds()
	{
		for (auto& foreground : foregrounds_vector_)
		{
			foreground.Draw();
		}
	}

	void MapBackgrounds::Update()
	{
		for (auto& background : backgrounds_vector_)
			background.Update();
		for (auto& foreground : foregrounds_vector_)
			foreground.Update();
	}
}