#include "ImportInternal.h"

#include <cassert>

std::vector<std::string> ObjDataTools::Import::Internal::split(const std::string& s, const char separator, const bool skipEmptyElements)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while ((pos = s.find(separator, pos)) != std::string::npos) {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        if ((!substring.empty()) || (!skipEmptyElements))
            output.push_back(substring);
        prev_pos = ++pos;
    }

    std::string substring(s.substr(prev_pos, pos - prev_pos));
    if ((!substring.empty()) || (!skipEmptyElements))
        output.push_back(substring); // Last word
    return output;
}

int ObjDataTools::Import::Internal::getIndexFirstNonSpaceSymbol(const std::string& line)
{
    const size_t size = line.size();
    for (size_t i = 0; i < size; ++i)
        if (line[i] != ' ')
            return i;
    return size;
}

unsigned int ObjDataTools::Import::Internal::getLinesCount(std::istream& stream)
{
    int linesCount = 0;
    std::string line;
    while (!stream.eof())
    {
        std::getline(stream, line);
        ++linesCount;
    }
    stream.clear();
    stream.seekg(0);
    return linesCount;
}

bool ObjDataTools::Import::Internal::parseInt(const std::string& string, int& returnValue)
{
    try {
        int inputInt = std::stoi(string);
        returnValue = inputInt;
        return true;
    }
    catch (std::exception const& e)
    {
        return false;
    }
}

bool ObjDataTools::Import::Internal::parseFloat(const std::string& string, float& returnValue)
{
    try {
        float inputFloat = std::stof(string);
        returnValue = inputFloat;
        return true;
    }
    catch (std::exception const& e)
    {
        return false;
    }
}

bool ObjDataTools::Import::Internal::parseVector2D(const std::string& string, glm::vec2& returnVertex, std::string& errorMessage)
{
    if (string.empty())
    {
        errorMessage = "Empty body";
        return false;
    }
    std::vector<std::string> list = split(string, ' ');
    int size = list.size();
    if (size != 2)
    {
        errorMessage = "Number of elements is not equal to 2";
        return false;
    }
    float components[2] = { 0.f };
    for (int i = 0; i < size; ++i)
    {
        if (!parseFloat(list[i], components[i]))
        {
            errorMessage = "Vector component is not a float value";
            return false;
        }
    }
    returnVertex = glm::vec2(components[0], components[1]);
    return true;
}

bool ObjDataTools::Import::Internal::parseVector3D(const std::string& string, glm::vec3& returnVertex, std::string& errorMessage)
{
    if (string.empty())
    {
        errorMessage = "Empty body";
        return false;
    }
    std::vector<std::string> list = split(string, ' ');
    int size = list.size();
    if (size != 3)
    {
        errorMessage = "Number of elements is not equal to 3";
        //return false;
    }
    if (size > 3)
    {
        errorMessage = "Number of elements is more then 3";
        return false;
    }
    float components[3] = { 0.f };
    for (int i = 0; i < size; ++i)
    {
        if (!parseFloat(list[i], components[i]))
        {
            errorMessage = "Vector component is not a float value";
            return false;
        }
    }
    returnVertex = glm::vec3(components[0], components[1], components[2]);
    return true;
}

bool ObjDataTools::Import::Internal::parseVector4D(const std::string& string, glm::vec4& returnVertex, std::string& errorMessage)
{
    if (string.empty())
    {
        errorMessage = "Empty body";
        return false;
    }
    std::vector<std::string> list = split(string, ' ');
    int size = list.size();
    if (size != 4)
    {
        errorMessage = "Number of elements is not equal to 4";
        return false;
    }
    float components[4] = { 0.f };
    for (int i = 0; i < size; ++i)
    {
        if (!parseFloat(list[i], components[i]))
        {
            errorMessage = "Vector component is not a float value";
            return false;
        }
    }
    returnVertex = glm::vec4(components[0], components[1], components[2], components[3]);
    return true;
}

bool ObjDataTools::Import::Internal::checkCountIndices(const ObjData::Indices& indices, std::string& errorMessage)
{
    int verticesSize = indices.polygons.size();
    int texturesSize = indices.uv.size();
    int normalsSize = indices.normals.size();
    int polygonsStartsSize = indices.polygonsStarts.size();

    if (verticesSize == 0)
    {
        errorMessage = "Not found any vertex informations in file";
        return false;
    }

    bool vert_text_compare = (texturesSize == 0) || (texturesSize == verticesSize);
    if (!vert_text_compare)
    {
        errorMessage = "Texture coord indices were not specified for all the polygons";
        return false;
    }
    bool vert_norm_compare = (normalsSize == 0) || (normalsSize == verticesSize);
    if (!vert_norm_compare)
    {
        errorMessage = "Normal indices were not specified for all the polygons";
        return false;
    }
    if (polygonsStartsSize == 0)
    {
        errorMessage = "Not found any face informations in file";
        return false;
    }
    return true;
}

