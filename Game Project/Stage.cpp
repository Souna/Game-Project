#include "Stage.h"



Stage::Stage()
{
}

void Stage::update()
{}

void Stage::draw()
{
	backgrounds.drawbackgrounds();
}

void Stage::load(int32_t mapid)
{
	load_map(mapid);
}

void Stage::load_map(int32_t mapid)
{
	Stage::mapid = mapid;
	backgrounds = MapBackgrounds("Assets/Back/grassySoil.img/back.1.png");
}