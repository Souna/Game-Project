#define OLC_PGE_APPLICATION
#define PRINT_WARNINGS
#include "Window.h"
#include "Constants.h"

int main()
{
	if (Window::Get().Construct(constants::Constants::Get().Get_View_Width(), constants::Constants::Get().Get_View_Height(), 1, 1))
		Window::Get().Start();

	return 0;
}