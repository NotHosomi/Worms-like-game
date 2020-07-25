#include "pch.h"
#include "Grid.h"

Grid::Grid(ID3D11Device* _GD)
{
	// generate terrain
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		// generate altitude
		int altitude = 2 * sin(0.4 * i);
		altitude -= 15;
		// spawn tiles
		for (int j = 0; j < altitude; ++j)
		{
			tiles[i][j] = Tile(_GD, Vector2(i, j));
			tiles[i][j].kill();
		}
		for (int j = 0; j < altitude; ++j)
		{
			tiles[i][j] = Tile(_GD, Vector2(i, j));
		}
	}
}

void Grid::draw(DrawData2D* _DD)
{
	for (auto& column : tiles)
	{
		for (auto& tile : column)
		{
			tile.draw(_DD);
		}
	}
}

Tile* Grid::getTile(float x, float y)
{
	int x_coord = static_cast<int>(std::floor(x));
	int y_coord = static_cast<int>(std::floor(y));
	return &tiles[x_coord][x_coord];
}

Tile* Grid::getTile(const Vector2& grid_coords)
{
	int x_coord = static_cast<int>(std::floor(grid_coords.x));
	int y_coord = static_cast<int>(std::floor(grid_coords.y));
	return &tiles[x_coord][x_coord];
}

Vector2 Grid::findGridCoords(Vector2 screen_coords)
{
	Vector2 grid_coords;
	grid_coords.x = screen_coords.x / Tile::TILE_DIMS;
	grid_coords.y = screen_coords.y / Tile::TILE_DIMS;
	grid_coords.x = std::floor(grid_coords.x);
	grid_coords.y = std::floor(grid_coords.y);
	return grid_coords;
}

Vector2 Grid::findWorldCoords(const Vector2& tile_coords)
{
	Vector2 world_coords;
	world_coords.x = tile_coords.x * Tile::TILE_DIMS;
	world_coords.y = tile_coords.y * Tile::TILE_DIMS;
	return world_coords;
}

Vector2 Grid::findGridCoords(const Vector2& world_coords)
{
	Vector2 out = world_coords;
	out *= (1 / Tile::TILE_DIMS);
	return out;
}
