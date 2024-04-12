#pragma once
#include "SunNode.h"
#include "Animation.h"
#include "PhysicsObject.h"
#include "Console.h"

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
		void Draw(double view_x, double view_y, float alpha) const;
	private:
		enum class Type
		{
			NORMAL,
			HORIZONTAL_TILED,
			VERTICAL_TILED,
			TILED,
			HORIZONTAL_MOVE_A,
			VERTICAL_MOVE_A,
			HORIZONTAL_MOVE_B,
			VERTICAL_MOVE_B
		};

		static Type Type_By_Id(int32_t id)
		{
			if (id >= static_cast<int>(Type::NORMAL) && id <= static_cast<int>(Type::VERTICAL_MOVE_B))
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
}