bool ObjDataTools::Import::Internal::checkRangeIndices(const ObjData::Indices& indices, const int& verticesDataSize, const int& texturesDataSize, const int& normalsDataSize, std::string& errorMessage)
{
    const std::vector<int>& vertices = indices.polygons;
    const std::vector<int>& textures = indices.uv;
    const std::vector<int>& normals = indices.normals;
    const std::vector<int>& polygonStarts = indices.polygonsStarts;
    const int polygonsCount = polygonStarts.size() - 1;

    const int& lastVertexIndex = verticesDataSize - 1;
    const int& lastTextureIndex = texturesDataSize - 1;
    const int& lastNormalIndex = normalsDataSize - 1;

    for (int startIndex = 0; startIndex < polygonsCount; ++startIndex)
    {
        int elementsCount = polygonStarts[startIndex + 1] - polygonStarts[startIndex];
        for (int polygonElementIndex = 0; polygonElementIndex < elementsCount; ++polygonElementIndex)
        {
            int currentIndex = polygonStarts[startIndex] + polygonElementIndex;
            if (vertices[currentIndex] > lastVertexIndex || vertices[currentIndex] < 0)
            {
                errorMessage = std::string("Polygon " + std::to_string(startIndex + 1) + " refers to non-existing vertex " + std::to_string(vertices[currentIndex] + 1));
                return false;
            }
            if (!textures.empty())
            {
                if (textures[currentIndex] > lastTextureIndex || textures[currentIndex] < 0)
                {
                    errorMessage = std::string("Polygon " + std::to_string(startIndex + 1) + " refers to non-existing texture index " + std::to_string(textures[currentIndex] + 1));
                    return false;
                }
            }
            if (!normals.empty())
            {
                if (normals[currentIndex] > lastNormalIndex || normals[currentIndex] < 0)
                {
                    errorMessage = std::string("Polygon " + std::to_string(startIndex + 1) + " refers to non-existing normal index " + std::to_string(normals[currentIndex] + 1));
                    return false;
                }
            }
        }
    }
    return true;
}

bool ObjDataTools::Import::Internal::checkIndices(const ObjData::Indices& indices, const int& verticesDataSize, const int& texturesDataSize, const int& normalsDataSize, std::string& errorMessage)
{
    if (!checkCountIndices(indices, errorMessage))
        return false;
    if (!checkRangeIndices(indices, verticesDataSize, texturesDataSize, normalsDataSize, errorMessage))
        return false;
    return true;
}

void ObjDataTools::Import::Internal::separateTokenAndBody(const std::string& line, std::string& returnToken, std::string& returnBody)
{
    int indent;
    if (line[0] == ' ') // if line has indent
        indent = ObjDataTools::Import::Internal::getIndexFirstNonSpaceSymbol(line);
    else
        indent = 0;

    if (indent == line.size()) // if line has only indent
    {
        returnToken = std::string();
        returnBody = std::string();
        return;
    }

    size_t firstSpaceIndex = line.find(" ", indent);
    bool entireStringIsToken = (firstSpaceIndex == std::string::npos);
    //int firstSpaceIndex = std::find(line.begin() + indent, line.end(), ' ') - line.begin();
    //bool entireStringIsToken = (firstSpaceIndex == -1);
    if (entireStringIsToken)
    {
        int tokenLength = -1; // to end of a line
        returnToken = line.substr(indent, tokenLength);
        returnBody = std::string();
    }
    else
    {
        int tokenLength = firstSpaceIndex - indent;
        returnToken = line.substr(indent, tokenLength);
        int bodyLength = line.size() - (firstSpaceIndex + 1);
        returnBody = line.substr(firstSpaceIndex + 1, bodyLength);
    }
}

void ObjDataTools::Import::Internal::readTokenAndBody(const std::string& line, ObjDataTools::Import::Internal::LineToken& returnToken, std::string& returnBody)
{
    std::string lineTokenString;
    separateTokenAndBody(line, lineTokenString, returnBody);
    int tokenIndex = 0;

    for (const std::string& currentToken : lineTokenStrings)
    {
        if (lineTokenString == currentToken)
        {
            returnToken = lineTokens[tokenIndex];
            return;
        }
        ++tokenIndex;
    }
    returnToken = LineToken::UNKNOWN;
}

bool ObjDataTools::Import::Internal::parsePolygonElement(const std::string& polygonElementBody, bool& hasVertex, int& vertexInd, bool& hasTexture, int& textureInd, bool& hasNormal, int& normalInd, std::string& errorMessage)
{
    if (polygonElementBody.empty())
    {
        errorMessage = "Polygon element is empty";
        return false;
    }
    
    std::vector<std::string> element = split(polygonElementBody, '/', false);
    if (element.size() > 3)
    {
        errorMessage = "More than 3 indices in polygon element";
        return false;
    }

    hasVertex = !element[0].empty();
    if (hasVertex && !parseInt(element[0], vertexInd))
    {
        errorMessage = "Vertex index is not an integer";
        return false;
    }
    vertexInd--; // in .obj index starts from 1

    hasTexture = (element.size() > 1) && (!element[1].empty());
    if (hasTexture && !parseInt(element[1], textureInd))
    {
        errorMessage = "Texture index is not an integer";
        return false;
    }
    textureInd--; // in .obj index starts from 1

    hasNormal = (element.size() > 2) && (!element[2].empty());
    if (hasNormal && !parseInt(element[2], normalInd))
    {
        errorMessage = "Normal index is not an integer";
        return false;
    }
    normalInd--; // in .obj index starts from 1

    return true;
}

