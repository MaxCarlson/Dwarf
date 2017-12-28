#pragma once

#include "../Systems.h"
#include "../Components/PositionComponent.h"
#include "../Components/RenderComponent.h"
#include "../BearLibTerminal.h"

struct vchar;


struct RenderItem
{
	RenderItem(int ch, uint8_t code, color_t fg, color_t bg)
		: ch(ch), code(code), fg(fg), bg(bg) {}

	int ch;
	uint8_t code;
	color_t fg;
	color_t bg;
};

// Render all entities that are within camera fov/zlevel
// and are visible
class RenderSystem : public System<Requires<PositionComponent, RenderComponent>>
{
public:
	RenderSystem() = default;

	void init();

	void update();

	void updateRender();



private:

	bool renderChanged = true;

	const RenderItem getTileRender(const Coordinates co);
};