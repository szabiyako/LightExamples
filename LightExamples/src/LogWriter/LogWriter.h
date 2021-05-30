#pragma once

#include <string>
#include <filesystem>

class LogWriter
{
public:
	LogWriter(const std::string &logFilePath = std::filesystem::current_path().u8string() + "log.txt");
	bool write(const std::string &message);
private:
	const std::string m_filePath;
};

