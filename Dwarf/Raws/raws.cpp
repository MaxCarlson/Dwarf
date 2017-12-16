#include "raws.h"
#include "Lua.h"
#include "ItemRead.h"
#include "FileReader.h"

void loadGameTables()
{
	readInItems();
}

void loadRaws()
{
	using namespace FileStrings;

	auto ll = std::make_unique<LuaLife>();

	readFile(0, "Raws/LuaDefs/index.txt");
	for (const std::string & fileName : returnTable(0)->strings)
	{
		loadLuaScript("Raws/LuaDefs/" + fileName);
	}

	loadGameTables();
}
