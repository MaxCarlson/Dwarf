#include "ItemHelper.h"
#include "../Map/Tile.h"
#include "../Raws/Defs/ItemDefs.h"

ItemHelper itemHelper;

//template<typename T>
void ItemHelper::forEachItem(const std::function<void(Entity)>& func)
{
	//T t;
	for (auto e : getEntities())
	{
		func(e);
	}

}


