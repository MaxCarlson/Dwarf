#include "stdafx.h"
#include "RegionHelper.h"
#include "Helpers\MessageQueue.h"
#include "Map\Tile.h"
#include "Raws\Materials.h"
#include "Raws\ItemRead.h"
#include "Raws\Defs\MaterialDef.h"
#include "Raws\Defs\ItemDefs.h"
#include "Raws\raws.h"
#include "Globals\TypesOfActions.h"
#include "ECS\Messages\perform_mining_message.h"
#include "ECS\Messages\recalculate_mining_message.h"

std::unique_ptr<RegionHelper> regionHelper;

MessageQueue<perform_mining_message> miningMessages;

void RegionHelper::init()
{
}

void RegionHelper::performMining(Entity e, const int idx, const int type)
{
	miningMessages.emplace(perform_mining_message{ e, idx, static_cast<uint8_t>(type) });
}

void mining(const perform_mining_message &msg)
{
	region::makeFloor(msg.targetIdx);
}

void channeling(const perform_mining_message &msg)
{
	region::makeEmptySpace(msg.targetIdx);

	Coordinates co = idxToCo(msg.targetIdx);
	if (region::solid(getIdx(CO_DOWN)))
	{
		region::makeRamp(getIdx(CO_DOWN));
	}
}

void ramping(const perform_mining_message &msg)
{
	region::makeRamp(msg.targetIdx);

	Coordinates co = idxToCo(msg.targetIdx);
	if (region::solid(getIdx(CO_UP)))
	{
		region::makeEmptySpace(getIdx(CO_UP));
		region::setFlag(CO_UP, region::Flag::CAN_STAND_HERE);
	}
}

void upStairs(const perform_mining_message &msg)
{
	region::makeUpStair(msg.targetIdx);
}

void downStairs(const perform_mining_message &msg)
{
	region::makeDownStair(msg.targetIdx);
}

void upDownStairs(const perform_mining_message &msg)
{
	region::makeUpDownStair(msg.targetIdx);
}

void spawnMiningProducts(const perform_mining_message &msg)
{
	const auto co = idxToCo(msg.targetIdx);

	const auto matIdx = region::getTileMaterial(co);

	auto* mat = getMaterial(matIdx);

	if (!mat)
		return;

	for(int i = 0; i < mat->minesToAmount; ++i)
		spawnItemOnGround(mat->minesToTag, matIdx, co, SpawnColor::MATERIAL_COLOR);
}

void recalculateArea(const perform_mining_message &msg)
{
	const auto co = idxToCo(msg.targetIdx);

	for(int x = -2; x < 3; ++x)
		for(int y = -2; y < 3; ++y)
			for (int z = -2; z < 3; ++z)
				if (x + co.x > 0 && x + co.x < MAP_WIDTH && y + co.y > 0 && y + co.y < MAP_HEIGHT && z + co.z > 0 && z + co.z < MAP_DEPTH)
				{
					region::tile_recalc({ x + co.x, y + co.y, z + co.z });
				}
}

void RegionHelper::update(const double duration)
{
	bool miningProcessed = false;
	miningMessages.processAll([&miningProcessed](const perform_mining_message &msg)
	{
		switch (msg.opperation)
		{
		case MiningTypes::MINING_MINING:
			mining(msg);
			break;

		case MiningTypes::MINING_CHANNELING:
			channeling(msg);
			break;

		case MiningTypes::MINING_RAMPING:
			ramping(msg);
			break;

		case MiningTypes::MINING_UP_STAIRS:
			upStairs(msg);
			break;

		case MiningTypes::MINING_DOWN_STAIRS:
			downStairs(msg);
			break;

		case MiningTypes::MINING_UP_DOWN_STAIRS:
			upDownStairs(msg);
			break;
		}
		
		recalculateArea(msg);
		spawnMiningProducts(msg);
		miningProcessed = true;
	});

	if (miningProcessed)
	{
		emit(recalculate_mining_message{});
	}
}
