#include "ProgramOptions.h"

ProgramOptions::ProgramOptions()
{
}

ProgramOptions::~ProgramOptions()
{
	Options.clear();
}

void ProgramOptions::AddOption(std::string & option, std::string & value)
{
	Options[option] = value;
}

const std::string ProgramOptions::GetOption(const std::string & option)
{
	//do options exist from the command args
	if (OptionExists(option)) {
		return Options[option];
	}
	else
	{
	}
}

bool ProgramOptions::GetOptionAsBool(const std::string& option)
{
	std::string optionValue = GetOption(option);
	// Convert to lower case to make string comparisons case-insensitive
	//std::transform(optionValue.begin(), optionValue.end(), optionValue.begin(), ::tolower);
	if (optionValue == "true" || optionValue == "yes" || optionValue == "on" || optionValue == "1")
		return true;
	else if (optionValue == "false" || optionValue == "no" || optionValue == "off" || optionValue == "0")
		return false;
	return true;
}

int ProgramOptions::GetOptionAsInt(const std::string& option)
{
	std::string optionValue = GetOption(option);
	const char* value = optionValue.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);

	return (int)n;
}

void ProgramOptions::ChangeOption(std::string & option, std::string & value)
{
	Options[option] = value;
}

bool ProgramOptions::OptionExists(const std::string & option)
{
	return Options.find(option) != Options.end();
}

void ProgramOptions::ParseConfigFile(const std::string& config)
{
}
