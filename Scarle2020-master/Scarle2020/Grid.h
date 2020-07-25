#pragma once
#include "Tile.h"
#include <vector>

class Grid
{
public:
	Grid(ID3D11Device* _GD);
	int generateAltitude(int x);
	~Grid() = default;

	Tile* getTile(float x, float y);
	Tile* getTile(const Vector2& grid_coords);
	Vector2 findGridCoords(Vector2 screen_coords);
	static Vector2 findWorldCoords(const Vector2& tile_coords);
	Vector2 findGridCoords(const Vector2& world_coords);

	void draw(DrawData2D* _DD);

	static constexpr int GRID_HEIGHT = 24;
	static constexpr int GRID_WIDTH = 43;
private:
	// I am aware a 2D vector is not optimal, however it allows for
	// simpler code at the cost of memory access optimization
	std::vector<std::vector<Tile>> tiles;
};

