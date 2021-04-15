#include <SFML/Graphics.hpp>
#include "Buttons.h"
#include "Board.h"
#include <iostream>

using namespace std;
#define tiles (*tilesptr)

bool Board::load(const std::string& tileset)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	Reset(height*width - non_bomb_tiles);
	return true;
}

void Board::Reset(int bombs)
{
	non_bomb_tiles = height * width - bombs;
	playing = true;
	revealed_tiles = 0;
	debugmode = false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(0, 0 + 16);
			quad[1].texCoords = sf::Vector2f(tileSize.x, 0 + 16);
			quad[2].texCoords = sf::Vector2f(tileSize.x, tileSize.y + 16);
			quad[3].texCoords = sf::Vector2f(0, tileSize.y + 16);
		}
}

void  Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

// Process left click: reveal a tile. reveal neighboring tiles that are not bombs
// x = i = rows = height
// y = j = columns = width
void Board::processLeftClick(int ypos, int xpos)
{
	try
	{
		// if already won/lost then window should be stuck until reset
		if (!playing)
			return;

		int x, y;
		x = xpos / tileSize.x;
		y = ypos / tileSize.y;

		// check if the click is on the buttons panel
		if (x >= height)
			return;

		// reveal the tiles iteratively
		revealiteratively(x, y);

		// if tile is a bomb then set losing face
		if (tiles[x][y].tile == Tilename::bomb)
		{
			buttons->setFace(false);
			debugmode = false;
			ToggleDebugMode();
			playing = false;
		}

		// if all non bomb tiles are revealed then set win face
		if (revealed_tiles == non_bomb_tiles)
		{
			buttons->setFace(true);
			debugmode = true;
			ToggleDebugMode();
			playing = false;
			flagRemainingTiles();
		}
	}
	catch (exception &ex)
	{
		cout << "Exception in processLeftClick: " << ex.what() << endl;
	}
}

void Board::revealiteratively(int x, int y)
{
	Tile &tile = tiles[x][y];
	if (tile.state != TileState::revealed)
	{
		revealTile(x, y);
	}

	if (tile.tile <= Tilename::zero && tile.state != TileState::flagged)
	{
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				if (i < 0 || j < 0 || i >= height || j >= width)
				{
					continue;
				}
				else if (!(i == x &&  j == y) && tiles[i][j].state != TileState::revealed)
				{
					revealiteratively(i, j);
				}
			}
		}
	}
}

void Board::revealTile(int x, int y)
{
	Tile &tile = tiles[x][y];

	if (tile.state == TileState::revealed)
		return;

	if (tile.state != TileState::flagged)
	{
		tile.state = TileState::revealed;
		changeTile(x, y, tile.tile);

		if (tile.tile != Tilename::bomb)
			revealed_tiles++;
	}
}

void Board::hideTile(int x, int y)
{
	Tile &tile = tiles[x][y];

	tile.state = tile.prevState;
	if (tile.state == TileState::hidden)
		changeTile(x, y, Tilename::hidden);

}

void Board::changeTile(int x, int y, Tilename tile)
{
	// find its position in the tileset texture
	int tu = (int)tile % (m_tileset.getSize().x / tileSize.x);
	int tv = (int)tile / (m_tileset.getSize().x / tileSize.x);

	// get a pointer to the current tile's quad
	sf::Vertex* quad = &m_vertices[(y + x * width) * 4];

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y + 16);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y + 16);
	quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y + 16);
	quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y + 16);
}

void Board::processRightClick(int ypos, int xpos)
{
	try
	{
		int x, y;
		x = xpos / tileSize.x;
		y = ypos / tileSize.y;

		// on right click, if the tile is hidden then change it to flagged, if flagged then change it to hidden. No operation will be performed on revealed tile
		if (tiles[x][y].state == TileState::hidden)
		{
			tiles[x][y].state = TileState::flagged;
			changeTile(x, y, Tilename::flag);
			buttons->decrement_bombs();
		}
		else if (tiles[x][y].state == TileState::flagged)
		{
			tiles[x][y].state = TileState::hidden;
			changeTile(x, y, Tilename::hidden);
			buttons->increment_bombs();
		}
	}
	catch (exception & ex)
	{
		cout << "Exception in processRightClick: " << ex.what() << endl;
	}
}

void Board::ToggleDebugMode()
{
	if (!playing)
		return;
	debugmode = debugmode ? false : true;

	// if bombs are hidden, reveal them and if revealed then hide them
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			if (tiles[i][j].tile == Tilename::bomb)
			{
				if (debugmode && tiles[i][j].state == TileState::revealed)
				{
					// This will not be used in practical scenario because the game will end if bomb is already revaealed
					tiles[i][j].prevState = TileState::revealed;
				}
				else if (debugmode && tiles[i][j].state == TileState::hidden)
				{
					revealTile(i, j);
				}
				else if (!debugmode && tiles[i][j].state == TileState::revealed)
				{
					hideTile(i, j);
				}
			}
		}
	}
}

void Board::flagRemainingTiles()
{
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			if (tiles[i][j].state != TileState::revealed)
			{
				tiles[i][j].state = TileState::flagged;
				changeTile(i, j, Tilename::flag);
				buttons->decrement_bombs();
			}
		}
	}
}

