#include <SFML/Graphics.hpp>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include "Board.h"
#include "Tile.hpp"
#include "Buttons.h"
#define TILESIZE 32

using namespace std;
void readConfig(unsigned int &width, unsigned int &height, unsigned int &bombs);

int main()
{

	unsigned int width, height, bombs;
	readConfig(width, height, bombs);

	// create the window
	sf::RenderWindow window(sf::VideoMode(width * TILESIZE, height * TILESIZE + 88), "MineSweeper");

	// create the tilemap
	TileMap map(width, height, bombs);
	map.Generate();
	vector<vector<Tile>>* tilesptr = map.getTileMap();

	Board board(sf::Vector2u(TILESIZE, TILESIZE), width, height, tilesptr);
	if (!board.load("images/m_merged.png"))
		return -1;

	Buttons buttons(width, height * TILESIZE);
	if (!buttons.load("images/m_merged.png"))
		return -1;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					board.processRightClick(event.mouseButton.x, event.mouseButton.y);
				}
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					board.processLeftClick(event.mouseButton.x, event.mouseButton.y);
					LeftClickAction action = buttons.processLeftClick(event.mouseButton.x, event.mouseButton.y);

					if (action != LeftClickAction::NoAction)
					{
						switch (action)
						{
						case LeftClickAction::ResetGame:
						{
							board.Reset();
							map.Generate();
							tilesptr = map.getTileMap();
							break;
						}
						case LeftClickAction::ToggleDebug:
						{
							board.ToggleDebugMode();
							break;

						}
						case LeftClickAction::LoadTest1:
						{
							cout << "load test 1" << endl;
							break;

						}
						case LeftClickAction::LoadTest2:
						{
							cout << "load test2" << endl;
							break;

						}
						case LeftClickAction::LoadTest3:
						{
							cout << "load test3" << endl;
							break;
						}
						}

					}

				}
			}
		}

		// draw the board
		window.clear(sf::Color(255, 255, 255));
		window.draw(board);
		window.draw(buttons);
		window.display();
	}

	return 0;
}

void readConfig(unsigned int &width, unsigned int &height, unsigned int &bombs)
{
	std::ifstream file("boards/config.cfg");
	string line;
	getline(file, line);
	width = stoi(line);

	getline(file, line);
	height = stoi(line);

	getline(file, line);
	bombs = stoi(line);
}

void loadTest(string filename)
{
	std::ifstream file("boards/" + filename);

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 25; j++)
		{

		}
	}
	
}