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
void loadTest(string filename, vector<int> &bombs);

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

	Buttons buttons(width, height * TILESIZE, bombs);
	if (!buttons.load("images/m_merged.png"))
		return -1;

	Board board(sf::Vector2u(TILESIZE, TILESIZE), width, height, tilesptr, &buttons);
	if (!board.load("images/m_merged.png"))
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

					string str = action == LeftClickAction::LoadTest1 ? "testboard1.brd" : "";
					str = action == LeftClickAction::LoadTest2 ? "testboard2.brd" : str;
					str = action == LeftClickAction::LoadTest3 ? "testboard3.brd" : str;

					if (action != LeftClickAction::NoAction)
					{
						switch (action)
						{
						case LeftClickAction::ResetGame:
						{
							board.Reset();
							map.Generate();
							buttons.reset(bombs);
							break;
						}
						case LeftClickAction::ToggleDebug:
						{
							board.ToggleDebugMode();
							break;

						}
						case LeftClickAction::LoadTest1:
						case LeftClickAction::LoadTest2:
						case LeftClickAction::LoadTest3:
						{
							if (width != 25 || height != 16)
							{
								cout << "Please enter width = 25 and height = 16 in config file for test buttons to work" << endl;
								break;
							}
							board.Reset();
							vector<int> _bombs;
							loadTest(str, _bombs);
							//bombs = _bombs.size();
							map.Generate(&_bombs);
							buttons.Update_bombs(_bombs.size());

							break;
						}
						}// end switch case
					} 

				}// end left click processing
			} // end mouse clicked processing
		} // end while

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

void loadTest(string filename, vector<int> &bombs)
{
	std::ifstream file("boards/" + filename);
	char isbomb;
	string line;
	unsigned int index = 0;
	// read file character by character
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			file.get(isbomb);
			if (isbomb == '1')
			{
				bombs.push_back(index);
			}
			index++;
		}
		
		// get line so that we move to next line
		getline(file, line);
		cout << endl;
	}
}