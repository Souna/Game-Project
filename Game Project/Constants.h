#pragma once
#include "Singleton.h"
#include <cstdint>

namespace constants
{
	constexpr uint16_t TIMESTEP = 2;
	constexpr uint8_t NUM_FILES = 2;

	class Constants : public Singleton<Constants>
	{
	public:
		~Constants() {}

		Constants()
		{
			VIEWWIDTH = 1366;
			VIEWHEIGHT = 768;
			SCALEX = 1;
			SCALEY = 1;
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

		int16_t Get_Scale_X()
		{
			return SCALEX;
		}

		void Set_Scale_X(int16_t scale)
		{
			SCALEX = scale;
		}

		int16_t Get_Scale_Y()
		{
			return SCALEY;
		}

		void Set_Scale_Y(int16_t scale)
		{
			SCALEY = scale;
		}

	private:
		int16_t VIEWWIDTH;
		int16_t VIEWHEIGHT;
		int16_t SCALEX;
		int16_t SCALEY;

	};
}