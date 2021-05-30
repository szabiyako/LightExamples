#include "Export.h"

#include <fstream>

#include "ExportInternal.h"

void ObjDataTools::Export::toFile(
    const ObjData& objdata,
    std::string& errorMessage,
    const std::string& filePath,
    ProgressNotifier* progressNotifier/*= nullptr*/,
    bool* cancel/*= nullptr*/,
    const unsigned int& precision/*= 7*/)
{
    std::ofstream output(filePath);
    if (!output.is_open())
    {
        errorMessage = "Failed to open file \"" + filePath + "\"";
        return;
    }
    toStream(objdata, output, progressNotifier, cancel, precision);
    if (cancel != nullptr)
        if (*cancel == true)
            errorMessage = "Saving \"" + filePath + "\" was stopped by user";
}

void ObjDataTools::Export::toStream(
    const ObjData& objdata,
    std::ostream& output,
    ProgressNotifier* progressNotifier/*= nullptr*/,
    bool* cancel/*= nullptr*/,
    const unsigned int& precision/*= 7*/)
{
    int precisionBefore = output.precision();
    output.precision(precision);

    unsigned int verticesSize = objdata.data.vertices.size();
    unsigned int texturesSize = objdata.data.uv.size();
    unsigned int normalsSize = objdata.data.normals.size();
    unsigned int polygonsStartsSize = objdata.indices.polygonsStarts.size();

    unsigned int linesCount = verticesSize + texturesSize + normalsSize + polygonsStartsSize;

    bool isProgressNotifierExist = progressNotifier != nullptr;

    if (isProgressNotifierExist)
        progressNotifier->start(0, linesCount);


    if (!objdata.data.vertices.empty())
        Internal::writeVertices(output, objdata.data.vertices, 0, linesCount, progressNotifier, cancel);
    if (!objdata.data.uv.empty())
        Internal::writeTextures(output, objdata.data.uv, verticesSize, linesCount, progressNotifier, cancel);
    if (!objdata.data.normals.empty())
        Internal::writeNormals(output, objdata.data.normals, verticesSize + texturesSize, linesCount, progressNotifier, cancel);

    if (objdata.indices.polygonsStarts.size() > 1)
        Internal::writeFaces(output, objdata.indices, verticesSize + texturesSize + normalsSize, linesCount, progressNotifier, cancel);

    if (isProgressNotifierExist)
        progressNotifier->finish();

    output.precision(precisionBefore);
}