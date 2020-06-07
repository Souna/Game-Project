#include "Window.h"
#include "Constants.h"
#include "Template/Range.h"
#include "Stage.h"


Window::Window()
{
	sAppName = "THE GAME";
	width = Constants::get().get_viewwidth();
	height = Constants::get().get_viewheight();
	fullscreen = false;
}


void init()
{

}


bool Window::OnUserCreate()
{
	Stage::get().load(1);
	return true;
}


//Main loop
bool Window::OnUserUpdate(float fElapsedTime)
{
	vimouse = { GetMouseX(), GetMouseY() };

	Clear(olc::BLACK);

	update();

	draw();

	return true;
}


void Window::DisplayDebugInfo(olc::vi2d& mouse)
{
	DrawString({ 10,10 }, "Mouse: " + std::to_string((int)mouse.x) + ", " + std::to_string((int)mouse.y), olc::YELLOW, 2.0f);
}


void Window::update()
{
	Stage::get().update();
}


void Window::draw()
{
	Stage::get().draw();

	DisplayDebugInfo(vimouse);
}