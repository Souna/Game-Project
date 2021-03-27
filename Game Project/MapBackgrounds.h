#pragma once

#include "olcPixelGameEngine.h"
#include "SunNode.h"
#include "Animation.h"
#include "Console.h"
#include "PhysicsObject.h"

namespace game
{
	/**
	* A single background image with all of its properties.
	*/
	class Background
	{
	public:
		Background(SunNode src);

		// Updates background positions/animations.
		void Update();
		// Draws background to screen using PGE functions.
		void Draw();
	private:
		enum class Type
		{
			// Add different types later. Tiled, Move, etc.
			NORMAL
		};

		static Type Type_By_Id(int32_t id)
		{
			if (id >= static_cast<int>(Type::NORMAL) && id <= static_cast<int>(Type::NORMAL))
				return static_cast<Type>(id);
			// else...

			Console::Get().Print(__func__, "Unhandled background type -- ID: " + std::to_string(id));
			return Type::NORMAL;
		}

		// Changes the background type.
		// [NORMAL, ...]
		void Set_Type(Type type);

		int16_t view_width_;
		int16_t view_height_;
		int16_t width_offset_;
		int16_t height_offset_;

		// Contains actual image.
		Animation animation_;
		bool animated_;

		// Copy values.
		int16_t copy_x_;
		int16_t copy_y_;
		// Parallax values.
		double rotate_x_;
		double rotate_y_;
		int16_t horizonal_tile_;
		int16_t vertical_tile_;
		float opacity_;
		bool flipped_;

		MovingObject moving_object_;
	};

	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================

	/**
	* A collection of background objects (images) for use on maps.
	*/
	class MapBackgrounds
	{
	public:
		MapBackgrounds(const char* path);
		MapBackgrounds(SunNode src);
		MapBackgrounds();

		// Calls Draw() function of backgrounds.
		void Draw_Backgrounds();
		// Calls Draw() function of foregrounds.
		void Draw_Foregrounds();
		void Update();

	private:
		// Contains private vectors of backgrounds and foregrounds, of the background type.
		std::vector<Background> backgrounds_vector_;
		std::vector<Background> foregrounds_vector_;
		bool black;
	};
}