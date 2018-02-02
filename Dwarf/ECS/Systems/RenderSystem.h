#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"
#include "../BearLibTerminal.h"

struct vchar;
struct RenderItem;

// Render all entities that are within camera fov/zlevel
// and are visible
class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;

	void init();

	void update(const double duration);

	void updateRender();



private:

	bool renderChanged = true;
};