#include "RenderSystem.h"
#include "../Messages/render_changed_message.h"
#include "EntityPositionCache.h"
#include "../Components/Building.h"
#include "../Components/PositionComponent.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"

// Add these to systems so they're not random obj's
#include "../Engine.h"
#include "../Map/MapRender.h"

std::unordered_map<int, std::vector<RenderComponent>> renderEntities;

void RenderSystem::init()
{
	subscribe_mbox<render_changed_message>();
}

void RenderSystem::update()
{	

	const auto& entities = getEntities();

	int offsetX = engine->mapRenderer->offsetX; 
	int offsetY = engine->mapRenderer->offsetY;

	terminal_color("default");

	for (const auto& rend : renderEntities)
	{
		
	}


	/*
	for (const auto& e : entities)
	{
		auto co = e.getComponent<PositionComponent>().co;

		// Adjust copy of position coordinates to be aligned with camera
		// coordinates
		co.x -= offsetX;
		co.y -= offsetY;

		// If the camera z level matches our entities
		// zLevel, render it!
		// (Eventually add opacity to non obscured Entities below camera z Level)
		if (isInCameraRange(co))
		{
			const auto& rend = e.getComponent<RenderComponent>();

			const char* cstr = rend.colorStr.c_str();

			// Switch terminal color to item color
			terminal_color(cstr);

			// Draw tile image, from tile set associated with rend.terminalcode
			// with location in tileset being rend.ch
			terminal_put(co.x, co.y, terminalCodes[rend.terminalCode] + rend.ch);
		}
	}
	*/
}

void RenderSystem::updateRender()
{
	std::queue<render_changed_message> * rc = mbox<render_changed_message>(); // Delete this?

	while (!rc->empty()) // And this? Render is going to be changing quite a bit
	{
		renderChanged = true;
		rc->pop();
	}

	renderChanged = true; // delete this and add in render_changed_messages ? Only once needed?

	if (renderChanged)
	{
		renderEntities.clear();

		const int minX = engine->mapRenderer->offsetX;
		const int maxX = terminal_state(TK_WIDTH) + minX;
		const int minY = engine->mapRenderer->offsetY;
		const int maxY = terminal_state(TK_HEIGHT) + minY;


		const int zlvl = engine->mapRenderer->currentZLevel;

		std::vector<std::size_t> visible = positionCache->find_by_region(minX, maxX, maxY, minY, zlvl, zlvl - 10);

		auto &world = getWorld();

		for (auto id : visible)
		{
			auto& e = world.getEntity(id);

			if (!e.isValid())
			{
				std::cout << "Entity " << e.getId().index << " is not valid (In RendeSystem)!! \n";
				continue;
			}

			bool rendered = false;

			auto* b = &e.getComponent<Building>();
			auto* pos = &e.getComponent<PositionComponent>();
			auto* rend = &e.getComponent<RenderComponent>();

			if (!pos || !rend)
				continue;

			const int idx = getIdx(pos->co);

			// Render buildings code
			if (b && pos && !rendered)
			{
				int glyphIdx = 0;
				int boffsetX = 0;
				int boffsetY = 0;

				if (b->width == 3)
					boffsetX = -1;
				if (b->height == 3)
					boffsetY = -1;

				for (int y = 0; y < b->height; ++y)
				{
					for (int x = 0; x < b->width; ++x)
					{

						const int idx = getIdx({ pos->co.x + boffsetX, pos->co.y + boffsetY, pos->co.z });

						int glyph = b->charCodes[glyphIdx++];

						std::string color;

						if (b->complete)
							color = getMaterial(b->materials[0].first)->color; // Should color rely on materials? This needs to be enhanced instead of relying on just first material
						else
							color = "grey";

						renderEntities[idx].push_back({ glyph, rend->terminalCode, color });
						++glyphIdx;
						++boffsetX;
					}
					boffsetX = 0;
					if (b->width == 3)
						boffsetX = -1;

					++boffsetY;
				}
				rendered = true;
			}

			// Non building-Entity rendering
			if (!rendered)
			{
				renderEntities[idx].push_back({ rend->ch, rend->terminalCode, rend->colorStr });

				rendered = true;
			}
		}
	}


}
