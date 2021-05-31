#include "Import.h"

#include "LoadableData/Image/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void ImageTools::Import::fromFile(
	Image& image,
	const std::string& filePath,
	std::string& errorMessage)
{
	Image imageToLoad;
	errorMessage = std::string();
	stbi_set_flip_vertically_on_load(true);
	unsigned char *localBuffer = stbi_load(filePath.c_str(), &imageToLoad.width, &imageToLoad.height, &imageToLoad.bitsPerPixel, 4);
	if (localBuffer == nullptr) {
		errorMessage = "Failed to import image \"" + filePath + "\"";
		return;
	}
	const size_t imageSize = size_t(imageToLoad.width) * size_t(imageToLoad.height) * size_t(imageToLoad.bitsPerPixel);
	imageToLoad.data.resize(imageSize);
	for (size_t i = 0; i < imageSize; ++i) { //TODO maby replace with memcpy
		imageToLoad.data[i] = localBuffer[i];
	}
	stbi_image_free(localBuffer);
	image = imageToLoad;
}