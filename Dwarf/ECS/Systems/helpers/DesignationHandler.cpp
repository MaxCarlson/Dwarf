#include "../stdafx.h"
#include "DesignationHandler.h"
#include "../../Messages/designation_message.h"
#include "../Map/Tile.h"
#include "../Designations.h"
#include "../ECS/Messages/recalculate_mining_message.h"
#include "../ECS/Messages/pick_map_changed_message.h"

void DesignationHandler::init()
{
	subscribe<designation_message>([this](designation_message &msg)
	{
		designate(msg);
	});
}

void DesignationHandler::designate(const designation_message & msg) // Add in checks to make sure improper areas aren't designated
{
	auto co1 = idxToCo(msg.desIdx.first);							// Sepperate functions for differing types of desig checking
	auto co2 = idxToCo(msg.desIdx.second); 
	const auto type = msg.type;

	// Don't allow multi dimensional designations ~~ Possibly allow eventually
	if (co1.z != co2.z)
		return;

	if (co1 == co2) // Is this needed?
	{
		designations->mining[getIdx(co1)] = type;
		return;
	}
	// Flip variables if needed so we can enumerate through loop easily
	if (co1.x > co2.x)
	{
		int tmp = co1.x;
		co1.x = co2.x;
		co2.x = tmp;
	}
	if (co1.y > co2.y)
	{
		int tmp = co1.y;
		co1.y = co2.y;
		co2.y = tmp;
	}

	// Handle chopping designation
	if (msg.type == designation_message::CUT_TREES)
	{
		designateChopping(co1, co2);
	}

	// Handle a mining like designation
	else
	{
		designateMining(type, co1, co2);
	}
}

void DesignationHandler::designateMining(const int type, const Coordinates co1, const Coordinates co2)
{
	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{
			designations->mining[getIdx({ x, y, co1.z })] = type;
		}

	emit(recalculate_mining_message{});
	emit(pick_map_changed_message{});
}

inline int isTree(const Coordinates co) 
{
	int id = region::treeId(getIdx(co));

	// Make sure we only designate a tree tile that is ground level for that tree
	if (id)
	{
		region::ti
	}

	return id;
}

void DesignationHandler::designateChopping(const Coordinates co1, const Coordinates co2)
{
	for (int x = co1.x; x <= co2.x; ++x)
		for (int y = co1.y; y <= co2.y; ++y)
		{

		}
}
