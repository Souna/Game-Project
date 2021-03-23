#include "Stage.h"
#include "SunFile.h"
#include "SunNode.h"
#include "SunBitmap.h"

namespace game
{
	Stage::Stage()
	{
	}

	void Stage::Update()
	{
		backgrounds_.Update();
	}

	void Stage::Draw()
	{
		//	Draw all backgrounds.
		backgrounds_.Draw_Backgrounds();
	}

	void Stage::Load(int32_t map_id)
	{
		Load_Map(map_id);
	}

	int32_t Stage::Get_Map_ID()
	{
		return map_id_;
	}

	void Stage::Load_Map(int32_t map_id)
	{
		Stage::map_id_ = map_id;

		SunNode src = sun_file::map["Map"]["Map" + std::to_string(map_id)];
		backgrounds_ = MapBackgrounds(src["back"]);//["Map.sun"]["Map"]["Map1"]["back"]
	}
}