#pragma once
#include <string>
#include "Tile.hpp"

class Board : public sf::Drawable, public sf::Transformable
{

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	unsigned int width, height;
	sf::Vector2u tileSize;
	std::vector<std::vector<Tile>> *tilesptr;

public:
	bool load(const std::string& tileset);
	void changeTile(int x, int y, Tilename tile);
	void processLeftClick(int ypos, int xpos);
	void processRightClick(int ypos, int xpos);

	Board(sf::Vector2u tSize, unsigned int width, unsigned int height, std::vector<std::vector<Tile>> *tilesptr)
	{
		this->width = width;
		this->height = height;
		tileSize.x = tSize.x;
		tileSize.y = tSize.y;
		this->tilesptr = tilesptr;

	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void revealTile(int x, int y);
	void revealiteratively(int x, int y);
};