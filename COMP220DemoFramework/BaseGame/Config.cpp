#include "Config.h"

const std::string Config::DEFAULT_CONFIGFILENAME = "config.ini";

Config::Config()
{
}

Config::~Config()
{
}

bool Config::ParseFile(std::string  filename)
{
	//use an ifstream to load in the file
	std::ifstream configFile(filename.c_str());
	std::string line;

	//Was the file valid?
	if (configFile.good()) {
		//Keep going until we hit end of file
		while (!configFile.eof())
		{
			//read a line at a time and parse
			std::getline(configFile, line);
			ProcessLine(line);
		}

		configFile.close();
		return true;
	}
	return false;
}

bool Config::ParseCommandLine(int argc, char ** argsv)
{
	for (int i = 1; i < argc; i++)
	{
		//Other command line options should be section-option=value
		//so parse this for each argument
		std::string optionValuePair = argsv[i];
		int delminatorPos = optionValuePair.find('=');
		
		std::string sectionOptionPair = optionValuePair.substr(0, delminatorPos);

		int sectionDelimPos = sectionOptionPair.find('-');
		std::string sectionName = sectionOptionPair.substr(0, sectionDelimPos);
		std::string optionName = sectionOptionPair.substr(sectionDelimPos + 1, sectionOptionPair.length());

		std::string optionValue = optionValuePair.substr(delminatorPos+1);

		AddOption(sectionName, optionName, optionValue);
	}
	return true;
}

const std::string Config::GetOption(const std::string & section, const std::string & option, const std::string & defaultValue)
{
	//Check if the option and section exist
	if (ConfigSections.find(section) != ConfigSections.end() && ConfigSections[section].Options.find(option) != ConfigSections[section].Options.end())
	{
		return ConfigSections[section].Options[option];
	}

	return defaultValue;
}

bool Config::GetOptionAsBool(const std::string & section, const std::string & option, bool defaultValue)
{
	std::string optionValue = GetOption(section, option);
	// Convert to lower case to make string comparisons case-insensitive
	std::transform(optionValue.begin(), optionValue.end(), optionValue.begin(), ::tolower);
	if (optionValue == "true" || optionValue == "yes" || optionValue == "on" || optionValue == "1")
		return true;
	else if (optionValue == "false" || optionValue == "no" || optionValue == "off" || optionValue == "0")
		return false;

	return defaultValue;
}

int Config::GetOptionAsInt(const std::string & section, const std::string & option, int defaultValue)
{
	std::string optionValue = GetOption(section,option);
	const char* value = optionValue.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return (int)n;
}

void Config::AddOption(const std::string & section, const std::string & option, const std::string & value)
{
	//does section and option exist?
	if (ConfigSections.find(section) != ConfigSections.end() && ConfigSections[section].Options.find(option) != ConfigSections[section].Options.end())
	{
		ConfigSections[section].Options[option] = value;
	}
	else
	{
		//Create Section and Option
		ConfigSection configSection;
		configSection.Name = section;

		configSection.Options.insert(std::pair < std::string, std::string>(option, value));
	}

}

void Config::ProcessLine(std::string & line)
{
	;
	if (line[0] == '[')
	{
		ConfigSection section=CreateSection(line);
		ConfigSections.insert(std::pair < std::string, ConfigSection>(section.Name, section));
		CurrentSection = &ConfigSections[section.Name];
	}
	else
	{
		if (CurrentSection)
		{
			if (!line.empty())
				CurrentSection->Options.insert(ParseOption(line));
		}
	}


}

ConfigSection Config::CreateSection(std::string& line)
{
	ConfigSection section;

	section.Name = line.substr(1, line.length() - 2);
	return section;
}

std::pair<std::string, std::string> Config::ParseOption(std::string & line)
{
	//An option is option=value, so parse based on this pattern
	int seperatorIndex = line.find('=');
	std::string optionKey = line.substr(0, seperatorIndex);
	std::string optionValue = line.substr(seperatorIndex + 1, line.length() - 1);

	return std::pair < std::string, std::string>(optionKey,optionValue);
}
