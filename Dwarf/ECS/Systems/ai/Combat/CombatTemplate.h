#pragma once
#include "ECS\Components\MovementComponent.h"
#include "Coordinates.h"
#include "Map\Tile.h"
#include "ECS\Messages\entity_moved_message.h"
#include "ECS\Systems\helpers\PathFinding.h"
#include "ECS\Components\PositionComponent.h"
#include "Globals\GlobalWorld.h"

template<typename Tag>
class CombatTemplate
{
public:

	template<typename SUCCESS, typename CANCEL>
	void getCloseToMovingThing(const Entity &e, MovementComponent &mov, const Coordinates &co, size_t followId, double distance, const SUCCESS &success, const CANCEL &cancel)
	{
		if (mov.progress)
			return;

		const auto& fid = world.getEntity(followId);

		if (!fid.isValid() || !fid.hasComponent<PositionComponent>())
		{
			cancel();
			return;
		}

		const auto& fCo = fid.getComponent<PositionComponent>().co;

		// If we're close enough to the entity we're following to do success
		if (get_3D_distance(co, fCo) < distance)
		{
			success();
			return;
		}

		// If the Entity we're trying to get close to is not at where out path ends
		if (mov.path.begin() != fCo)
		{
			auto path = findPath(co, fCo);

			if (!path->failed)
			{
				mov.path = path->path;
			}
			else
			{
				cancel();
				return;
			}
		}
	}


	void cancelCombat(const Entity &e)
	{
		e.removeComponent<Tag>();
		e.activate();
	}
};