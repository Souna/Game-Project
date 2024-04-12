#define OLC_PGE_APPLICATION
#define PRINT_WARNINGS
#include "Window.h"
#include "Constants.h"

int main()
{
	if (Window::Get().Construct(
		constants::Constants::Get().Get_View_Width(),
		constants::Constants::Get().Get_View_Height(),
		constants::Constants::Get().Get_Scale_X(),
		constants::Constants::Get().Get_Scale_Y())
		)
		Window::Get().Start();

	return 0;
}