#pragma once

#include <string>
#include <ostream>

#include "ProgressNotifier/ProgressNotifier.h"

#include "LoadableData/ObjData/ObjData.h"

namespace ObjDataTools {
namespace Export {
namespace Internal {

void writeVertices(
            std::ostream &output,
            const std::vector<glm::vec3> &vertices,
            const unsigned int startLine = 0,
            const unsigned int linesCount = 0,
            ProgressNotifier *progressNotifier = nullptr,
            bool *cancel = nullptr);
void writeNormals(
            std::ostream &output,
            const std::vector<glm::vec3> &normals,
            const unsigned int startLine = 0,
            const unsigned int linesCount = 0,
            ProgressNotifier *progressNotifier = nullptr,
            bool *cancel = nullptr);
void writeTextures(
            std::ostream &output,
            const std::vector<glm::vec2> &textures,
            const unsigned int startLine = 0,
            const unsigned int linesCount = 0,
            ProgressNotifier *progressNotifier = nullptr,
            bool *cancel = nullptr);
void writeFaces(
            std::ostream &output,
            const ObjData::Indices &indices,
            const unsigned int startLine = 0,
            const unsigned int linesCount = 0,
            ProgressNotifier *progressNotifier = nullptr,
            bool *cancel = nullptr);

}
}
}