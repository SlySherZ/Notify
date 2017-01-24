#include "stdafx.h"
#include "Settings.h"

const wstring Settings::separator = L":   :";

Settings::Settings(const wchar_t* file)
{
	filename = file;

}

wstring Settings::read(const wstring key)
{

	wstring line;
	wifstream file(filename);
	wstring starters = key + separator;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			if (line.find(starters) == 0)
			{
				file.close();

				return line.substr(starters.length(), line.length());
			}

		}
		file.close();
		return L"0Key not found!";
	}
	else
		return L"0No settings file!";
	
		
}

void Settings::set(const wstring key, const wstring value)
{

}
