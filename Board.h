#pragma once
#include <string>
#include "Tile.hpp"
#include "Buttons.h"

class Board : public sf::Drawable, public sf::Transformable
{

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	unsigned int width, height;
	sf::Vector2u tileSize;
	std::vector<std::vector<Tile>> *tilesptr;
	bool debugmode = false;
	Buttons *buttons;
	unsigned int non_bomb_tiles, revealed_tiles = 0;
	bool playing = true;   // on win/lose, playing becomes false and left click will have no effect

public:
	bool load(const std::string& tileset);
	void changeTile(int x, int y, Tilename tile);
	void processLeftClick(int ypos, int xpos);
	void processRightClick(int ypos, int xpos);
	void Reset(int bombs);
	void ToggleDebugMode();

	Board(sf::Vector2u tSize, unsigned int width, unsigned int height, int bombs, std::vector<std::vector<Tile>> *tilesptr, Buttons *buttons)
	{
		this->width = width;
		this->height = height;
		tileSize.x = tSize.x;
		tileSize.y = tSize.y;
		this->tilesptr = tilesptr;
		this->buttons = buttons;
		non_bomb_tiles = width * height - bombs;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void revealTile(int x, int y);
	void hideTile(int x, int y);
	void revealiteratively(int x, int y);
};