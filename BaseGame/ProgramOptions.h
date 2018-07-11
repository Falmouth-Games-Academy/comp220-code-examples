#pragma once

#include <string>
#include <map>

#include "INIReader.h"

class ProgramOptions
{
public:
	ProgramOptions();
	~ProgramOptions();

	void AddOption(std::string& option, std::string& value);
	const std::string GetOption(const std::string& option);
	bool GetOptionAsBool(const std::string& option);
	int GetOptionAsInt(const std::string& option);

	void ChangeOption(std::string& option, std::string& value);

	bool OptionExists(const std::string& option);

	void ParseConfigFile(const std::string& config);
private:
	std::map<std::string, std::string> Options;
	INIReader ConfigFile;
};