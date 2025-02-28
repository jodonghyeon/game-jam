#pragma once
#include "GlobalConstants.h"
#include <raylib.h>

class MainMenu
{
public:
	void	Draw() const {
		DrawCircle(0, WindowHeight / 2, WindowHeight / 2, BLACK);
		DrawCircle(WindowWidth, WindowHeight / 2, WindowHeight / 2, BLACK);
		DrawText("Doppleganger", (WindowWidth - 18 * basic_font_size) / 2, (WindowHeight - 9 * basic_font_size) / 2, basic_font_size * 3, GRAY);
		DrawText("Mouse press to start", (WindowWidth - 20 * basic_font_size) / 2, (WindowHeight - 4 * basic_font_size) / 2, basic_font_size * 2, GRAY);
	}

	bool	IsGameStart() const {
		return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	}

	static constexpr int	basic_font_size = 20;
};