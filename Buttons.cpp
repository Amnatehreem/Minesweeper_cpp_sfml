#include <SFML/Graphics.hpp>
#include "Buttons.h"
#include <iostream>
using namespace std;
#define tiles (*tilesptr)

#define DIGIT_TILE_WIDTH 21
#define LEFT_OFFSET 32
#define DIGIT_TILE_HEIGHT 32
#define DIGIT_TILE_VER_START 16
#define DIGIT_TILE_OFFSET 26 * 32
#define FACE_OFFSET 12 * 32
#define DEBUG_OFFSET FACE_OFFSET + 64 * 3
#define TEST_OFFSET DEBUG_OFFSET + 64
#define BIGTILE_SIZE 64

bool Buttons::load(const std::string& tileset)
{
	// Harcoding the number of tiles here as it will remain fixed
	unsigned int number_of_tiles = 8;
	unsigned int tilesize_small = 32;
	unsigned int tilesize_large = 64;
	unsigned short number_tiles_offset = 26 * 32;

	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);

	m_vertices.resize(number_of_tiles * 4);

	sf::Vertex* quad;

	// fill the position and texture coordinates for digits
	for (int i = 0; i < 3; i++)
	{
		quad = &m_vertices[i * 4];

		// define its 4 corners
		// offset of digits from left side is 32 (after one game block). width of each digit is 21 and height of each digit is 32
		
		quad[0].position = sf::Vector2f(i * DIGIT_TILE_WIDTH + LEFT_OFFSET, height_offset);
		quad[1].position = sf::Vector2f((i + 1) * DIGIT_TILE_WIDTH + LEFT_OFFSET, height_offset);
		quad[2].position = sf::Vector2f((i + 1) * DIGIT_TILE_WIDTH + LEFT_OFFSET, DIGIT_TILE_HEIGHT + height_offset);
		quad[3].position = sf::Vector2f(i * DIGIT_TILE_WIDTH + LEFT_OFFSET, DIGIT_TILE_HEIGHT + height_offset);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(DIGIT_TILE_OFFSET + 0*DIGIT_TILE_WIDTH, 16);
		quad[1].texCoords = sf::Vector2f(DIGIT_TILE_OFFSET + DIGIT_TILE_WIDTH*(0 + 1), 16);
		quad[2].texCoords = sf::Vector2f(DIGIT_TILE_OFFSET + DIGIT_TILE_WIDTH*(0 + 1), DIGIT_TILE_HEIGHT + 16);
		quad[3].texCoords = sf::Vector2f(DIGIT_TILE_OFFSET + 0*DIGIT_TILE_WIDTH, DIGIT_TILE_HEIGHT + 16);
	}

	// Fill the postion and texture coordinates for face
	quad = &m_vertices[12];
	face_offset = (((width - 1) * 32) / 2)/32;
	face_offset *= 32;
	quad[0].position = sf::Vector2f(face_offset, height_offset);
	quad[1].position = sf::Vector2f(face_offset + BIGTILE_SIZE, height_offset);
	quad[2].position = sf::Vector2f(face_offset + BIGTILE_SIZE, BIGTILE_SIZE + height_offset);
	quad[3].position = sf::Vector2f(face_offset, BIGTILE_SIZE + height_offset);

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(FACE_OFFSET, 0);
	quad[1].texCoords = sf::Vector2f(FACE_OFFSET + BIGTILE_SIZE, 0);
	quad[2].texCoords = sf::Vector2f(FACE_OFFSET + BIGTILE_SIZE, BIGTILE_SIZE);
	quad[3].texCoords = sf::Vector2f(FACE_OFFSET, BIGTILE_SIZE);

	// Fill the postion and texture coordinates for debug button
	quad = &m_vertices[16];
	debug_offset = face_offset + 64 + 64;
	quad[0].position = sf::Vector2f(debug_offset, height_offset);
	quad[1].position = sf::Vector2f(debug_offset + BIGTILE_SIZE, height_offset);
	quad[2].position = sf::Vector2f(debug_offset + BIGTILE_SIZE, BIGTILE_SIZE + height_offset);
	quad[3].position = sf::Vector2f(debug_offset, BIGTILE_SIZE + height_offset);

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(DEBUG_OFFSET, 0);
	quad[1].texCoords = sf::Vector2f(DEBUG_OFFSET + BIGTILE_SIZE, 0);
	quad[2].texCoords = sf::Vector2f(DEBUG_OFFSET + BIGTILE_SIZE, BIGTILE_SIZE);
	quad[3].texCoords = sf::Vector2f(DEBUG_OFFSET, BIGTILE_SIZE);

	// Fill the postion and texture coordinates for test buttons
	test_offset = debug_offset + 64;

	for (int i = 0; i < 3; i++)
	{
		quad = &m_vertices[i * 4 + 20];

		// define its 4 corners
		quad[0].position = sf::Vector2f(i * BIGTILE_SIZE + test_offset, height_offset);
		quad[1].position = sf::Vector2f((i + 1) * BIGTILE_SIZE + test_offset, height_offset);
		quad[2].position = sf::Vector2f((i + 1) * BIGTILE_SIZE + test_offset, BIGTILE_SIZE + height_offset);
		quad[3].position = sf::Vector2f(i * BIGTILE_SIZE + test_offset, BIGTILE_SIZE + height_offset);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(TEST_OFFSET + i * BIGTILE_SIZE, 0);
		quad[1].texCoords = sf::Vector2f(TEST_OFFSET + BIGTILE_SIZE*(i + 1), 0);
		quad[2].texCoords = sf::Vector2f(TEST_OFFSET + BIGTILE_SIZE*(i + 1), BIGTILE_SIZE);
		quad[3].texCoords = sf::Vector2f(TEST_OFFSET + i * BIGTILE_SIZE, BIGTILE_SIZE);
	}

	return true;
}

void  Buttons::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

// Process left click: reveal a tile. reveal neighboring tiles that are not bombs
LeftClickAction Buttons::processLeftClick(int ypos, int xpos)
{
	try
	{
		int x, y;
		x = xpos / 32;
		y = ypos;

		if ((x >= height_offset / 32) && y >= face_offset)
		{
			if (y >= face_offset && y < face_offset + 64)
			{
				return LeftClickAction::ResetGame;
			}
			else if (y >= debug_offset && y < debug_offset + 64)
			{
				return LeftClickAction::ToggleDebug;
			}
			else if (y >= test_offset)
			{
				return (LeftClickAction)((y - test_offset) / 64);
			}
		}
	}
	catch (exception &ex)
	{
		cout << "Exception in Buttons::processLeftClick: " << ex.what() << endl;
	}

	return LeftClickAction::NoAction;
}

void Buttons::changeTile(int x, int y, Tilename tile)
{
	//// find its position in the tileset texture
	//int tu = (int)tile % (m_tileset.getSize().x / tileSize.x);
	//int tv = (int)tile / (m_tileset.getSize().x / tileSize.x);

	//// get a pointer to the current tile's quad
	//sf::Vertex* quad = &m_vertices[(y + x * width) * 4];

	//// define its 4 texture coordinates
	//quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
	//quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
	//quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
	//quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
}

