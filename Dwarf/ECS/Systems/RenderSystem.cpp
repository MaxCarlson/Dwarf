#include "RenderSystem.h"
#include "../Messages/render_changed_message.h"
#include "EntityPositionCache.h"
#include "../Components/Building.h"
#include "../Components/PositionComponent.h"
#include "../Raws/Materials.h"
#include "../Raws/Defs/MaterialDef.h"
#include "../Map/Tile.h"
#include "../Drawing/vchar.h"
#include "Globals\Camera.h"
#include "Globals\game_states.h"
#include "Globals\GlobalTerminals.h"
#include "dfr\DwarfRender.h"
#include <imgui.h>
#include <imgui-SFML.h>

std::unordered_map<int, std::vector<RenderComponent>> renderEntities;

void RenderSystem::init()
{
	subscribe_mbox<render_changed_message>();
}

const static color_t defaultColor = color_from_name("black");

/*
vchar getTileRender(const Coordinates co)
{
	const vchar& v = region::getRenderTile(co); // For 3D rendering once we implement lighting

	//const vchar& v = region::renderCache(getIdx(co));

	auto rendIt = renderEntities.find((dfr::terminal->width * co.y) + co.x);
	
	if (rendIt != renderEntities.end())
	{
		const auto& rend = rendIt->second[0]; // Add cycling through glyphs every once in a while if multiple on tile

		return rend.ch;
	}

	return v;
}
*/

void applyHaze(vchars& ch, int z, int chZ)
{
	int heightDiff = std::min(z - chZ, 5);

	//float amount = 0.15 * heightDiff;
	float amount = std::pow(heightDiff, 0.25) - 0.75;

	ch.fg = lerp(ch.fg, color{ 0x004561 }, amount);
}

std::pair<vchar, int> getTileRender(const Coordinates co)
{
	auto [v, z] = region::getRenderTile(co); // For 3D rendering once we implement lighting

	//const vchar& v = region::renderCache(getIdx(co));

	auto rendIt = renderEntities.find((dfr::terminal->width * co.y) + co.x);

	if (rendIt != renderEntities.end())
	{
		const auto& rend = rendIt->second[0]; // Add cycling through glyphs every once in a while if multiple on tile

		return { rend.ch, z };
	}

	return { v, z };
}

void RenderSystem::update(const double duration)
{

	int z = camera.z;

	updateRender(); // Remove the need to always do this

	int maxX = std::min(MAP_WIDTH,  dfr::terminal->width);
	int maxY = std::min(MAP_HEIGHT, dfr::terminal->height); 

	for(int x = 0; x < maxX; ++x)
		for (int y = 0; y < maxY; ++y)
		{
			auto [rend, chZ] = getTileRender({ x, y, z });
			
			
			// If the character z level is lower than the camera,
			// apply a hazing effect
			vchars tmp{ static_cast<uint32_t>(rend.c), rend.fg, rend.bg };
			applyHaze(tmp, z, chZ);

			dfr::terminal->setChar(x + camera.offsetX, y + camera.offsetY, tmp);
		}
}

void RenderSystem::updateRender()
{
	/*
	std::queue<render_changed_message> * rc = mbox<render_changed_message>(); 
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

		/*
		const int terminal_width = terminal_state(TK_WIDTH);

		const int minX = engine->mapRenderer->offsetX;
		const int maxX = terminal_width + minX;
		const int minY = engine->mapRenderer->offsetY;
		const int maxY = terminal_state(TK_HEIGHT) + minY;
		*/

		const int zlvl = camera.z; 

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

			// 2D Idxing 
			const int idx = (dfr::terminal->width * pos->co.y) + pos->co.x;

			// Render buildings code
			if (b && pos && !rendered)
			{
				int glyphIdx = 0;
				int boffsetX = 0;
				int boffsetY = 0;

				// Offset for 3x3 buildings
				if (b->width == 3)
					boffsetX = -1;
				if (b->height == 3)
					boffsetY = -1;

				for (int y = 0; y < b->height; ++y)
				{
					for (int x = 0; x < b->width; ++x)
					{
						const int idx = (dfr::terminal->width * (pos->co.y + boffsetY)) + pos->co.x + boffsetX; //getIdx({ pos->co.x + boffsetX, pos->co.y + boffsetY, pos->co.z });

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

						renderEntities[idx].push_back(vchar{ glyph, color_from_name(color.c_str()), color_from_name("black") });
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
				renderEntities[idx].push_back({ rend->ch });

				rendered = true;
			}
		}
	}
}

void RenderSystem::renderDesignations()
{
	
}
