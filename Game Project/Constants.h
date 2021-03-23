#pragma once
#include "Singleton.h"
#include <cstdint>

namespace constants
{
	constexpr uint16_t TIMESTEP = 8;

	class Constants : public Singleton<Constants>
	{
	public:
		~Constants() {}

		Constants()
		{
			VIEWWIDTH = 1440;	// 1024
			VIEWHEIGHT = 720;	// 768
		}

		int16_t Get_View_Width()
		{
			return VIEWWIDTH;
		}

		void  Set_View_Width(int16_t width)
		{
			VIEWWIDTH = width;
		}

		int16_t Get_View_Height()
		{
			return VIEWHEIGHT;
		}

		void Set_View_Height(int16_t height)
		{
			VIEWHEIGHT = height;
		}

	private:
		int16_t VIEWWIDTH;
		int16_t VIEWHEIGHT;
	};
}