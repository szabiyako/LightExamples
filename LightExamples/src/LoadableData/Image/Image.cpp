#include "Image.h"

void Image::clear()
{
	data.clear();
	width = -1;
	height = -1;
	bitsPerPixel = -1;
}

bool Image::isEmpty()
{
	return data.empty()
		   || (width == -1)
		   || (height == -1)
		   || (bitsPerPixel == -1);
}
