#include "NPC.h"
#include <algorithm>

NPC::NPC(const std::string name, const IVector2 start_coord, const std::unordered_map<int, std::vector<IVector2>>& room_coords)
	: name(name),
	position({	static_cast<float>(TileWidth * start_coord.x + TileWidth / 2), 
				static_cast<float>(TileWidth * start_coord.y + TileWidth / 2) }),
	room_coords(room_coords)
{ 
	SetRandomDest();
	FindPath();
	speed = static_cast<float>(GetRandomValue(speed_min, speed_max));
}

void NPC::Update(const double dt, const Map& current_map)
{
	if (IsArrived() || path.size() == 0)
	{
		SetRandomDest();
		path.clear();
		FindPath();
		speed = static_cast<float>(GetRandomValue(speed_min, speed_max));
	}
	for (const IVector2& door_coord : current_map.door_coords)
	{
		if (NPC_map[door_coord.y][door_coord.x] == DO &&
			current_map.current_map[door_coord.y][door_coord.x] == DC &&
			position.x + TileWidth / 2 >= TileWidth * door_coord.x &&
			position.x - TileWidth / 2 <= TileWidth * (door_coord.x + 1) &&
			position.y + TileWidth / 2 >= TileWidth * door_coord.y &&
			position.y - TileWidth / 2 <= TileWidth * (door_coord.y + 1))
		{
			position.x = TileWidth * (static_cast<int>(position.x) / TileWidth) + TileWidth / 2;
			position.y = TileWidth * (static_cast<int>(position.y) / TileWidth) + TileWidth / 2;
			NPC_map[door_coord.y][door_coord.x] = DC;
			path.clear();
			FindPath();
			break;
		}
		else if (NPC_map[door_coord.y][door_coord.x] == DC &&
			current_map.current_map[door_coord.y][door_coord.x] == DO &&
			position.x + TileWidth / 2 >= TileWidth * door_coord.x &&
			position.x - TileWidth / 2 <= TileWidth * (door_coord.x + 1) &&
			position.y + TileWidth / 2 >= TileWidth * door_coord.y &&
			position.y - TileWidth / 2 <= TileWidth * (door_coord.y + 1))
		{
			NPC_map[door_coord.y][door_coord.x] = DO;
			path.clear();
			FindPath();
			break;
		}
	}

	const Vector2	semi_dest_pos = { TileWidth * path[path.size() - 1].x + TileWidth / 2, TileWidth * path[path.size() - 1].y + TileWidth / 2 };

	if (position.x < semi_dest_pos.x)
		position.x += speed * dt;
	else if (position.x > semi_dest_pos.x)
		position.x -= speed * dt;
	if (position.y < semi_dest_pos.y)
		position.y += speed * dt;
	else if (position.y > semi_dest_pos.y)
		position.y -= speed * dt;

	if (abs(position.x - semi_dest_pos.x) <= radius / 15.f)
	{
		position.x = semi_dest_pos.x;
	}
	if (abs(position.y - semi_dest_pos.y) <= radius / 15.f)
	{
		position.y = semi_dest_pos.y;
	}
	if (abs(position.x - semi_dest_pos.x) <= radius / 15.f && abs(position.y - semi_dest_pos.y) <= radius / 15.f)
	{
		path.pop_back();
	}
}

void NPC::Draw() const
{
	DrawCircleV(position, radius, BLACK);
}

void NPC::SetRandomDest()
{
	const int	room_num = GetRandomValue(0, TypeNum - R0 - 1);
	dest_coord = room_coords.at(room_num)[0];
}

void NPC::FindPath()
{
	std::list<std::shared_ptr<Node>>	open;
	std::list<std::shared_ptr<Node>>	closed;

	const IVector2	start_coord = { (static_cast<int>(position.x) - TileWidth / 2) / TileWidth, (static_cast<int>(position.y) - TileWidth / 2) / TileWidth };
	open.push_back(std::make_shared<Node>(0, mht_dist(start_coord, dest_coord), start_coord, nullptr));

	std::shared_ptr<Node> current_node;
	while (true)
	{
		current_node = *open.begin();
		for (std::shared_ptr<Node> node : open)
		{
			if (node->f_cost < current_node->f_cost || 
				(node->f_cost == current_node->f_cost && node->h_cost < current_node->h_cost))
				current_node = node;
		}
		open.remove(current_node);
		closed.push_back(current_node);

		if (current_node->coord == dest_coord)
		{
			while (current_node->parent != nullptr)
			{
				path.push_back(current_node->coord);
				current_node = current_node->parent;
			}
			path.push_back(current_node->coord);

			return;
		}

		const IVector2	current_coord = current_node->coord;
		IVector2		nb_coord;
		for (int i = 0; i < 4; ++i)
		{
			switch (i)
			{
			case 0:
				nb_coord = { current_node->coord.x, current_coord.y + 1 };
				break;
			case 1:
				nb_coord = { current_node->coord.x - 1, current_coord.y };
				break;
			case 2:
				nb_coord = { current_node->coord.x + 1, current_coord.y };
				break;
			case 3:
				nb_coord = { current_node->coord.x, current_coord.y - 1 };
				break;
			}

			if (nb_coord.x < 0 || nb_coord.x >= NumTilesX || nb_coord.y < 0 || nb_coord.y >= NumTilesY ||
				NPC_map[nb_coord.y][nb_coord.x] == Wa ||
				NPC_map[nb_coord.y][nb_coord.x] == DC ||
				std::any_of(closed.begin(), closed.end(),
					[&](const std::shared_ptr<Node>& node)
					{ return node->coord == nb_coord; }))
				continue;

			std::list<std::shared_ptr<Node>>::iterator it = 
				std::find_if(open.begin(), open.end(),
				[&](const std::shared_ptr<Node>& node)
				{ return node->coord == nb_coord; });
			const int	new_g_cost = current_node->g_cost + 1;
			const int	new_h_cost = mht_dist(nb_coord, dest_coord);
			const int	new_f_cost = new_g_cost + new_h_cost;
			if (it == open.end())
			{
				open.push_back(std::make_shared<Node>(current_node->g_cost + 1, new_h_cost, nb_coord, current_node));
			}
			else if (new_f_cost < (*it)->f_cost)
			{
				(*it)->g_cost = new_g_cost;
				(*it)->f_cost = new_f_cost;
				(*it)->parent = current_node;
			}
		}
	}

}

bool NPC::IsArrived()
{
	if (abs(position.x - dest_coord.x) <= radius / 8.f && abs(position.y - dest_coord.y) <= radius / 8.f)
		return true;
	return false;
}

