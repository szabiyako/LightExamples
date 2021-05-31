#pragma once

#include <vector>

struct Image
{
	std::vector<unsigned char> data;
	int width = -1;
	int height = -1;
	int bitsPerPixel = -1;

	void clear();
	bool isEmpty();
};

