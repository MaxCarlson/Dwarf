#include "stdafx.h"
#include "FarmingAi.h"
#include "JobBoard.h"
#include "Map\Tile.h"
#include "Designations.h"
#include "Raws\ReadPlants.h"
#include "Raws\Defs\PlantDef.h"
#include "../DijkstraSystems/DijkstraMapsHandler.h"

static const std::string jobSkill = "farming";

void SeedsHelper::forAllSeeds(std::function<void(Entity)> func)
{
	for (const auto& e : getEntities())
		func(e);
}

namespace JobsBoard
{
	void evaluate_farming(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (designations->planting.empty())
			return;

		
	}
}

void FarmingAi::init()
{
}

void FarmingAi::update(const double duration)
{
	for (auto e : getEntities())
		doWork(e);
}

void FarmingAi::doWork(Entity & e)
{

}

