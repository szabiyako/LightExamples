#include "ExportInternal.h"

void ObjDataTools::Export::Internal::writeVertices(
                std::ostream &output,
                const std::vector<glm::vec3> &vertices,
                const unsigned int startLine,
                const unsigned int linesCount,
                ProgressNotifier *progressNotifier/*= nullptr*/,
                bool *cancel/*= nullptr*/)
{
    int currentPercent = 0;
    int vericesCount = vertices.size();
    //Q_ASSERT_X(vericesCount >= 1, "ObjDataTools::Utils::writeVertices", "No vertices to write");
    for (int i = 0; i < vericesCount; ++i)
    {
        output << "v " << vertices[i].x << ' '
               << vertices[i].y << ' '
               << vertices[i].z << '\n';
        if (progressNotifier != nullptr)
        {
            int currentLineNumber = startLine + i + 1;
            int newPercent = (currentLineNumber * 100) / linesCount;
            if (newPercent > currentPercent)
            {
                currentPercent = newPercent;
                progressNotifier->setProgress(currentLineNumber);
            }
        }
        if (cancel != nullptr)
            if (*cancel == true)
                return;
    }
}

void ObjDataTools::Export::Internal::writeNormals(
                std::ostream &output,
                const std::vector<glm::vec3> &normals,
                const unsigned int startLine,
                const unsigned int linesCount,
                ProgressNotifier *progressNotifier/*= nullptr*/,
                bool *cancel/*= nullptr*/)
{
    int currentPercent = 0;
    int normalsCount = normals.size();
    //Q_ASSERT_X(normalsCount >= 1, "ObjDataTools::Utils::writeNormals", "No normals to write");
    for (int i = 0; i < normalsCount; ++i)
    {
        output << "vn " << normals[i].x << ' '
               << normals[i].y << ' '
               << normals[i].z << '\n';
        if (progressNotifier != nullptr)
        {
            int currentLineNumber = startLine + i + 1;
            int newPercent = (currentLineNumber * 100) / linesCount;
            if (newPercent > currentPercent)
            {
                currentPercent = newPercent;
                progressNotifier->setProgress(currentLineNumber);
            }
        }
        if (cancel != nullptr)
            if (*cancel == true)
                return;
    }
}

void ObjDataTools::Export::Internal::writeTextures(
                std::ostream &output,
                const std::vector<glm::vec2> &textures,
                const unsigned int startLine,
                const unsigned int linesCount,
                ProgressNotifier *progressNotifier/*= nullptr*/,
                bool *cancel/*= nullptr*/)
{
    int currentPercent = 0;
    int texturesCount = textures.size();
    //Q_ASSERT_X(texturesCount >= 1, "ObjDataTools::Utils::writeTextures", "No textures to write");
    for (int i = 0; i < texturesCount; ++i)
    {
        output << "vt " << textures[i].x << ' '
               << textures[i].y << '\n';

        if (progressNotifier != nullptr)
        {
            int currentLineNumber = startLine + i + 1;
            int newPercent = (currentLineNumber * 100) / linesCount;
            if (newPercent > currentPercent)
            {
                currentPercent = newPercent;
                progressNotifier->setProgress(currentLineNumber);
            }
        }
        if (cancel != nullptr)
            if (*cancel == true)
                return;
    }
}

void ObjDataTools::Export::Internal::writeFaces(
                std::ostream &output,
                const ObjData::Indices &indices,
                const unsigned int startLine,
                const unsigned int linesCount,
                ProgressNotifier *progressNotifier/*= nullptr*/,
                bool *cancel/*= nullptr*/)
{
    int currentPercent = 0;
    int polygonsCount = indices.polygonsStarts.size() - 1;
    //Q_ASSERT_X(indices.polygonsStarts.size() > 2, "ObjDataTools::Utils::writeFaces", "indices.polygonsStarts contains less then two elements");
    const std::vector<int>& vertices = indices.polygons;
    const std::vector<int>& textures = indices.uv;
    const std::vector<int>& normals = indices.normals;
    const std::vector<int>& polygonsStarts = indices.polygonsStarts;

    for (int polygonIndex = 0; polygonIndex < polygonsCount; ++polygonIndex)
    {
        if (polygonIndex != 0)
            output << '\n';
        output << "f";
        int elementsCount = polygonsStarts[polygonIndex + 1] - polygonsStarts[polygonIndex];
        for (int elementIndex = 0; elementIndex < elementsCount; ++elementIndex)
        {
            int globalIndex = polygonsStarts[polygonIndex] + elementIndex;
            output << ' ' << vertices[globalIndex] + 1;
            if (!textures.empty() && !normals.empty())
            {
                output << '/' << textures[globalIndex] + 1;
                output << '/' << normals[globalIndex] + 1;
            }
            else if (!textures.empty())
                output << '/' << textures[globalIndex] + 1;
            else if (!normals.empty())
                output << "//" << normals[globalIndex] + 1;
        }
        if (progressNotifier != nullptr)
        {
            int currentLineNumber = startLine + polygonIndex + 1;
            int newPercent = (currentLineNumber * 100) / linesCount;
            if (newPercent > currentPercent)
            {
                currentPercent = newPercent;
                progressNotifier->setProgress(currentLineNumber);
            }
        }
        if (cancel != nullptr)
            if (*cancel == true)
                return;
    }
}
