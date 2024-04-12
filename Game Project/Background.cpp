#include "SunNode.h"
#include "Constants.h"
#include "Background.h"
#include "SunFile.h"
#include "DrawArgument.h"

namespace game
{
	Background::Background(SunNode background) : animated_(false)
	{
		view_width_ = constants::Constants::Get().Get_View_Width();
		view_height_ = constants::Constants::Get().Get_View_Height();
		width_offset_ = view_width_ / 2;
		height_offset_ = view_height_ / 2;

		// Just selects ["Map.sun"]["Back"], where all the background images are stored.
		SunNode background_storage = sun_file::Map["Back"];

		// Checks the animated property of the current background number.
		animated_ = background["ani"].Get_Boolean();

		// Resolve the actual background image and assign it to animation_.
		animation_ = background_storage[background["backgroundSet"] + ".img"][animated_ ? "Ani" : "Back"][background["bgNumber"]];
		
		opacity_ = background["a"];
		flipped_ = background["flip"].Get_Boolean();
		copy_x_ = background["cx"];
		copy_y_ = background["cy"];
		rotate_x_ = background["rx"];
		rotate_y_ = background["ry"];
		moving_object_.Set_X(background["x"]);
		moving_object_.Set_Y(background["y"]);

		Type type = Type_By_Id(background["type"]);
		Set_Type(type);
	}

	void Background::Update()
	{
		moving_object_.Move();
		animation_.Update();
	}

	void Background::Draw(double view_x, double view_y, float alpha) const
	{
		double x;

		if (moving_object_.Horizontal_Mobile())
		{
			x = moving_object_.Get_Absolute_X(view_x, alpha);
		}
		else
		{
			double shift_x = rotate_x_ * (width_offset_ - view_x) / 100 + width_offset_;
			x = moving_object_.Get_Absolute_X(shift_x, alpha);
		}

		double y;

		if (moving_object_.Vertical_Mobile())
		{
			y = moving_object_.Get_Absolute_Y(view_y, alpha);
		}
		else
		{
			double shift_y = rotate_y_ * (height_offset_ - view_y) / 100 + height_offset_;
			y = moving_object_.Get_Absolute_Y(shift_y, alpha);
		}

		if (horizonal_tile_ > 1)
		{
			while (x > 0)
				x -= copy_x_;

			while (x < -copy_x_)
				x += copy_x_;
		}

		if (vertical_tile_ > 1)
		{
			while (y > 0)
				y -= copy_y_;

			while (y < -copy_y_)
				y += copy_y_;
		}

		int16_t ix = static_cast<int16_t>(std::round(x));
		int16_t iy = static_cast<int16_t>(std::round(y));

		int16_t tw = copy_x_ * horizonal_tile_;
		int16_t th = copy_y_ * vertical_tile_;

		for (int16_t tx = 0; tx < tw; tx += copy_x_)
			for (int16_t ty = 0; ty < th; ty += copy_y_)
			{
				animation_.Draw(DrawArgument(Point<int16_t>(ix + tx, iy + ty), flipped_, opacity_ / 255), alpha);
			}
	}

	void Background::Set_Type(Type type)
	{
		// Get width and height of background image
		int16_t dim_x = animation_.Get_Dimensions().X();
		int16_t dim_y = animation_.Get_Dimensions().Y();

		// TODO: Double check for zero. Is this a wz reading issue?
		if (copy_x_ == 0)
			copy_x_ = (dim_x > 0) ? dim_x : 1;

		if (copy_y_ == 0)
			copy_y_ = (dim_y > 0) ? dim_y : 1;

		// Values of 'repeatedness'?
		horizonal_tile_ = 1;
		vertical_tile_ = 1;

		switch (type)
		{
		case Type::HORIZONTAL_TILED:
		case Type::HORIZONTAL_MOVE_A:
			horizonal_tile_ = view_width_ / copy_x_ + 3;
			break;
		case Type::VERTICAL_TILED:
		case Type::VERTICAL_MOVE_A:
			vertical_tile_ = view_height_ / copy_y_ + 3;
			break;
		case Type::TILED:
		case Type::HORIZONTAL_MOVE_B:
		case Type::VERTICAL_MOVE_B:
			horizonal_tile_ = view_width_ / copy_x_ + 3;
			vertical_tile_ = view_height_ / copy_y_ + 3;
			break;
		}

		switch (type)
		{
		case Type::HORIZONTAL_MOVE_A:
		case Type::HORIZONTAL_MOVE_B:
			moving_object_.horizontal_speed = rotate_x_ / 64;
			break;
		case Type::VERTICAL_MOVE_A:
		case Type::VERTICAL_MOVE_B:
			moving_object_.vertical_speed = rotate_y_ / 64;
			break;
		}
	}
}