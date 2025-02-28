#pragma once
#include "Map.h"
#include <string>

class NPC
{
public:
	NPC(const std::string name, const IVector2 start_coord, const std::unordered_map<int, std::vector<IVector2>>& room_coords);

	void	Update(const double dt, const Map& current_map);
	void	Draw() const;

	void	SetRandomDest();
	void	FindPath();

	bool	IsArrived();

	void	SetName(const std::string& new_name) { name = new_name; }

	std::string	GetName() const { return name; }
	Vector2		GetPosition() const { return position; }
	IVector2	GetDestCoord() const { return dest_coord; }
	std::array<std::array<Tile, NumTilesX>, NumTilesY>	GetMap() const { return NPC_map; }
	std::vector<IVector2>	GetPath() const { return path; }

	static constexpr float radius = 20.f;
	static constexpr int speed_min = 15;
	static constexpr int speed_max = 30;
private:
	std::string				name;
	std::array<std::array<Tile, NumTilesX>, NumTilesY>	NPC_map = DefaultMap;
	Vector2					position;
	const std::unordered_map<int, std::vector<IVector2>>& room_coords;
	IVector2				dest_coord;
	std::vector<IVector2>	path;
	float	speed;
};