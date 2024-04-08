#include "Stage.h"
#include "SunFile.h"
#include "SunNode.h"
#include "SunBitmap.h"
#include "Misc.h"

namespace game
{
	Stage::Stage()
	{
	}

	void Stage::Update()
	{
		/*/if (state != State::ACTIVE)
			return;*/
		backgrounds_.Update();
		
	}

	void Stage::Init()
	{

	}

	void Stage::Draw(float alpha)
	{
		/*if (state != State::ACTIVE)
			return;*/

		//Point<int16_t> viewpos = camera.position(alpha);
		//Point<double> viewrpos = camera.realposition(alpha);
		//double viewx = viewrpos.x();
		//double viewy = viewrpos.y();

		//	Draw all backgrounds.
		backgrounds_.Draw_Backgrounds(0, 0, alpha);
	}

	void Stage::Load(int32_t map_id, int8_t portal_id = 0)
	{
		state = State::INACTIVE;

		switch (state)
		{
		case State::INACTIVE:
			Load_Map(map_id);
			Respawn(portal_id);
			break;
		}
	}

	int32_t Stage::Get_Map_ID()
	{
		return map_id_;
	}

	void Stage::Load_Map(int32_t map_id)
	{
		// We will also load tiles, objects, physics, mapinfo, portals here
		Stage::map_id_ = map_id;
		std::string strId = string_format::Extend_Id(map_id, 9);
		std::string prefix = std::to_string(map_id / 100000000);

		SunNode mapImgNode = sun_file::Map["Map"]["Map" + prefix][strId + ".img"];

		backgrounds_ = MapBackgrounds(mapImgNode["back"]);
	}

	void Stage::Respawn(int8_t portal_id)
	{
		//Music(mapinfo.get_bgm()).play();
		//Point<int16_t> spawnpoint = portals.Get_Portal_By_Id(portal_id);
		//Point<int16_t> start_pos = physics_.Get_Y_Below(spawnpoint);
		//player_.Respawn(startpos, mapinfo.Is_Underwater());
		//camera_.set_position(startpos);
		//camera_.set_view(mapinfo.get_walls(), mapinfo.get_borders());
	}
}