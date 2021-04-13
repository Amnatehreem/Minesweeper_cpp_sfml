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
	 Tilename tile = Tilename::zero;
	 int tile_x = 0;
	 int tile_y = 0;

};

 class TileMap
 {
	 int width, height;
	 std::vector<std::vector<Tile>> *tilesptr;
	 Tilename bombsAround(int i, int j);

 public:
	 TileMap(int width, int height, int mines);
	 std::vector<std::vector<Tile>>* getTileMap();
	 ~TileMap()
	 {
		 //delete tiles;
	 }

 };

