#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>

struct ConfigSection
{
	std::string Name;
	std::map<std::string, std::string> Options;
};

class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	bool Parse(std::string& filename);
private:

	void ProcessLine(std::string& line, ConfigSection * currentSection);

	std::vector<ConfigSection> ConfigSections;
};