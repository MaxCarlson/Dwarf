

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
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 5, evergreen = 7 },
		rend   = { glyph = glyphs['grass'], color = "light ice" },
	},

	temperate_plains = {
		name = "Arid Plains", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 85, sand = 15 },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 15, evergreen = 4 },
	},

	grassy_plains = {
		name = "Grassy Plains", minTemp = 0, maxTemp = 25, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 99, sand = 1 },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 15, evergreen = 4 },
		rend   = { glyph = glyphs['grass'], color = "light grass_green" },
	},

	savannah_plains = {
		name = "Savannah Plains", minTemp = 25, maxTemp = 100, minRain = 0, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 25 },
		trees  = { deciduous = 7, evergreen = 0 },
		rend   = { glyph = glyphs['grass'], color = "wheat" },
	},

	-- Hills
	frozen_hills = {
		name = "Frozen Hills", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 1 }
	},

	temperate_hills = {
		name = "Temperate Hills", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 3 }
	},

	grassy_hills = {
		name = "Grassy Hills", minTemp = 5, maxTemp = 25, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 4, evergreen = 2 }
	},

	savannah_hills = {
		name = "Savannah Hills", minTemp = 25, maxTemp = 55, minRain = 0, maxRain = 100,
		occurs = { biome_types["hills"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 4, evergreen = 2 }
	},

	-- Mountains

	frozen_mountains = {
		name = "Frozen Mountains", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["mountains"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 1 }
	},

	temperate_mountains = {
		name = "Temperate Mountains", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["mountains"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 0, evergreen = 3 }
	},

	grassy_mountains = {
		name = "Grassy Mountains", minTemp = 5, maxTemp = 25, minRain = 0, maxRain = 100,
		occurs = { biome_types["mountains"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 3, evergreen = 8 }
	},

	savannah_mountains = {
		name = "Savannah Mountains", minTemp = 25, maxTemp = 55, minRain = 0, maxRain = 100,
		occurs = { biome_types["mountains"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 4, evergreen = 2 }
	},

	-- Highlands
	
	frozen_highlands = {
		name = "Frozen Highlands", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["highlands"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 3 },
		trees  = { deciduous = 0, evergreen = 1 }
	},

	temperate_highlands = {
		name = "Temperate Highlands", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["highlands"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 8 },
		trees  = { deciduous = 0, evergreen = 3 }
	},

	grassy_highlands = {
		name = "Grassy Highlands", minTemp = 5, maxTemp = 25, minRain = 0, maxRain = 100,
		occurs = { biome_types["highlands"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 3, evergreen = 8 }
	},

	savannah_highlands = {
		name = "Savannah Highlands", minTemp = 25, maxTemp = 55, minRain = 0, maxRain = 100,
		occurs = { biome_types["highlands"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 11 },
		trees  = { deciduous = 4, evergreen = 0 }
	},

	-- Plateaus

	frozen_plateaus = {
		name = "Frozen Plateaus", minTemp = -100, maxTemp = 3, minRain = 0, maxRain = 100,
		occurs = { biome_types["plateaus"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 3 },
		trees  = { deciduous = 0, evergreen = 1 }
	},

	temperate_plateaus = {
		name = "Temperate Plateaus", minTemp = -5, maxTemp = 5, minRain = 0, maxRain = 100,
		occurs = { biome_types["plateaus"] },
		soils   = { soil = 75, sand = 25 },
		plants = { grass = 8 },
		trees  = { deciduous = 0, evergreen = 3 }
	},

	grassy_plateaus = {
		name = "Grassy Plateaus", minTemp = 5, maxTemp = 25, minRain = 0, maxRain = 100,
		occurs = { biome_types["plateaus"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 12 },
		trees  = { deciduous = 3, evergreen = 8 }
	},

	savannah_plateaus = {
		name = "Savannah Plateaus", minTemp = 25, maxTemp = 55, minRain = 0, maxRain = 100,
		occurs = { biome_types["plateaus"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 11 },
		trees  = { deciduous = 4, evergreen = 0 }
	},

	-- Oceans
	sea_ice = {
		name = "Sea Ice", minTemp = -100, maxTemp = -10, minRain = 0, maxRain = 55,
		occurs = { biome_types["oceans"] },
		soils  = { soil = 0, sand = 100 },  
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1},
		rend   = { glyph = glyphs['water'], color = "dark ice" },
	},

	temperate_ocean = {
		name = "Temperate Ocean", minTemp = -10, maxTemp = 10, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	ocean = {
		name = "Ocean", minTemp = 10, maxTemp = 30, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	tropical_ocean = {
		name = "Tropical Ocean", minTemp = 30, maxTemp = 100, minRain = 0, maxRain = 100,
		occurs = { biome_types["oceans"] },
		soils = { soil = 5, sand = 95 },
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	-- Coasts

	-- Forests

	temperate_forest = {
		name = "Temperate Forest", minTemp = -10, maxTemp = 85, minRain = 13, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
		soils  = { soil = 95, sand = 5 },
		plants = { grass = 20, blackberry = 2, plump_helmet = 2 },
		trees  = { deciduous = 5, evergreen = 45 },
		rend   = { glyph = glyphs['ever_tree'], color = "dark forest_green" },
	},

	tropical_forest = {
		name = "Tropical Forest", minTemp = 30, maxTemp = 100, minRain = 14, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
		soils = { soil = 93, sand = 7 },
		plants = { grass = 15, plump_helmet = 2, },
		trees = { deciduous = 55, evergreen = 0 },
		rend = { glyph = glyphs['deci_tree'], color = "forest_green" },
	},

	-- Rainforests
	temperate_rainforest = {
		name = "Temperate Rainforest", minTemp = -10, maxTemp = 85, minRain = 22, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
		soils  = { soil = 95, sand = 5 },
		plants = { grass = 20, blackberry = 2, plump_helmet = 2 },
		trees  = { deciduous = 5, evergreen = 45 },
		rend   = { glyph = glyphs['ever_tree'], color = "dark forest_green" },
	},

	tropical_rainforest = {
		name = "Tropical Rainforest", minTemp = 33, maxTemp = 100, minRain = 25, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coasts"] },
		soils = { soil = 93, sand = 7 },
		plants = { grass = 15, plump_helmet = 2, },
		trees = { deciduous = 55, evergreen = 0 },
		rend = { glyph = glyphs['deci_tree'], color = "forest_green" },
	},
	
	-- Deserts
	tundra = {
		name = "Tundra", minTemp = -100, maxTemp = 12, minRain = 5, maxRain = 95,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"], biome_types["plateaus"] },
		soils  = { soil = 75, sand = 25 },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 2, evergreen = 4 },
		rend   = { glyph = glyphs['grass'], color = "light ice" },
	},

	temperate_desert = {
		name = "Desert", minTemp = 25, maxTemp = 100, minRain = 0, maxRain = 18,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"], biome_types["plateaus"], biome_types["highlands"] },
		soils = { soil = 0, sand = 100 },
		plants = { grass = 25 },
		trees  = { deciduous = 2, evergreen = 1 },
		rend = { glyph = glyphs['sand1'], color = "light wheat" },
	},

	tropical_desert = {
		name = "Desert", minTemp = 33, maxTemp = 100, minRain = 0, maxRain = 10,
		occurs = { biome_types["plains"], biome_types["hills"], biome_types["coasts"] },
		soils = { soil = 0, sand = 100 },
		plants = { grass = 15 },
		trees  = { deciduous = 2, evergreen = 1 },
		rend = { glyph = glyphs['sand1'], color = "light wheat" },
	},
};