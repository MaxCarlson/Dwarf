#include "../stdafx.h"
#include "FileReader.h"


namespace FileStrings
{
	std::unordered_map<int, StringTable> stringTables;

	void readFile(const int idx, std::string fileName)
	{
		StringTable fileStrings;

		std::ifstream f(fileName);

		std::string line;
		while (getline(f, line))
		{
			if (!line.empty())
				fileStrings.strings.emplace_back(line);
		}

		stringTables[idx] = fileStrings;
	}

	StringTable * returnTable(const int idx)
	{
		return &stringTables.at(idx);
	}

}

