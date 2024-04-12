#pragma once
//#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Singleton.h"
#include "Error.h"
#include "SunBitmap.h"

class Window : public olc::PixelGameEngine, public Singleton<Window>
{
public:
	Window();

	// Initializes all variables needed at the start.
	Error Init();
	// Called once at the start, so create things here.
	bool OnUserCreate();
	// Called every frame.
	bool OnUserUpdate(float fElapsedTime);
	// Show debug info (includes mouse pos).
	void DisplayDebugInfo(olc::vi2d& mouse, float alpha);
	// Calls Update() on all game classes.
	void Update();
	// Calls Draw() on all game classes.
	void Draw(float alpha);

private:
	// Window width.
	int16_t width_;
	// Window height.
	int16_t height_;

	bool fullscreen_;
	// 2d int vector storing mouse position.
	olc::vi2d vimouse_;
};