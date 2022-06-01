#include "Window.h"
#include "Constants.h"
#include "Range.h"
#include "Stage.h"
#include "SunFile.h"

Window::Window()
{
	sAppName = "THE GAME";
	width_ = constants::Constants::Get().Get_View_Width();
	height_ = constants::Constants::Get().Get_View_Height();
	fullscreen_ = false;
}

Error Window::Init()
{
	// Initialize .sun files.
	if (Error error = sun_file::Init())
		return error;

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

	game::Stage::Get().Load(1);	//loads 1.img. Temporary test map ID.

	return true;
}

//Main loop
bool Window::OnUserUpdate(float fElapsedTime)
{
	vimouse_ = { GetMouseX(), GetMouseY() };

	Clear(olc::BLUE);

	Update();

	Draw();

	return true;
}

void Window::DisplayDebugInfo(olc::vi2d& mouse)
{
	DrawString({ 10,10 }, "Mouse Physical: " + std::to_string((int)mouse.x) + ", " + std::to_string((int)mouse.y), olc::RED, 2.0f);
	//DrawString({ 10,30 }, "Map ID: " + std::to_string(game::Stage::Get().Get_Map_ID()), olc::YELLOW, 2.0f);
}

void Window::Update()
{
	game::Stage::Get().Update();
}

void Window::Draw()
{
	//DrawString({})
	game::Stage::Get().Draw();
	DisplayDebugInfo(vimouse_);
}