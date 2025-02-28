#include "GameStates.h"
#include "MainMenu.h"
#include "InGame.h"

int	main()
{
	InitWindow(WindowWidth, WindowHeight, WindowTitle);

	double	previous_time = GetTime();

	GameState	gamestate = GameState::MainMenu;

	MainMenu	main_menu;
	InGame		in_game;
	while (!WindowShouldClose())
	{
		const double	current_time = GetTime();
		const double	dt = current_time - previous_time;

		BeginDrawing();
		ClearBackground(WHITE);
		switch (gamestate)
		{
		case GameState::MainMenu:
			if (dt >= 1.0 / TargetFPS)
			{
				previous_time = current_time;
			}
			main_menu.Draw();
			if (main_menu.IsGameStart())
			{
				in_game.Load();
				gamestate = GameState::InGame;
			}
			break;
		case GameState::InGame:
			if (dt >= 1.0 / TargetFPS)
			{
				in_game.Update(dt);
				previous_time = current_time;
			}
			in_game.Draw();
			if (in_game.IsGameOver())
			{
				in_game.UnLoad();
				gamestate = GameState::MainMenu;
			}
			break;
		}

		EndDrawing();
	}
	CloseWindow();
	return 0;
}