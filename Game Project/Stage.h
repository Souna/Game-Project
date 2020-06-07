#pragma once
#include "Singleton.h"
#include "MapBackgrounds.h"
#include <cstdint>

class Stage : public Singleton<Stage>
{
public:
	Stage();

	// Calls update() of all objects on stage.
	void update();
	// Calls draw() of all objects on stage.
	void draw(/*float alpha(?)*/);
	// Loads the map to display.
	void load(int32_t mapid);

private:
	// Actually load the map. Called by load().
	// Loads in tiles, backgrounds, physics, mapinfo, portals.
	void load_map(int32_t mapid);

	// Collection of backgrounds on the map.
	MapBackgrounds backgrounds;
	int32_t mapid;
	/*
	Mobs object;
	BGM object;
	Tiles object;
	Footholds object;
	Effects object;
	*/
};