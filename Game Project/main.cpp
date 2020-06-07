#define OLC_PGE_APPLICATION
#include "Window.h"
#include "Constants.h"

int main()
{
	if (Window::get().Construct(Constants::get().get_viewwidth(), Constants::get().get_viewheight(), 1, 1))
		Window::get().Start();

	return 0;
}