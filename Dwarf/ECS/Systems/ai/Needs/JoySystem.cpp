#include "stdafx.h"
#include "JoySystem.h"
#include "..\WorkTemplate.h"
#include "..\JobBoard.h"

namespace JobsBoard
{
	void evaluate_joy(JobBoard & board, const Entity & e, AiWorkComponent &prefs, const Coordinates& co, JobEvaluatorBase * jt) // TODO: Require entity be in idle time unless joy is <= 0.0
	{

	}
}


void JoySystem::init()
{
	JobsBoard::register_job_offer<JoyTag>(JobsBoard::evaluate_joy);
}

inline void findSource(const Entity &e, const Coordinates& co, JoyTag &tag, WorkTemplate<JoyTag> &work, MovementComponent &mov);
inline void gotoSource(const Entity &e, const Coordinates& co, JoyTag &tag, WorkTemplate<JoyTag> &work, MovementComponent &mov);
inline void workJoy(const Entity &e, const Coordinates& co, JoyTag &tag, WorkTemplate<JoyTag> &work, MovementComponent &mov);


void JoySystem::update(const double duration)
{
	for (const auto& e : getEntities())
	{
		WorkTemplate<JoyTag> work;

		const auto& co = e.getComponent<PositionComponent>().co;
		auto& tag = e.getComponent<JoyTag>();
		auto& mov = e.getComponent<MovementComponent>();

		switch (tag.step)
		{
		case JoyTag::FIND_SOURCE:

			break;

		case JoyTag::GOTO_SOURCE:

			break;

		case JoyTag::PLAY:

			break;
		}
	}
}

inline void findSource(const Entity & e, const Coordinates & co, JoyTag & tag, WorkTemplate<JoyTag>& work, MovementComponent & mov)
{
}

inline void gotoSource(const Entity & e, const Coordinates & co, JoyTag & tag, WorkTemplate<JoyTag>& work, MovementComponent & mov)
{
}

inline void workJoy(const Entity & e, const Coordinates & co, JoyTag & tag, WorkTemplate<JoyTag>& work, MovementComponent & mov)
{
}
