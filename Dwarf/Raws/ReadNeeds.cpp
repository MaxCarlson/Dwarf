#include "stdafx.h"
#include "ReadNeeds.h"
#include "Defs\NeedDef.h"
#include "Lua.h"

boost::container::flat_map<std::string, NeedDef> needDefs;

NeedDef * getNeed(const std::string & tag)
{
	auto find = needDefs.find(tag);

	if (find != needDefs.end())
		return &find->second;

	return nullptr;
}

void readInNeeds() noexcept
{
}

void sanityCheckNeeds() noexcept
{
}
