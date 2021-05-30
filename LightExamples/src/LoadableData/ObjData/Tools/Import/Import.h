#pragma once

#include <string>
#include <istream>

#include "ProgressNotifier/ProgressNotifier.h"

class ObjData;

namespace ObjDataTools {
namespace Import {

void fromFile(
			ObjData& objdata,
			std::string& errorMessage,
			const std::string& filePath,
			ProgressNotifier* progressNotifier = nullptr,
			bool* cancel = nullptr);
void fromStream(
			ObjData& objdata,
			std::string& errorMessage,
			std::istream& input,
			ProgressNotifier* progressNotifier = nullptr,
			bool* cancel = nullptr);

}
}

