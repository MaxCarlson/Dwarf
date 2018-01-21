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

struct RenderItem
{
	RenderItem() = default;
	RenderItem(int ch, uint8_t code, color_t fg, color_t bg)
		: ch(ch), code(code), fg(fg), bg(bg) {}

	int ch;
	uint8_t code;
	color_t fg;
	color_t bg;
};

std::unordered_map<int, std::vector<RenderComponent>> renderEntities;

void RenderSystem::init()
{
	subscribe_mbox<render_changed_message>();
}

const static color_t defaultColor = color_from_name("black");

// Not in use
const RenderItem RenderSystem::getTileRender(const Coordinates co)
{
	const int idx = getIdx(co);

	const vchar& v = region::renderCache(idx);

	RenderItem r = { v.c, 2, v.fg, v.bg };

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
	//const vchar& v = region::getRenderTile(co); // For 3D rendering once we implement lighting

	const vchar& v = region::renderCache(getIdx(co));

	auto rendIt = renderEntities.find((terminal_state(TK_WIDTH) * co.y) + co.x);

	vchar r;

	if (rendIt != renderEntities.end())
	{
		const auto& rend = rendIt->second[0]; // Add cycling through glyphs every once in a while if multiple on tile

		r = { rend.ch, color_from_name(rend.colorStr.c_str()), defaultColor };
	}
	else 
		r = { v.c, v.fg, v.bg };

	return r;
}

void RenderSystem::update()
{	
	static const int codes[] = { 0xE000, 0xE100, 0xE200, 0xE300, 0xE400, 0xE500, 0xE600, 0xE700, 0xE800 };

	const auto& entities = getEntities();

	const int offsetX = engine->mapRenderer->offsetX; 
	const int offsetY = engine->mapRenderer->offsetY;
	const int maxX = (terminal_state(TK_WIDTH)  + offsetX) <= MAP_WIDTH  ? (terminal_state(TK_WIDTH)  + offsetX) : MAP_WIDTH;
	const int maxY = (terminal_state(TK_HEIGHT) + offsetY) <= MAP_HEIGHT ? (terminal_state(TK_HEIGHT) + offsetY) : MAP_HEIGHT;

	terminal_color("default");

	updateRender();

	int z = engine->mapRenderer->currentZLevel;
	int X = 0;
	for (int x = offsetX; x < maxX; ++x)
	{
		int Y = 0;
		for (int y = offsetY; y < maxY; ++y)
		{
			//auto rend = getTileRender({ x, y, z });
			auto rend = getTR({ x, y, z });

			terminal_color(rend.fg);
			terminal_bkcolor(rend.bg);

			int calcChar = rend.c / 255;

			terminal_put(X, Y, codes[calcChar + 1] + ( rend.c - calcChar * 256));
			++Y;
		}
		++X;
	}
	terminal_bkcolor("black");
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

		const int terminal_width = terminal_state(TK_WIDTH);

		const int minX = engine->mapRenderer->offsetX;
		const int maxX = terminal_width + minX;
		const int minY = engine->mapRenderer->offsetY;
		const int maxY = terminal_state(TK_HEIGHT) + minY;


		const int zlvl = engine->mapRenderer->currentZLevel;

		//std::vector<std::size_t> visible = positionCache->find_by_region(minX, maxX, maxY, minY, zlvl, zlvl - 10); When we want to render more than just one zLvl

		for (auto e : getEntities())
		{
			// Don't show entities not on camera level (for the moment)
			// Eventually want to implement lighting to show multiple z levels
			auto* pos = &e.getComponent<PositionComponent>();
			if (!pos || pos->co.z != zlvl)
				continue;

			else if (!e.isValid()) // Should almost never be the case, here for debug
			{
				std::cout << "Entity " << e.getId().index << " is not valid (In RendeSystem)!! \n";
				continue;
			}

			bool rendered = false;

			auto* b = &e.getComponent<Building>();
			auto* rend = &e.getComponent<RenderComponent>();

			//if (!pos || !rend) // Already implicit in being part of this system
			//	continue;

			// 2D Idxing 
			const int idx = (terminal_width * pos->co.y) + pos->co.x;

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
						const int idx = (terminal_width * (pos->co.y + boffsetY)) + pos->co.x + boffsetX; //getIdx({ pos->co.x + boffsetX, pos->co.y + boffsetY, pos->co.z });

						int glyph = b->charCodes[glyphIdx++];

						std::string color; // Convert all colors to color_t's

						if (b->complete)
							if (getMaterial(b->materials[0].first))
							{
								color = getMaterial(b->materials[0].first)->color; // Should color rely on materials? This needs to be enhanced instead of relying on just first material
							}
							else
								color = "grey";
							
						else
							color = "grey";

						renderEntities[idx].push_back({ glyph, 2, color });
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
