#pragma once
#include "Singleton.h"
#include "MapBackgrounds.h"
#include "Window.h"
#include <cstdint>
#include "Physics.h"
#include "Camera.h"
#include "MapInfo.h"

namespace game
{
	class Stage : public Singleton<Stage>
	{
	public:
		Stage();

		void Init();

		// Calls Update() for all objects on stage.
		void Update();
		// Calls Draw() for all objects on stage.
		void Draw(float alpha);
		// Loads the map to display.
		void Load(int32_t map_id, int8_t portal_id);
		// Returns the current map ID.
		int32_t Get_Map_ID();

	private:
		// Actually load the map. Called by Load().
		// Loads in tiles, backgrounds, physics, mapinfo, portals.
		void Load_Map(int32_t map_id);
		void Respawn(int8_t portal_id);

		enum State
		{
			INACTIVE,
			TRANSITION,
			ACTIVE
		};

		Camera camera_;
		Physics physics_;
		//Player player_;

		//Optional<Playable> playable_;
		int32_t map_id_;
		State state_;

		MapInfo map_info_;
		MapBackgrounds backgrounds_;
		
	};
}