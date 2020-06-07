#pragma once
#include "olcPixelGameEngine.h"
#include "Singleton.h"

// Window derives from PixelGameEngine as well as Singleton. Does this even make sense?
class Window : public olc::PixelGameEngine, public Singleton<Window>
{
public:
	Window();

	// Initializes all variables needed at the start.
	void init();
	// Called once at the start, so create things here.
	bool OnUserCreate();
	// Called every frame.
	bool OnUserUpdate(float fElapsedTime);
	// Show debug info (includes mouse pos).
	void DisplayDebugInfo(olc::vi2d &mouse);
	// Calls update() on all game classes.
	void update();
	// Calls draw() on all game classes.
	void draw();


private:
	// Window width.
	int16_t width;
	// Window height.
	int16_t height;

	bool fullscreen;
	// 2d int vector storing mouse position.
	olc::vi2d vimouse;
};