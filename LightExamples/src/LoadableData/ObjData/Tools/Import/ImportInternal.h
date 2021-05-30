#pragma once

#include <string>
#include <istream>

#include "LoadableData/ObjData/ObjData.h"

namespace ObjDataTools {
namespace Import {
namespace Internal {

enum class LineToken
{
    UNKNOWN,
    VERTEX,
    TEXTURE,
    NORMAL,
    FACE
};
static const ObjDataTools::Import::Internal::LineToken lineTokens[5] = {
    ObjDataTools::Import::Internal::LineToken::UNKNOWN,
    ObjDataTools::Import::Internal::LineToken::VERTEX,
    ObjDataTools::Import::Internal::LineToken::TEXTURE,
    ObjDataTools::Import::Internal::LineToken::NORMAL,
    ObjDataTools::Import::Internal::LineToken::FACE
};
static const std::string lineTokenStrings[5] = { 
                                             "",
                                             "v",
                                             "vt",
                                             "vn",
                                             "f" };

std::vector<std::string> split(const std::string& s, const char separator, const bool skipEmptyElements = true);
int getIndexFirstNonSpaceSymbol(const std::string& line);
unsigned int getLinesCount(std::istream& stream);
bool parseInt(const std::string& string, int& returnValue);
bool parseFloat(const std::string& string, float& returnValue);
bool parseVector2D(const std::string& string, glm::vec2& returnVertex, std::string& errorMessage);
bool parseVector3D(const std::string& string, glm::vec3& returnVertex, std::string& errorMessage);
bool parseVector4D(const std::string& string, glm::vec4& returnVertex, std::string& errorMessage);
bool checkCountIndices(const ObjData::Indices& indices, std::string& errorMessage);
bool checkRangeIndices(const ObjData::Indices& indices, const int& verticesDataSize, const int& texturesDataSize, const int& normalsDataSize, std::string& errorMessage);
bool checkIndices(const ObjData::Indices& indices, const int& verticesDataSize, const int& texturesDataSize, const int& normalsDataSize, std::string& errorMessage);
void separateTokenAndBody(const std::string& line, std::string& returnToken, std::string& returnBody);
void readTokenAndBody(const std::string& line, LineToken& returnToken, std::string& returnBody);
bool parsePolygonElement(const std::string& polygonElementBody, bool& hasVertex, int& vertexInd, bool& hasTexture, int& textureInd, bool& hasNormal, int& normalInd, std::string& errorMessage);
bool parsePolygon(const std::string& polygonBody, ObjData::Indices& indices, std::string& errorMessage);
bool loadBodyByToken(ObjData& objData, const LineToken& token, const std::string body, std::string& errorMessage, const unsigned int& currentLineNumber);

}
}
}

