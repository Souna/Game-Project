#pragma once

#include <vector>
#include "Point.h"
#include <fstream>
#include "SunNode.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Interpolated.h"
#include "DrawArgument.h"

namespace game
{
	/**
	* A single frame within an Animation.
	*/
	class Frame
	{
	public:
		Frame(SunNode src);
		Frame();

		auto Draw(const DrawArgument& arguments) const -> void;

		auto Start_Opacity() const->uint8_t;
		auto Start_Scale() const->uint16_t;
		auto Get_Delay() const->uint16_t;
		auto Get_Origin() const->Point<int16_t>;
		auto Get_Dimensions() const->Point<int16_t>;
		auto Get_Head() const->Point<int16_t>;
		auto Get_Bounds() const->Rectangle<int16_t>;
		auto Opacity_Step(uint16_t timestep) const -> float;
		auto Scale_Step(uint16_t timestep) const -> float;

	private:
		Texture texture_;
		uint16_t delay_;
		std::pair<uint8_t, uint8_t> opacities_;
		std::pair<int16_t, int16_t> scales_;
		Rectangle<int16_t> bounds_;
		Point<int16_t> head_;
	};

	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================

	/**
	* A class which consists of multiple frames/textures to make an Animation.
	*/
	class Animation
	{
	public:
		Animation(SunNode src);
		Animation();

		auto Update() -> bool;
		auto Update(uint16_t timestep) -> bool;
		auto Reset() -> void;

		auto Draw(const DrawArgument& arguments, float alpha) const -> void;

		auto Get_Delay(int16_t frame) const->uint16_t;
		auto Get_Delay_Until(int16_t frame) const->uint16_t;
		auto Get_Origin() const->Point<int16_t>;
		auto Get_Dimensions() const->Point<int16_t>;
		auto Get_Head() const->Point<int16_t>;
		auto Get_Bounds() const->Rectangle<int16_t>;

	private:
		auto Get_Frame() const -> const Frame&;

		std::vector<Frame> frames_;
		bool animated_;
		bool zigzag_;

		Nominal<int16_t>frame_;
		Linear<float> opacity_;
		Linear<float> xy_scale_;

		uint16_t delay_;
		int16_t framestep_;
		float opacitystep_;
	};
}