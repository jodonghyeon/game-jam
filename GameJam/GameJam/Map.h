#pragma once
#include "GlobalConstants.h"
#include "Vector2.h"
#include <array>
#include <unordered_map>
#include <vector>
#include <memory>

struct Node
{
	Node(const int g_cost, const int h_cost, const IVector2& coord, const std::shared_ptr<Node> parent)
		: g_cost(g_cost), h_cost(h_cost), f_cost(g_cost + h_cost), coord(coord), parent(parent)
	{ }

	int			g_cost, h_cost, f_cost;
	IVector2	coord;
	std::shared_ptr<Node>		parent;
};

enum Tile {
	Vo,
	Wa,
	DO,
	DC,
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	TypeNum
};

constexpr std::array<std::array<Tile, NumTilesX>, NumTilesY>	DefaultMap =
{
	std::array<Tile, NumTilesX>
	{ Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa },
	{ Wa, R0, Wa, Vo, DO, Vo, Vo, Vo, Vo, Vo, DO, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, R1, Wa },
	{ Wa, Vo, Vo, Vo, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Vo, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Vo, Wa, Wa },
	{ Wa, Vo, Wa, Wa, Wa, Wa, Wa, R2, Wa, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Vo, R3, Wa, Vo, Wa, Vo, Wa, Wa },
	{ Wa, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Wa, Vo, Wa, Wa, Wa, DO, Wa, Wa, Wa, Vo, Wa, Vo, Vo, Wa },
	{ Wa, Vo, Wa, Vo, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Vo, Vo, Vo, Wa, Wa, Wa, DO, Wa, Wa, Vo, Wa },
	{ Wa, Vo, Wa, Vo, Wa, Wa, Wa, Vo, Vo, Vo, Vo, Vo, Wa, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Vo, Wa, DO, Wa },
	{ Wa, Vo, Wa, Vo, Wa, Wa, Vo, Vo, Wa, DO, Wa, Vo, Vo, Vo, Wa, Wa, Wa, Wa, Vo, Wa, Vo, Vo, Vo, Wa },
	{ Wa, Vo, Wa, Vo, Wa, Wa, Vo, Wa, Wa, Vo, Wa, Wa, DO, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Wa, Wa, Vo, Wa },
	{ Wa, Vo, Vo, Vo, Vo, Vo, Vo, Wa, Wa, Vo, Wa, Wa, Vo, Vo, Vo, Wa, R4, Wa, Vo, DO, Vo, Wa, Vo, Wa },
	{ Wa, DO, Wa, Wa, Wa, Wa, Vo, Wa, R5, Vo, Vo, Wa, Vo, Wa, Vo, Wa, Vo, Vo, Vo, Wa, Vo, Vo, Vo, Wa },
	{ Wa, Vo, Wa, Wa, Vo, DO, Vo, Wa, Wa, Wa, Vo, Wa, Vo, Wa, Vo, Wa, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Wa },
	{ Wa, Vo, Vo, Wa, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Wa, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Wa },
	{ Wa, Wa, Vo, Vo, Vo, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Vo, Wa, Vo, Wa },
	{ Wa, R6, Vo, Wa, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Vo, DO, Vo, Vo, Vo, Vo, Vo, Vo, Vo, Wa, R7, Wa },
	{ Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa, Wa }
};

struct Map {
	Map() {
		for (int y = 0; y < NumTilesY; ++y)
		{
			for (int x = 0; x < NumTilesX; ++x)
			{
				const Tile current_tile = current_map[y][x];
				if (R0 <= current_tile && current_tile < TypeNum)
				{
					room_coords[current_tile - R0].push_back({ x, y });
				}
				else if (current_tile == DO || current_tile == DC)
				{
					door_coords.push_back({ x, y });
				}
			}
		}

	}

	void	SwitchDoor(const IVector2& door_coord)
	{
		Tile& tile = current_map[door_coord.y][door_coord.x];
		if (tile == DO)
			tile = DC;
		else if (tile = DC)
			tile = DO;
	}

	std::array<std::array<Tile, NumTilesX>, NumTilesY>	current_map = DefaultMap;
	std::unordered_map<int, std::vector<IVector2>>		room_coords;
	std::vector<IVector2>								door_coords;
};
