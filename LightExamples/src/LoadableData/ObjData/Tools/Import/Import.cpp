#include "Import.h"

#include <fstream>

#include "ImportInternal.h"

void ObjDataTools::Import::fromFile(
			ObjData& objdata,
			std::string& errorMessage,
			const std::string& filePath,
			ProgressNotifier* progressNotifier,
			bool* cancel)
{
    std::ifstream input(filePath);
    if (!input.is_open())
    {
        errorMessage = "Failed to open file \"" + filePath + "\"";
        return;
    }

    fromStream(
		objdata,
		errorMessage,
		input,
		progressNotifier,
		cancel);
}

void ObjDataTools::Import::fromStream(
			ObjData& objdata,
			std::string& errorMessage,
			std::istream& input,
			ProgressNotifier* progressNotifier,
			bool* cancel)
{
    ObjData loadableData;
    unsigned int currentLineNumber = 0;
    unsigned int linesCount = 39;//Internal::getLinesCount(input);
    bool isProgressNotifierExist = progressNotifier != nullptr;

    const bool usePercentsAsInteger = true;
    int currentPercent = 0;

    if (isProgressNotifierExist)
        progressNotifier->start(0, linesCount);

    // Loading data
    while (!input.eof())
    {
        if (cancel != nullptr)
        {
            if (*cancel == true)
            {
                errorMessage = "Canceled by user";
                return;
            }
        }

        ++currentLineNumber;

        if (isProgressNotifierExist)
        {
            if (usePercentsAsInteger)
            {
                int percentsAsInteger = (currentLineNumber * 100) / linesCount;
                if (percentsAsInteger > currentPercent)
                {
                    currentPercent = percentsAsInteger;
                    progressNotifier->setProgress(currentLineNumber);
                }
            }
            else
                progressNotifier->setProgress(currentLineNumber);
        }
        std::string line;
        std::getline(input, line);

        //line.replace('\t', ' ');
        const size_t lineSize = line.size();
        for (size_t i = 0; i < lineSize; ++i)
            if (line[i] == '\t')
                line[i] = ' ';
        Internal::LineToken token;
        std::string body;

        Internal::readTokenAndBody(line, token, body);

        if (!Internal::loadBodyByToken(loadableData, token, body, errorMessage, currentLineNumber))
        {
            if (isProgressNotifierExist)
                progressNotifier->finish();
            return;
        }
    }

    // Checks for errors
    if (!Internal::checkIndices(loadableData.indices, loadableData.data.vertices.size(), loadableData.data.uv.size(),
        loadableData.data.normals.size(), errorMessage))
    {
        // FOR NOW JUST REMOVE TEXTURE IF ITS NOT FOR FULL MODEL
        if (errorMessage == "Texture coord indices were not specified for all the polygons") {
            loadableData.data.uv.clear();
            loadableData.indices.uv.clear();
            errorMessage.clear();
        }
        else {
            if (isProgressNotifierExist)
                progressNotifier->finish();
            return;
        }
    }

    objdata = loadableData;
    if (isProgressNotifierExist)
        progressNotifier->finish();
}
