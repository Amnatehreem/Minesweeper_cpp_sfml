#pragma once

void readConfig(unsigned int &width, unsigned int &height, unsigned int &bombs);
void loadTest(std::string filename, std::vector<int> &bombs);

class Data
{
	static unsigned int width;
	static unsigned int height;
	static int bombs;
};