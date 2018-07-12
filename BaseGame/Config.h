#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

//Struct which defines how a section is stored
struct ConfigSection
{
	std::string Name;
	std::map<std::string, std::string> Options;
};

//Class for dealing with configuration options of our game
class Config
{
public:
	Config();
	~Config();

	//Parse config file
	bool ParseFile (std::string filename);
	//Parse command line args
	bool ParseCommandLine(int argc, char ** argsv);
	
	//Get Option from the config as a string
	const std::string GetOption(const std::string& section,const std::string& option,const std::string& defaultValue="");
	//Get Option from the config as a bool
	bool GetOptionAsBool(const std::string& section, const std::string& option, bool defaultValue=false);
	//Get Option from the config as a int
	int GetOptionAsInt(const std::string& section, const std::string& option, int defaultValue = 0);

	const static std::string DEFAULT_CONFIGFILENAME;
	
	//Add an option
	void AddOption(const std::string& section, const std::string& option, const std::string& value);

private:

	//Process one line from the config file
	void ProcessLine(std::string& line);
	//Create a config section
	ConfigSection CreateSection(std::string& line);
	//Parse an Option line from the config file
	std::pair<std::string, std::string> ParseOption(std::string& line);

private:
	std::map<std::string,ConfigSection> ConfigSections;
	ConfigSection * CurrentSection;
};