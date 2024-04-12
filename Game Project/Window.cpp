#include "Window.h"
#include "Constants.h"
#include "Range.h"
#include "Stage.h"
#include "SunFile.h"

Window::Window() : fullscreen_(false)
{
	sAppName = "THE GAME";
	width_ = constants::Constants::Get().Get_View_Width();
	height_ = constants::Constants::Get().Get_View_Height();
};

Error Window::Init()
{
	// Initialize .sun files.
	if (Error error = sun_file::Init())
		return error;

	game::Stage::Get().Init();

	return Error::NONE;
}

bool Window::OnUserCreate()
{
	// Run initialize and display any errors.
	if (Error error = Init())
	{
		const char* message = error.get_message();
		const char* args = error.get_args();
		bool can_retry = error.can_retry();

		std::cout << "Error: " << message << std::endl;

		if (args && args[0])
			std::cout << "Message: " << args << std::endl;

		if (can_retry)
			std::cout << "Enter 'r' to retry." << std::endl;

		std::string command;
		std::cin >> command;

		if (can_retry && command == "r")
			OnUserCreate();
	}
	// No init errors, continue.
	// Load map 0 for testing.
	game::Stage::Get().Load(0, 0);
	SetPixelMode(olc::Pixel::ALPHA);
	return true;
}

//Main loop
bool Window::OnUserUpdate(float fElapsedTime)
{
	vimouse_ = { GetMouseX(), GetMouseY() };
	float alpha = static_cast<float>(fElapsedTime) / constants::TIMESTEP;

	//Default blank background color
	Clear(olc::BLUE);

	Update();

	Draw(alpha);

	return true;
}

void Window::DisplayDebugInfo(olc::vi2d& mouse, float alpha)
{
	DrawString({ 10,10 }, "Mouse Physical: " + std::to_string((int)mouse.x) + ", " + std::to_string((int)mouse.y), olc::RED, 2.0f);
	DrawString({ 10,30 }, "Map ID: " + std::to_string(game::Stage::Get().Get_Map_ID()), olc::YELLOW, 2.0f);
	DrawString({ 10,50 }, "Alpha Value: " + std::to_string(alpha), olc::WHITE, 2.0f);

}

void Window::Update()
{
	game::Stage::Get().Update();
}

void Window::Draw(float alpha)
{
	game::Stage::Get().Draw(alpha);
	//FillTriangle({ vimouse_.x-100, vimouse_.y }, { vimouse_.x + 100, vimouse_.y }, { vimouse_.x, vimouse_.y + 100 }, olc::CYAN);
	DisplayDebugInfo(vimouse_, alpha);
}