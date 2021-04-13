#pragma once
#include <vector>

enum class Tilename {
	hidden,
	zero,
	one,
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	bomb,
	flag
};

enum class TileState
{
	hidden,
	revealed,
	flagged
};

 class Tile
{
public:
	 TileState state = TileState::hidden;
	 TileState prevState = TileState::hidden;
	 Tilename tile = Tilename::zero;
	 int tile_x = 0;
	 int tile_y = 0;

	 void reset()
	 {
		 state = TileState::hidden;
		 prevState = TileState::hidden;
		 tile = Tilename::zero;
	 }

};

 class TileMap
 {
	 int width, height, n_bombs;
	 std::vector<std::vector<Tile>> *tilesptr;
	 Tilename bombsAround(int i, int j);

 public:
	 TileMap(int width, int height, int n_bombs)
	 {
		 this->height = height;
		 this->width = width;
		 this->n_bombs = n_bombs;
	 }

	 void Generate(std::vector<int> *map = nullptr);

	 std::vector<std::vector<Tile>>* getTileMap();
	 ~TileMap()
	 {
		 //delete tiles;
	 }
 };

