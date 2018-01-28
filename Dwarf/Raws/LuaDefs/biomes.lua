

biome_types = {
	oceans = 1,
	plains = 2,
	hills = 3,
	mountains = 4,
	highlands = 5,
	plateaus = 6,
	coasts = 7,
}

temps = {
	vcold = { minTemp = -100, maxTemp = 3 }
}

biomes = {
	
	-- Plains
	frozen_plains = {
		name = "Frozen Plains", minTemp = -100, maxTemp = 3, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "light ice" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 5, evergreen = 7 }
	},

	temperate_plains = {
		name = "Arid Plains", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "wheat" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 15, evergreen = 4 }
	},

	grassy_plains = {
		name = "Grassy Plains", minTemp = 0, maxTemp = 25, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "light grass_green" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 15, evergreen = 4 }
	},

	savannah_plains = {
		name = "Savannah Plains", minTemp = 25, maxTemp = 100, minRain = 0, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 55, sand = 45 },
		rend   = { glyph = glyphs['grass'], color = "wheat" },
		plants = { grass = 25 },
		trees  = { deciduous = 7, evergreen = 0 }
	},

	-- Hills
	frozen_hills = {
		name = "Frozen Hills", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 15 }
	},

	frozen_hills = {
		name = "Frozen Hills", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 15 }
	},

	-- Mountains

	-- Highlands

	-- Plateaus


	-- Forests

	-- Rainforests
	temperate_rainforest = {
		name = "Temperate Rainforest", minTemp = -10, maxTemp = 85, minRain = 25, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
		soils  = { soil = 95, sand = 5 },
		rend   = { glyph = glyphs['ever_tree'], color = "dark forest_green" },
		plants = { grass = 20, blackberry = 2, plump_helmet = 2 },
		trees  = { deciduous = 10, evergreen = 55 }
	},

	tropical_rainforest = {
		name = "Tropical Rainforst", minTemp = 55, maxTemp = 100, minRain = 25, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
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
		name = "Temperate Ocean", minTemp = -10, maxTemp = 35, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	ocean = {
		name = "Ocean", minTemp = 7, maxTemp = 55, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	tropical_ocean = {
		name = "Tropical Ocean", minTemp = 40, maxTemp = 100, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	-- Coasts
	
	-- Deserts
	tundra = {
		name = "Tundra", minTemp = -100, maxTemp = 12, minRain = 5, maxRain = 95,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 75, sand = 25 },
		rend   = { glyph = glyphs['grass'], color = "light ice" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 2, evergreen = 4 }
	},

	temperate_desert = {
		name = "Desert", minTemp = 32, maxTemp = 100, minRain = 0, maxRain = 18,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"], biome_types["plateaus"], biome_types["highlands"] },
		soils = { soil = 0, sand = 100 },
		rend = { glyph = glyphs['sand1'], color = "light wheat" },
		plants = { grass = 25 },
		trees  = { deciduous = 2, evergreen = 1 }
	},

	tropical_desert = {
		name = "Desert", minTemp = 47, maxTemp = 100, minRain = 0, maxRain = 10,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"] },
		soils = { soil = 0, sand = 100 },
		rend = { glyph = glyphs['sand1'], color = "light wheat" },
		plants = { grass = 15 },
		trees  = { deciduous = 2, evergreen = 1 }
	},
};