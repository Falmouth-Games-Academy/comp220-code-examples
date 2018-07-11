#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

bool ConfigFile::Parse(std::string & filename)
{
	ConfigSection * currentSection = nullptr;
	std::ifstream configFile;

	configFile.open(filename.c_str());
	std::string line;

	while (!configFile.eof())
	{
		std::getline(configFile, line);
		ProcessLine(line, currentSection);
	}
	return false;
}

void ConfigFile::ProcessLine(std::string & line, ConfigSection * currentSection)
{
}
