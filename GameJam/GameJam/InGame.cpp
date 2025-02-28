#include "InGame.h"

InGame::InGame()
{ }

void InGame::Load()
{
	const int	dp_index = GetRandomValue(0, TypeNum - R0 - 1);
	int j = 0;
	for (int i = 0; i <= TypeNum - R0 - 1; ++i)
	{
		const IVector2		start_coord = (map.room_coords.at(i))[0];
		if (i != dp_index)
		{
			NPCs.emplace_back(names[j], start_coord, map.room_coords);
			++j;
		}
	}
	NPCs.emplace_back(names[GetRandomValue(0, names.size() - 1)], (map.room_coords.at(dp_index))[0], map.room_coords);
}

void InGame::UnLoad()
{
	current_time = 0.0;
	NPCs.clear();
	map = Map{};
	opened_profile = -1;
}

void InGame::Update(const double dt)
{
	current_time += dt;
	for (int i = 0; i < NPCs.size(); ++i)
	{
		if (NPCs[i].IsArrived() && i > 6)
			NPCs[i].SetName(names[GetRandomValue(0, names.size() - 1)]);
		NPCs[i].Update(dt, map);
	}
	if (current_time <= 80.0 && static_cast<int>(current_time) / 20 > NPCs.size() - TypeNum + R0)
	{
		NPCs.emplace_back(names[GetRandomValue(0, names.size() - 1)], (map.room_coords.at(GetRandomValue(0, TypeNum - R0 - 1)))[0], map.room_coords);
	}
}

void InGame::Draw()
{
	for (int y = 0; y < NumTilesY; ++y)
	{
		for (int x = 0; x < NumTilesX; ++x)
		{
			Color	tile_color;
			switch (map.current_map[y][x])
			{
			case Vo:
				tile_color = WHITE;
				break;
			case Wa:
				tile_color = BLACK;
				break;
			case DO:
				tile_color = LIGHTGRAY;
				break;
			case DC:
				tile_color = DARKGRAY;
				break;
			case R0:
				tile_color = GOLD;
				break;
			case R1:
				tile_color = PINK;
				break;
			case R2:
				tile_color = MAROON;
				break;
			case R3:
				tile_color = GREEN;
				break;
			case R4:
				tile_color = DARKGREEN;
				break;
			case R5:
				tile_color = SKYBLUE;
				break;
			case R6:
				tile_color = DARKBLUE;
				break;
			case R7:
				tile_color = VIOLET;
				break;
			}
			DrawRectangle(TileWidth * x, TileWidth * y, TileWidth, TileWidth, tile_color);
		}
	}

	for (const NPC& npc : NPCs)
	{
		npc.Draw();
	}

	DoorsUpdate();

	if (opened_profile != -1)
	{
		const NPC& npc = NPCs[opened_profile];
		
		DrawRectangle(0, 0, WindowWidth, WindowHeight, { 255, 255, 255, 120 });
		for (int i = 0; i < map.door_coords.size(); ++i)
		{
			if (npc.GetMap()[map.door_coords[i].y][map.door_coords[i].x] == DC)
				DrawRectangle(TileWidth * map.door_coords[i].x, TileWidth * map.door_coords[i].y, TileWidth, TileWidth, { 0, 0, 0, 200 });
			else if (npc.GetMap()[map.door_coords[i].y][map.door_coords[i].x] == DO)
				DrawRectangle(TileWidth * map.door_coords[i].x, TileWidth * map.door_coords[i].y, TileWidth, TileWidth, { 255, 255, 255, 200 });
		}

		Color	tile_color;
		switch (map.current_map[npc.GetDestCoord().y][npc.GetDestCoord().x])
		{
		case R0:
			tile_color = { 255, 203, 0, 200 };
			break;
		case R1:
			tile_color = { 255, 109, 194, 200 };
			break;
		case R2:
			tile_color = { 190, 33, 55, 200 };
			break;
		case R3:
			tile_color = { 0, 228, 48, 200 };
			break;
		case R4:
			tile_color = { 0, 117, 44, 200 };
			break;
		case R5:
			tile_color = { 102, 191, 255, 200 };
			break;
		case R6:
			tile_color = { 0, 82, 172, 200 };
			break;
		case R7:
			tile_color = { 135, 60, 190, 200 };
			break;
		}

		const std::vector<IVector2>& NPC_path = npc.GetPath();
		for (const IVector2& path_coord : NPC_path)
		{
			DrawRectangle(TileWidth * path_coord.x, TileWidth * path_coord.y, TileWidth, TileWidth, tile_color);
		}
		
		DrawText(npc.GetName().c_str(), (WindowWidth - 3 * profile_font_size) / 2, (WindowWidth - 8 * profile_font_size) / 2, profile_font_size, BLACK);

		if (opened_profile > 6)
		{
			DrawRectangle((WindowWidth - (opened_profile - 5) * profile_font_size) / 2 + profile_font_size * 2, (WindowWidth - 8 * profile_font_size) / 2, profile_font_size, profile_font_size, BLACK);
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			opened_profile = -1;
	}
	else if (ClickedNPC() != -1)
	{
		opened_profile = ClickedNPC();
	}

}

int InGame::ClickedNPC() const
{
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return -1;
	const Vector2	mouse_position = GetMousePosition();
	int	NPC_index = -1;

	for (int i = 0; i < NPCs.size(); ++i)
	{
		const NPC& npc = NPCs[i];
		if (abs(mouse_position.x - npc.GetPosition().x) < NPC::radius &&
			abs(mouse_position.y - npc.GetPosition().y) < NPC::radius)
		{
			NPC_index = i;
		}
	}
	return NPC_index;
}

void InGame::DoorsUpdate()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const Vector2	mouse_position = GetMousePosition();
		for (const IVector2& door_coord : map.door_coords)
		{
			if (static_cast<int>(mouse_position.x) / TileWidth == door_coord.x &&
				static_cast<int>(mouse_position.y) / TileWidth == door_coord.y)
			{
				for (const NPC& npc : NPCs)
				{
					const Vector2	npc_position = npc.GetPosition();
					if (npc_position.x + TileWidth / 2 > TileWidth * door_coord.x &&
						npc_position.x - TileWidth / 2 < TileWidth * (door_coord.x + 1) &&
						npc_position.y + TileWidth / 2 > TileWidth * door_coord.y &&
						npc_position.y - TileWidth / 2 < TileWidth * (door_coord.y + 1))
					{
						return;
					}
				}
				map.SwitchDoor(door_coord);
			}
		}
	}
}

bool InGame::IsGameOver() const
{
	for (int i = 7; i < NPCs.size(); ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			if (NPCs[i].GetName() == NPCs[j].GetName() &&
				abs(NPCs[i].GetPosition().x - NPCs[j].GetPosition().x) < 2 * NPC::radius &&
				abs(NPCs[i].GetPosition().y - NPCs[j].GetPosition().y) < 2 * NPC::radius)
				return true;
		}
	}
	return false;
}
