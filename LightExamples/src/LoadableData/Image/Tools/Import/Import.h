#pragma once

#include <string>

struct Image;

namespace ImageTools {
namespace Import {

void fromFile(
	Image &image,
	const std::string& filePath,
	std::string& errorMessage);
}
}

