#include "MapBackgrounds.h"
#include "Window.h"
#include "SunBitmap.h"
#include "SunFile.h"

namespace game
{
	MapBackgrounds::MapBackgrounds() {}

	MapBackgrounds::MapBackgrounds(SunNode back_folder)
	{
		int16_t bg_number = 0;

		SunNode background = back_folder[std::to_string(bg_number)];
		
		//Iterate through all of the properties of the background.
		while (background.Children_Size() > 0)
		{
			bool front = background["front"].Get_Boolean();

			if (front)
				foregrounds_vector_.emplace_back(background);
			else
				backgrounds_vector_.emplace_back(background);

			bg_number++;
			background = back_folder[std::to_string(bg_number)];
		}

		black = back_folder["0"]["backgroundSet"].Get_String() == "";	// Map is drawn with black background if value of 0 is blank.
	}

	void MapBackgrounds::Draw_Backgrounds(double view_x, double view_y, float alpha) const
	{
		if (black)
			Window::Get().Clear(olc::BLACK);

		for (auto& background : backgrounds_vector_)
		{
			background.Draw(view_x, view_y, alpha);
		}
	}

	void MapBackgrounds::Draw_Foregrounds(double view_x, double view_y, float alpha) const
	{
		for (auto& foreground : foregrounds_vector_)
		{
			foreground.Draw(view_x, view_y, alpha);
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