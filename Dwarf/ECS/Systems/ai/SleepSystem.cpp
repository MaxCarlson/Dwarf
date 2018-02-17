#include "stdafx.h"
#include "SleepSystem.h"
#include "WorkTemplate.h"
#include "JobBoard.h"
#include "Designations.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Systems\helpers\BuildingHelper.h"
#include "ECS\Components\Claimed.h"

namespace JobsBoard
{
	void evaluateSleep(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt)
	{
		if (!e.hasComponent<Needs>())
			return;

		auto& sleep = e.getComponent<Needs>().needs[static_cast<int>(NeedIdx::SLEEP)];

		int priority = 0;
		if (sleep.lvl > SleepThreshold::SLEEPY)
			priority = SleepPriority::RESTED;

		else if (sleep.lvl > SleepThreshold::TIRED)
			priority = SleepPriority::SLEEPY;

		else if (sleep.lvl > SleepThreshold::VERY_TIRED)
			priority = SleepPriority::TIRED;

		else if (sleep.lvl > SleepThreshold::EXHAUSTED)
			priority = SleepPriority::VERY_TIRED;

		else
			priority = SleepPriority::EXHAUSTED;

		int distance = 0;

		board.emplace(std::make_pair(priority, JobsBoard::JobRating{ distance, jt }));
	}
}

void SleepSystem::init()
{
	JobsBoard::register_job_offer<SleepTag>(JobsBoard::evaluateSleep);
}

inline void findBed(const Entity &e, WorkTemplate<SleepTag> &work, MovementComponent &mov, SleepTag &tag, const Coordinates& co);
inline void gotoBed(const Entity &e, WorkTemplate<SleepTag> &work, MovementComponent &mov, SleepTag &tag, const Coordinates& co);
inline void doSleep(const Entity &e, WorkTemplate<SleepTag> &work, const double &duration, SleepTag &tag, const Coordinates& co);

//MessageQueue<WakeUpMessage> wakeups

void SleepSystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<SleepTag> work;

		const auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<SleepTag>();
		auto& mov = e.getComponent<MovementComponent>();


		switch (tag.step)
		{
		case SleepTag::FIND_BED:
			findBed(e, work, mov, tag, co);
			break;

		case SleepTag::GOTO_BED:
			gotoBed(e, work, mov, tag, co);
			break;

		case SleepTag::SLEEP:
			doSleep(e, work, duration, tag, co);
			break;
		}
	}
}

inline void findBed(const Entity &e, WorkTemplate<SleepTag>& work, MovementComponent &mov, SleepTag & tag, const Coordinates & co)
{
	auto myBed = designations->beds.find(e.getId().index);

	// I have a designated bed and found a path
	if (myBed != designations->beds.end())
	{
		auto path = findPath(co, myBed->second);

		if (!path->failed)
		{
			tag.bedCo = myBed->second;
			tag.step = SleepTag::GOTO_BED;
			mov.path = path->path;
			return;
		}
	}

	// Find closest unclaimed bed
	std::map<int, size_t> bedDist;
	buildingHelper.forEachBuildingOfType<PROVIDES_SLEEP>([&bedDist, &co](const Entity &b)
	{
		if (b.hasComponent<Claimed>() || !b.hasComponent<PositionComponent>())
			return;

		auto& bedCo = b.getComponent<PositionComponent>().co;
		const auto dist = static_cast<int>(get_3D_distance(co, bedCo));

		bedDist.emplace(std::make_pair(dist, b.getId().index));
	});


	// If exhausted find a floor to sleep on
}

inline void gotoBed(const Entity &e, WorkTemplate<SleepTag>& work, MovementComponent &mov, SleepTag & tag, const Coordinates & co)
{
	work.followPath(mov, co, tag.bedCo, [&tag]()
	{
		// On path failure
		tag.step = SleepTag::FIND_BED;

	}, [&tag]
	{
		// On reaching bed
		tag.step = SleepTag::SLEEP;
	});
}

inline void doSleep(const Entity &e, WorkTemplate<SleepTag>& work, const double& duration, SleepTag & tag, const Coordinates & co)
{
	constexpr int sleepIdx = static_cast<int>(NeedIdx::SLEEP);

	auto& sleep = e.getComponent<Needs>().needs[sleepIdx];

	// TODO:
	// Multiply recovery fraction by material sleep bonus
	// Multiply recovery by quality of bed
	// Add in bad thoughts for loud noises
	// Bonuses thoughts for very high quality beds

	// If I do not own the bed unclaim it upon wake up

	sleep.lvl += duration / 1000;

	if (sleep.lvl == 1000.0)
	{
		work.cancel_work(e);
		return;
	}
}
