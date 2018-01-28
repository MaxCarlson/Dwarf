
biome_chars = {
	


}

biome_types = {
	oceans = 1,
	plains = 2,
	hills = 3,
	mountains = 4,
	highlands = 5,
	plateau = 6,
	coast = 7,
}

biomes = {
	
	-- Plains
	tundra = {
		name = "Tundra", minTemp = -100, maxTemp = 32, minRain = 5, maxRain = 95,
		occurs = { biome_types["plains"], biome_types["coast"], biome_types["plateau"] },
		soils  = { soil = 75, sand = 25 },
		rend   = { glyph = glyphs['grass'], color = "light ice" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 2, evergreen = 4 }
	},

	grassy_plains = {
		name = "Grassy Plains", minTemp = -15, maxTemp = 95, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coast"], biome_types["plateau"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "light grass_green" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 30, evergreen = 5 }
	},

	savannah = {
		name = "Savannah", minTemp = 55, maxTemp = 100, minRain = 0, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coast"], biome_types["plateau"] },
		soils = { soil = 55, sand = 45 },
		rend = { glyph = glyphs['grass'], color = "wheat" },
		plants = { grass = 25 },
		trees  = { deciduous = 7, evergreen = 0 }
	},



	-- Forests

	-- Rainforests
	temperate_rainforest = {
		name = "Temperate Rainforest", minTemp = -10, maxTemp = 85, minRain = 25, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coast"] },
		soils  = { soil = 95, sand = 5 },
		rend   = { glyph = glyphs['ever_tree'], color = "dark forest_green" },
		plants = { grass = 20, blackberry = 2, plump_helmet = 2 },
		trees  = { deciduous = 10, evergreen = 55 }
	},

	tropical_rainforest = {
		name = "Tropical Rainforst", minTemp = 55, maxTemp = 100, minRain = 25, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coast"] },
		soils = { soil = 93, sand = 7 },
		rend = { glyph = glyphs['deci_tree'], color = "forest_green" },
		plants = { grass = 15, plump_helmet = 2, },
		trees = { deciduous = 55, evergreen = 1 }
	},

	-- Oceans
	sea_ice = {
		name = "Sea Ice", minTemp = -100, maxTemp = -10, minRain = 0, maxRain = 55,
		occurs = { biome_types["oceans"] },
		soils  = { soil = 0, sand = 100 },
		rend   = { glyph = glyphs['water'], color = "dark ice" },  
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	temperate_ocean = {
		name = "Temperate Ocean", minTemp = -10, maxTemp = 45, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		rend = { glyph = glyphs['water'], color = "light blue" },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	-- Coasts
	
};