bool ObjDataTools::Import::Internal::parsePolygon(const std::string& polygonBody, ObjData::Indices& indices, std::string& errorMessage)
{
    if (polygonBody.empty())
    {
        errorMessage = "Polygon body is empty";
        return false;
    }
    std::vector<std::string> elements = split(polygonBody, ' ');
    int elementsCount = elements.size();
    if (elementsCount < 3)
    {
        errorMessage = "Polygon contains less than 3 vertices";
        return false;
    }

    std::vector<int>& starts = indices.polygonsStarts;
    if (starts.size() < 1)
        starts.push_back(0);
    starts.push_back(elementsCount + starts[starts.size() - 1]);

    int texturesCount = 0;
    int normalsCount = 0;
    for (int elementIndex = 0; elementIndex < elementsCount; ++elementIndex)
    {
        bool hasVertex = false;
        int vertexInd = -1;
        bool hasTexture = false;
        int textureInd = -1;
        bool hasNormal = false;
        int normalInd = -1;
        if (!parsePolygonElement(elements[elementIndex], hasVertex, vertexInd,
            hasTexture, textureInd, hasNormal, normalInd, errorMessage))
        {
            errorMessage = "Failed to parse polygon. " + errorMessage;
            return false;
        }
        if (!hasVertex)
        {
            errorMessage = "Polygon block doesn\'t contain a vertex index";
            return false;
        }
        indices.polygons.push_back(vertexInd);

        if (hasTexture)
        {
            indices.uv.push_back(textureInd);
            texturesCount++;
        }
        if (hasNormal)
        {
            indices.normals.push_back(normalInd);
            normalsCount++;
        }
    }
    if ((texturesCount > 0) && (texturesCount < elementsCount))
    {
        errorMessage = "Texture coordnate indices were not specified for all polygon elements";
        return false;
    }
    if ((texturesCount > 0) && (texturesCount < elementsCount))
    {
        errorMessage = "Normal indices were not specified for all polygon elements";
        return false;
    }
    return true;
}

bool ObjDataTools::Import::Internal::loadBodyByToken(ObjData& objData, const ObjDataTools::Import::Internal::LineToken& token, const std::string body, std::string& errorMessage, const unsigned int& currentLineNumber)
{
    if (token == LineToken::UNKNOWN)
        return true;

    if (token == LineToken::VERTEX)
    {
        glm::vec3 vertex;
        glm::vec4 vertex4D; // if vertex has a [w] element
        bool useVertex4D = false;
        if (!parseVector3D(body, vertex, errorMessage))
        {
            errorMessage = std::string("Error in line " + std::to_string(currentLineNumber) + ". " + errorMessage);
            useVertex4D = true;
            std::string errorMessage;
            if (!parseVector4D(body, vertex4D, errorMessage))
                return false;
        }
        if (useVertex4D)
        {
            vertex = glm::vec3(vertex4D.x / vertex4D.w,
                vertex4D.y / vertex4D.w,
                vertex4D.z / vertex4D.w);
            errorMessage = "";
        }
        objData.data.vertices.push_back(vertex);
        return true;
    }
    if (token == LineToken::TEXTURE)
    {
        glm::vec2 texture;
        glm::vec3 texture3D; // if texture has a [z] element
        bool useTexture3D = false;
        if (!parseVector2D(body, texture, errorMessage))
        {
            errorMessage = std::string("Error in line " + std::to_string(currentLineNumber) + ". " + errorMessage);
            useTexture3D = true;
            std::string errorMessage;
            if (!parseVector3D(body, texture3D, errorMessage))
                return false;
        }
        if (useTexture3D)
        {
            texture = glm::vec2(texture3D.x, texture3D.y); // for now just skip [z]
            errorMessage = "";
        }
        objData.data.uv.push_back(texture);
        return true;
    }
    if (token == LineToken::NORMAL)
    {
        glm::vec3 normal;
        if (!parseVector3D(body, normal, errorMessage))
        {
            errorMessage = std::string("Error in line " + std::to_string(currentLineNumber) + ". " + errorMessage);
            return false;
        }
        objData.data.normals.push_back(normal);
        return true;
    }
    if (token == LineToken::FACE)
    {
        if (!parsePolygon(body, objData.indices, errorMessage))
        {
            errorMessage = std::string("Error in line " + std::to_string(currentLineNumber) + ". " + errorMessage);
            return false;
        }
        return true;
    }

    errorMessage = "ObjDataTools::Import::Internal::loadBodyByToken(); invalid token";
    return false;
}