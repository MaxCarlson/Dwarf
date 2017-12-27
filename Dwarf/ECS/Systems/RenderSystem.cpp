#include "RenderSystem.h"
#include "../Messages/render_changed_message.h"

std::unordered_map<int, RenderComponent> renderEntities;

void RenderSystem::init()
{
	subscribe_mbox<render_changed_message>();
}

void RenderSystem::update()
{
	std::queue<render_changed_message> * rc = mbox<render_changed_message>(); // Delete this?

	while (!rc->empty()) // And this? Render is going to be changing quite a bit
	{
		renderChanged = true;
		rc->pop();
	}
}
