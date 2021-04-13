#include "Tile.hpp"
#include <iostream>
using namespace std;

#define tiles  (*tilesptr)

// This function calculates m unique random numbers from 0 to n
void getrand(int m, int n, vector<int> &randNum)
{
	//srand(3);
	int *arr = new int[n];
	for (int i = 0; i < n; ++i)
		arr[i] = i;

	for (int i = 0; i < m; ++i)
	{
		// Find a random index in array portion 0 to n-i and swap arr[n-i] with arr[random from 0 to n-i]
		int randn = rand() % (n - i);
		int temp = arr[randn];
		arr[randn] = arr[n - i - 1];
		arr[n - i] = temp;
	}

	for (int i = 0; i < m; i++)
	{
		randNum[i] = arr[n - m + i];
	}
}

TileMap::TileMap(int width, int height, int n_mines)
{
	try
	{
		this->height = height;
		this->width = width;

		// allocate a 2D vector of tiles
		tilesptr = new vector<vector<Tile>>(height, vector<Tile>(width));

		// Find location of n_mines random bombs in the map
		vector <int> mines(n_mines);
		getrand(n_mines, width * height, mines);

		// Mark the location of bombs in the tiles
		for (int i = 0; i < n_mines; i++)
		{
			int x = mines[i] / width;
			int y = mines[i] % width;
			tiles[x][y].tile = Tilename::bomb;
		}

		// Fill the remaining tiles with the number of bombs around it
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (tiles[i][j].tile != Tilename::bomb)
				{
					tiles[i][j].tile = bombsAround(i, j);
				}
			}
		}

	}
	catch (exception &ex)
	{
		cout << "Caught exception in TileMap: " << ex.what() << endl;
	}
}

// This function calculated the bombs around a tile and returns corresponding tilename
Tilename TileMap::bombsAround(int x, int y)
{
	unsigned int bombs = 0;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			// continue if tile number is outside of the borders and if it is same as the tile for which we are calculating neighbors
			if (i < 0 || j < 0 || (i == x &&  j == y) || i >= height || j >= width)
			{
				continue;
			}
			else
			{
				if (tiles[i][j].tile == Tilename::bomb)
					bombs++;
			}
		}
	}

	// first tile name is "hidden" so we have to add + 1 to use the enums from "zero" onwards
	return (Tilename)(bombs + 1);
}

vector<vector<Tile>>* TileMap::getTileMap()
{
	return tilesptr;
}