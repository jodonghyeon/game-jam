#pragma once
#include "NPC.h"

class InGame
{
public:
	InGame();

	void	Load();
	void	UnLoad();

	void	Update(const double	dt);
	void	Draw();

	int		ClickedNPC() const;
	void	DoorsUpdate();

	bool	IsGameOver() const;

	static constexpr int	profile_font_size = 50;
private:
	double				current_time = 0.0;
	std::vector<NPC>	NPCs = {};
	const std::vector<std::string>	names =
	{
		"Daniel",
		"Thomas",
		"Samuel",
		"Rachel",
		"Robert",
		"Sophie",
		"Donald"
	};
	Map	map{};
	int	opened_profile = -1;
};
