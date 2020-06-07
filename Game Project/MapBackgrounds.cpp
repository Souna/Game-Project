#include "MapBackgrounds.h"
#include "Window.h"

Background::Background(const char* path)
{
	// Construct the background decal object by first loading
	// it in as a sprite, then making that sprite into a decal.
	sbackground = new olc::Sprite(path);
	dbackground = new olc::Decal(sbackground);
}

void Background::update()
{}

void Background::draw()
{
	//olc::PixelGameEngine::DrawDecal({ 0,0 }, dbackground);
	Window::get().DrawDecal({ 0,0 }, dbackground);
}

//=======================================================================

MapBackgrounds::MapBackgrounds(const char* path) //does this need to be const char*
{

	vbackgrounds.push_back(path);
}

MapBackgrounds::MapBackgrounds()
{}

void MapBackgrounds::drawbackgrounds()
{
	for (auto& background : vbackgrounds)
	{
		background.draw();
	}
}

void MapBackgrounds::drawforegrounds()
{
	for (auto& foreground : vforegrounds)
	{
		foreground.draw();
	}
}
