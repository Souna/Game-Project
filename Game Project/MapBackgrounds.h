#pragma once
#include "olcPixelGameEngine.h"

// A single background image with all its properties.
// For now it's a decal.
class Background
{
public:
	Background(const char* path);

	// Updates background positions/animations.
	void update();
	// Draws background to screen using PGE functions.
	void draw();
private:
	// The actual background image in decal form.
	olc::Decal* dbackground;
	// Sprite object to turn into decal.
	olc::Sprite* sbackground;
};

// A collection of background objects (images) for use on maps.
class MapBackgrounds
{
public:
	MapBackgrounds(const char* path);
	MapBackgrounds();

	// Calls draw() function of backgrounds.
	void drawbackgrounds();
	// Calls draw() function of foregrounds.
	void drawforegrounds();

private:
	// Contains private vectors of backgrounds and foregrounds, of the background type.
	std::vector<Background> vbackgrounds;
	std::vector<Background> vforegrounds;
};