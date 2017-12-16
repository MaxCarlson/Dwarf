#pragma once
#include <string>
#include <vector>

namespace FileStrings
{
	struct StringTable
	{
		std::vector<std::string> strings;
	};

	void readFile(const int idx, std::string fileName);

	StringTable * returnTable(const int idx);
}

