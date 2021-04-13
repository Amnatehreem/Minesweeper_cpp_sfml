#pragma once
#include "Tile.hpp"

enum class LeftClickAction
{
	LoadTest1,
	LoadTest2,
	LoadTest3,
	ResetGame,
	ToggleDebug,
	NoAction
};

class Buttons : public sf::Drawable, public sf::Transformable
{

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	unsigned int width, height_offset;
	unsigned int face_offset = 0;
	unsigned int debug_offset = 0;
	unsigned int test_offset = 0;
	//std::vector<std::vector<Tile>> *tilesptr;

public:
	bool load(const std::string& tileset);
	void changeTile(int x, int y, Tilename tile);
	LeftClickAction processLeftClick(int ypos, int xpos);

	Buttons(unsigned int width, unsigned int _height_offset)
	{
		this->width = width;
		height_offset = _height_offset;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

