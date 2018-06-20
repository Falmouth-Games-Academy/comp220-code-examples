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
		//return from config file
		for (std::string sectionName : ConfigFile.Sections())
		{
			std::string val= ConfigFile.Get(sectionName, option, "");
			if (!val.empty())
				return val;
			else
				return "";
		}
	}
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
	ConfigFile = INIReader(config);
}
