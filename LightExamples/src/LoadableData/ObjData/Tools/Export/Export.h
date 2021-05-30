#pragma once

#include <string>
#include <ostream>

#include "ProgressNotifier/ProgressNotifier.h"

class ObjData;

namespace ObjDataTools {
namespace Export {

void toFile(const ObjData& objdata,
			std::string& errorMessage,
			const std::string& filePath,
			ProgressNotifier* progressNotifier = nullptr,
			bool* cancel = nullptr,
			const unsigned int& precision = 7);
void toStream(
			const ObjData& objdata,
			std::ostream& output,
			ProgressNotifier* progressNotifier = nullptr,
			bool* cancel = nullptr,
			const unsigned int& precision = 7);

}
}

