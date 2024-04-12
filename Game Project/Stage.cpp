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

		Point<int16_t> view_pos = camera_.Position(alpha);
		Point<double> view_real_pos = camera_.Real_Position(alpha);
		double view_x = view_real_pos.X();
		double view_y = view_real_pos.Y();

		//	Draw all backgrounds.
		backgrounds_.Draw_Backgrounds(view_x, view_y, alpha);

		//for (auto id : Layer::IDs)
		//{
		//	tilesobjs.draw(id, viewpos, alpha);
		//	reactors.draw(id, viewx, viewy, alpha);
		//	npcs.draw(id, viewx, viewy, alpha);
		//	mobs.draw(id, viewx, viewy, alpha);
		//	chars.draw(id, viewx, viewy, alpha);
		//	player.draw(id, viewx, viewy, alpha);
		//	drops.draw(id, viewx, viewy, alpha);
		//}
	}

	void Stage::Load(int32_t map_id, int8_t portal_id = 0)
	{
		state_ = State::INACTIVE;

		switch (state_)
		{
		case State::INACTIVE:
			Load_Map(map_id);
			Respawn(portal_id);
			break;
		case State::TRANSITION:
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
		physics_ = Physics(mapImgNode["foothold"]);
		map_info_ = MapInfo(mapImgNode, physics_.Get_FootholdTree().Get_Walls(), physics_.Get_FootholdTree().Get_Borders());
	}

	void Stage::Respawn(int8_t portal_id)
	{
		//Music(mapinfo.get_bgm()).play();
		//Point<int16_t> spawnpoint = portals.Get_Portal_By_Id(portal_id);
		Point<int16_t> spawn_point = { 0, 0 };
		//Point<int16_t> start_position = physics_.Get_Y_Below(spawn_point);
		Point<int16_t> start_position = { 0, 0 };
		//player_.Respawn(startpos, mapinfo.Is_Underwater());
		camera_.Set_Position(start_position);
		camera_.Set_View(map_info_.Get_Walls(), map_info_.Get_Borders());
	}
}