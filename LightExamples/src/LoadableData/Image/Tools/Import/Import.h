#pragma once

#include <string>

class Image;

namespace ImageTools {
namespace Import {

void fromFile(
	Image &image,
	const std::string& filePath,
	std::string& errorMessage);
}
}

