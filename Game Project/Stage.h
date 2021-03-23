#pragma once
#include "Singleton.h"
#include "MapBackgrounds.h"
#include "Window.h"
#include <cstdint>

namespace game
{
	class Stage : public Singleton<Stage>
	{
	public:
		Stage();

		// Calls Update() for all objects on stage.
		void Update();
		// Calls Draw() for all objects on stage.
		void Draw(/*float alpha(?)*/);
		// Loads the map to display.
		void Load(int32_t map_id);
		// Returns the current map ID.
		int32_t Get_Map_ID();

	private:
		// Actually load the map. Called by Load().
		// Loads in tiles, backgrounds, physics, mapinfo, portals.
		void Load_Map(int32_t map_id);

		// Collection of backgrounds on the map.
		MapBackgrounds backgrounds_;
		int32_t map_id_;
		/*
		Mobs object_;
		BGM object_;
		Tiles object_;
		Footholds object_;
		Effects object_;
		*/
	};
}