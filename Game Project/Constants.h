#pragma once
#include "Singleton.h"
#include <cstdint>

class Constants : public Singleton<Constants>
{
public:
	~Constants(){}

	Constants()
	{
		VIEWWIDTH = 1024;	// 1024
		VIEWHEIGHT = 768;	// 768
	}

	int16_t get_viewwidth()
	{
		return VIEWWIDTH;
	}

	void  set_viewwidth(int16_t width)
	{
		VIEWWIDTH = width;
	}

	int16_t get_viewheight()
	{
		return VIEWHEIGHT; 
	}

	void set_viewheight(int16_t height)
	{
		VIEWHEIGHT = height;
	}

private:
	int16_t VIEWWIDTH;
	int16_t VIEWHEIGHT;
};