#include "RenderSystem.h"
#include "../Messages/render_changed_message.h"
#include "EntityPositionCache.h"
#include "../Components/Building.h"
#include "../Components/PositionComponent.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../Map/Tile.h"
#include "../Drawing/vchar.h"
// Add these to systems so they're not random obj's
#include "../Engine.h"
#include "../Map/MapRender.h"

std::unordered_map<int, std::vector<RenderComponent>> renderEntities;

void RenderSystem::init()
{
	subscribe_mbox<render_changed_message>();
}

const static color_t defaultColor = color_from_name("black");

const RenderItem RenderSystem::getTileRender(const Coordinates co)
{
	const int idx = getIdx(co);

	const vchar& v = region::renderCache(idx);

	RenderItem r = { v.c, 2, v.fg, defaultColor };

	auto ids = positionCache->get_location(idx);

	if (ids.size() > 0)
	{
		for (const auto& id : ids)
		{
			if (getWorld().getEntity(id).hasComponent<RenderComponent>())
			{
				const auto& rend = getWorld().getEntity(id).getComponent<RenderComponent>();

				r = { rend.ch, rend.terminalCode,  color_from_name(rend.colorStr.c_str()), defaultColor };
				break;
			}
		}
	}

	return r;
}

const vchar getTR(const Coordinates co)
{
	vchar v = region::getRenderTile(co);
	auto rendIt = renderEntities.find((terminal_state(TK_WIDTH) * co.y) + co.x);

	if (rendIt != renderEntities.end())
	{
		auto rend = rendIt->second[0]; // Add cycling through glyphs every once in a while if multiple on tile

		v = { rend.ch, color_from_name(rend.colorStr.c_str()), color_from_name("black") };
	}


	return v;
}

void RenderSystem::update()
{	
	static const int codes[] = { 0xE000, 0xE100, 0xE200, 0xE300, 0xE400, 0xE500, 0xE600, 0xE700, 0xE800 };

	const auto& entities = getEntities();

	const int offsetX = engine->mapRenderer->offsetX; 
	const int offsetY = engine->mapRenderer->offsetY;
	const int maxX = (terminal_state(TK_WIDTH) + offsetX) <= MAP_WIDTH ? (terminal_state(TK_WIDTH) + offsetX) : MAP_WIDTH;
	const int maxY = (terminal_state(TK_HEIGHT) + offsetY) <= MAP_HEIGHT ? (terminal_state(TK_HEIGHT) + offsetY) : MAP_HEIGHT;

	terminal_color("default");

	
	int z = engine->mapRenderer->currentZLevel;
	int X = 0;
	for (int x = offsetX; x < maxX; ++x)
	{
		int Y = 0;
		for (int y = offsetY; y < maxY; ++y)
		{
			auto rend = getTileRender({ x, y, z });

			terminal_color(rend.fg);
			terminal_put(X, Y, codes[rend.code] + rend.ch);
			++Y;
		}
		++X;
	}
	
	

	/* //3D rendering, need to implement lighting for it to look okay

	updateRender(); // Remove this and add render_changed_message s ?

	// Render map tiles
	// Possibly move this to it's own function?
	int z = engine->mapRenderer->currentZLevel;
	int X = 0;
	for (int x = offsetX; x < maxX; ++x)
	{
		int Y = 0;
		for (int y = offsetY; y < maxY; ++y)
		{
			auto rend = getTR({ x, y, z });

			//terminal_bkcolor(rend.bg.c_str());
			terminal_color(rend.fg.c_str());
			terminal_put(X, Y, 0xE200 + rend.c);
			++Y;
		}
		++X;
	}
	*/
}

void RenderSystem::updateRender()
{
	/*
	std::queue<render_changed_message> * rc = mbox<render_changed_message>(); // Delete this?

	while (rc && !rc->empty()) // And this? Render is going to be changing quite a bit
	{
		renderChanged = true;
		rc->pop();
	}
*/
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

			const int idx = (terminal_state(TK_WIDTH) * pos->co.y) + pos->co.x;

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
