#include "LogWriter.h"

#include <fstream>

LogWriter::LogWriter(const std::string& logFilePath)
	: m_filePath(logFilePath)
{
}

bool LogWriter::write(const std::string &message)
{
	std::ofstream output(m_filePath);
	if (!output.is_open())
		return false;
	output << message;
	return true;
}
