#pragma once

#include "olcPixelGameEngine.h"
#include "SunNode.h"
#include "Animation.h"
#include "Console.h"
#include "PhysicsObject.h"
#include "Background.h"

namespace game
{
	/**
	* A collection of background objects for use on maps.
	*/
	class MapBackgrounds
	{
	public:
		MapBackgrounds();
		MapBackgrounds(SunNode src);

		// Calls Draw() function of backgrounds.
		void Draw_Backgrounds(double view_x, double view_y, float alpha) const;
		// Calls Draw() function of foregrounds.
		void Draw_Foregrounds(double view_x, double view_y, float alpha) const;

		void Update();

	private:
		// Containers for backgrounds and foregrounds.
		std::vector<Background> backgrounds_vector_;
		std::vector<Background> foregrounds_vector_;

		// If true, map background is black.
		bool black;
	};
}